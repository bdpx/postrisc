#include "util/common.hpp"
#include "DeviceVideo.hpp"

namespace postrisc {

VideoConfigSpace::VideoConfigSpace()
    : header(0x0012, sizeof(VideoConfigSpace))
{
}

void VideoConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

DeviceVideo::DeviceVideo(const std::string& name_, size_t width, size_t height)
    : Device(name_)
    , m_Width(width)
    , m_Height(height)
{
    m_Data.assign(width * height * sizeof(u32), std::byte{0});
    m_Data.resize(util::roundup_pow2(m_Data.size(), Device::minimal_size_alignment), std::byte{0xff});
    config.height = height;
    config.width = width;
}

DeviceVideo::~DeviceVideo(void)
{
}

u64 DeviceVideo::size(void) const
{
    return m_Data.size();
}

u64 DeviceVideo::config_size(void) const
{
    return sizeof(config);
}

void DeviceVideo::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_Data.data(), m_Data.size(), out);
}

void DeviceVideo::dump_config(const PhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<const std::byte*>(&config), sizeof(config), out);
}

void DeviceVideo::dump_statistic(const DumpFormatter& out) const
{
    out << "video" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

#define READ_TYPED_HANDLER(TYPE)                                                         \
Result DeviceVideo::read_##TYPE(const DeviceOffset device_offset, TYPE& data) const {    \
    LOG_DEBUG(LOG_PLATFORM) << "address=" << device_offset;                              \
    u64 const offset = device_offset.get_offset();                                       \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                      \
    TYPE const *host_address = reinterpret_cast<TYPE const*>(&m_Data[offset]);           \
    data = *host_address;                                                                \
    return Result::continue_execution;                                                   \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

Result DeviceVideo::read(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid load: " << device_offset
            << " size=" << len;

        return Result::device_error;
    }

    memcpy(bytes, &m_Data[offset], len);
    return Result::continue_execution;
}

#define WRITE_TYPED_HANDLER(TYPE)                                                  \
Result DeviceVideo::write_##TYPE(const DeviceOffset device_offset, TYPE data) {    \
    LOG_DEBUG(LOG_PLATFORM) << "offset=" << device_offset;                         \
    u64 const offset = device_offset.get_offset();                                 \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                \
    TYPE *host_address =                                                           \
        reinterpret_cast<TYPE *>(&m_Data[offset]);                                 \
    *host_address = data;                                                          \
    return Result::continue_execution;                                             \
}

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

Result DeviceVideo::write(const DeviceOffset device_offset, size_t len, const void *bytes)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid store: " << device_offset
            << " size=" << len;

        return Result::device_error;
    }

    memcpy(&m_Data[offset], bytes, len);
    return Result::continue_execution;
}

Result DeviceVideo::complex_operation(const DeviceOffset device_offset, size_t len, MemoryOperation& op)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len >= m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid cached access: " << device_offset;

        return Result::device_error;
    }

    void * p = &m_Data[offset];
    return op.complex_operation(p);
}

Result DeviceVideo::read_config(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    u64 const offset = device_offset.get_offset();
    switch (offset) {
        case offsetof(VideoConfigSpace, width):
            LOG_DEBUG(LOG_PLATFORM) << "read width: " << config.width;
            break;
        case offsetof(VideoConfigSpace, height):
            LOG_DEBUG(LOG_PLATFORM) << "read height: " << config.height;
            break;
        default:
            LOG_DEBUG(LOG_PLATFORM) << "read unsupported";
            break;
    }
    return read_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

Result DeviceVideo::write_config(const DeviceOffset device_offset, size_t len, const void *bytes)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid write: " << device_offset
            << " size=" << len;

        return Result::device_error;
    }

    LOG_DEBUG(LOG_DISPLAY)
        << "offset=" << device_offset
        << " size=" << len
        << fmt::endl;

    switch (offset) {
        case offsetof(VideoConfigSpace, clear):
            if (len == sizeof(u32)) {
                const u32 value = *reinterpret_cast<u32 const*>(bytes);
                clear(value);
            }
            break;

        case offsetof(VideoConfigSpace, redraw):
            if (len == sizeof(u32)) {
                redraw();
            }
            break;

        case offsetof(VideoConfigSpace, foreground_color):
            if (len == sizeof(u32)) {
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "fgcolor=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, background_color):
            if (len == sizeof(u32)) {
                const u32 value = *reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "bgcolor=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_pos_x):
            if (len == sizeof(u32)) {
                const u32 value = *reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "pos_x=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_pos_y):
            if (len == sizeof(u32)) {
                const u32 value = *reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "pos_y=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_value):
            if (len == sizeof(u32)) {
                const u32 value = *reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "char=" << value;
            }
            break;

        default:
           LOG_DEBUG(LOG_DISPLAY) << "invalid write offset: "
                                  << device_offset << ' ' << len;
           return Result::device_error;
    }

    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

void DeviceVideo::clear(u32 color)
{
    LOG_OUTPUT(LOG_DISPLAY) << "clear"
                            << " pixelnum=" << m_Data.size()
                            << " color=" << fmt::hex(color)
                            << fmt::endl;

    for (auto& x : m_Data) {
        x = std::byte{static_cast<u8>(color)};
    }
}

void DeviceVideo::redraw(void)
{
    LOG_OUTPUT(LOG_DISPLAY) << "redraw" << fmt::endl;
    m_Redraw.store(1);
}

void DeviceVideo::CheckRedraw(size_t &width, size_t &height)
{
    if (!m_Redraw.exchange(0)) return;

    width = this->width();
    height = this->height();
}

} // namespace postrisc
