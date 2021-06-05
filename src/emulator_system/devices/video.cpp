#include "util/common.hpp"
#include "video.hpp"

namespace postrisc {

VideoConfigSpace::VideoConfigSpace()
    : header(0x0012, sizeof(VideoConfigSpace))
    , width(0)
    , height(0)
    , clear(0)
    , redraw(0)
    , foreground_color(0)
    , background_color(0)
    , character_pos_x(0)
    , character_pos_y(0)
    , character_value(0)
    , pad(0)
{
}

void
VideoConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

device_video::device_video(const std::string& name_, size_t width, size_t height)
    : Device(name_)
    , m_Width(width)
    , m_Height(height)
{
    m_Data.assign(width * height * sizeof(u32), 0);
    m_Data.resize(util::align_up(m_Data.size(), Device::minimal_size_alignment), 0xff);
    config.height = height;
    config.width = width;
}

device_video::~device_video(void)
{
}

u64 device_video::size(void) const
{
    return m_Data.size();
}

u64 device_video::get_config_size(void) const
{
    return sizeof(config);
}

void
device_video::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_Data.data(), m_Data.size(), out);
}

void
device_video::dump_config(const CPhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<u8 const*>(&config), sizeof(config), out);
}

void
device_video::dump_statistic(const DumpFormatter& out) const
{
    out << "video" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

#define READ_TYPED_HANDLER(TYPE)                                                         \
CStatus device_video::read_##TYPE(const CDeviceOffset device_offset, TYPE& data) const { \
    LOG_DEBUG(LOG_PLATFORM) << "address=" << device_offset;                              \
    u64 const offset = device_offset.get_offset();                                       \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                      \
    TYPE const * host_address = reinterpret_cast<TYPE const *>(&m_Data[offset]);         \
    data = *host_address;                                                                \
    return CStatus::continue_execution;                                                  \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

CStatus
device_video::read(const CDeviceOffset device_offset, size_t len, void *bytes) const
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid load: " << device_offset
            << " size=" << len;

        return CStatus::device_error;
    }

    memcpy(bytes, &m_Data[offset], len);
    return CStatus::continue_execution;
}

#define WRITE_TYPED_HANDLER(TYPE)                                                      \
    CStatus device_video::write_##TYPE(const CDeviceOffset device_offset, TYPE data) { \
        LOG_DEBUG(LOG_PLATFORM) << "offset=" << device_offset;                         \
        u64 const offset = device_offset.get_offset();                                 \
        assert(offset + sizeof(TYPE) <= m_Data.size());                                \
        TYPE *host_address =                                                           \
            reinterpret_cast<TYPE *>(&m_Data[offset]);                                 \
        *host_address = data;                                                          \
        return CStatus::continue_execution;                                            \
    }

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

CStatus
device_video::write(const CDeviceOffset device_offset, size_t len, const void * bytes)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid store: " << device_offset
            << " size=" << len;

        return CStatus::device_error;
    }

    memcpy(&m_Data[offset], bytes, len);
    return CStatus::continue_execution;
}

CStatus
device_video::complex_operation(const CDeviceOffset device_offset, size_t len, IMemoryOperation& op)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len >= m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid cached access: " << device_offset;

        return CStatus::device_error;
    }

    void * p = &m_Data[offset];
    return op.complex_operation(p);
}

CStatus
device_video::read_config(const CDeviceOffset device_offset, size_t len, void *bytes) const
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

CStatus
device_video::write_config(const CDeviceOffset device_offset, size_t len, const void *bytes)
{
    u64 const offset = device_offset.get_offset();
    if (offset + len > size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid write: " << device_offset
            << " size=" << len;

        return CStatus::device_error;
    }

    LOG_DEBUG(LOG_DISPLAY)
        << "offset=" << device_offset
        << " size=" << len
        << fmt::endl;

    switch (offset) {
        case offsetof(VideoConfigSpace, clear):
            if (len == sizeof(u32)) {
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
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
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "bgcolor=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_pos_x):
            if (len == sizeof(u32)) {
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "pos_x=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_pos_y):
            if (len == sizeof(u32)) {
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "pos_y=" << value;
            }
            break;

        case offsetof(VideoConfigSpace, character_value):
            if (len == sizeof(u32)) {
                const u32 value = * reinterpret_cast<u32 const*>(bytes);
                LOG_DEBUG(LOG_DISPLAY) << "char=" << value;
            }
            break;

        default:
           LOG_DEBUG(LOG_DISPLAY) << "invalid write offset: "
                                  << device_offset << ' ' << len;
           return CStatus::device_error;
    }

    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

void
device_video::clear(u32 color)
{
    LOG_OUTPUT(LOG_DISPLAY) << "clear"
                            << " pixelnum=" << m_Data.size()
                            << " color=" << fmt::hex(color)
                            << fmt::endl;

    for (auto & x : m_Data) {
        x = color;
    }
}

void
device_video::redraw(void)
{
    LOG_OUTPUT(LOG_DISPLAY) << "redraw" << fmt::endl;
    m_Redraw.store(1);
}

u8 const *
device_video::CheckRedraw(size_t &width, size_t &height)
{
    if (!m_Redraw.exchange(0)) return nullptr;

    width = this->width();
    height = this->height();

    return m_Data.data();
}

} // namespace postrisc
