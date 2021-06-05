#include "util/common.hpp"
#include "video.hpp"

namespace postrisc {

VideoConfigSpace::VideoConfigSpace()
    : header(0x0012, sizeof(VideoConfigSpace))
    , width(0)
    , height(0)
{
}

void
VideoConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

device_video::device_video(TargetSystem * parent_, const std::string& name_, size_t width, size_t height)
    : Device(parent_, name_)
    , m_Width(width)
    , m_Height(height)
{
    m_Data.assign(width * height, 0);
    m_Data.resize(util::align_up(m_Data.size(), Device::minimal_size_alignment), 0xff);
    config.height = height;
    config.width = width;
}

device_video::~device_video(void)
{
}

uint64_t
device_video::size(void) const
{
    return m_Data.size();
}

uint64_t
device_video::get_config_size(void) const
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
device_video::dump_config(uint64_t address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<uint8_t const*>(&config), sizeof(config), out);
}

void
device_video::dump_statistic(const DumpFormatter& out) const
{
    out << "video" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

CStatus
device_video::read(device_address_t address, size_t len, void *bytes)
{
    if (address + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid load: " << fmt::hex(address)
            << " size=" << len
            << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes));

        return CStatus(CStatus::device_error);
    }

    memcpy(bytes, &m_Data[address], len);
    return CStatus(CStatus::continue_execution);
}

CStatus
device_video::write(device_address_t address, size_t len, const void * bytes)
{
    if (address + len > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid store: " << fmt::hex(address)
            << " size=" << len
            << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes));

        return CStatus(CStatus::device_error);
    }

    memcpy(&m_Data[address], bytes, len);
    return CStatus(CStatus::continue_execution);
}

CStatus
device_video::complex_operation(device_address_t address, IMemoryOperation& op)
{
    if (address > m_Data.size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid cached access: " << fmt::hex(address);

        return CStatus(CStatus::device_error);
    }

    void * p = &m_Data[address];
    return op.complex_operation(p);
}

CStatus
device_video::read_config(device_address_t address, size_t len, void *bytes)
{
    switch (address) {
        case offsetof(VideoConfigSpace, width):
            LOG_TRACE(LOG_PLATFORM) << "read width: " << config.width;
            break;
        case offsetof(VideoConfigSpace, height):
            LOG_TRACE(LOG_PLATFORM) << "read height: " << config.height;
            break;
        default:
            LOG_TRACE(LOG_PLATFORM) << "read unsupported";
            break;
    }
    return read_config_raw(address, &config, sizeof(config), len, bytes);
}

CStatus
device_video::write_config(device_address_t address, size_t len, const void *bytes)
{
    if (address + len > size()) {
        LOG_ERROR(LOG_DISPLAY)
            << "invalid write: " << fmt::hex(address)
            << " size=" << len
            << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes));

        return CStatus(CStatus::device_error);
    }

    LOG_TRACE(LOG_DISPLAY)
        << "address=" << fmt::hex(address)
        << " size=" << len
        << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes))
        << fmt::endl;

    switch (address) {
        case offsetof(VideoConfigSpace, clear):
            if (len == sizeof(uint32_t)) {
                const uint32_t value = * reinterpret_cast<uint32_t const*>(bytes);
                clear(value);
            }
            break;
        
        case offsetof(VideoConfigSpace, redraw):
            if (len == sizeof(uint32_t)) {
                redraw();
            }
            break;

        default:
           LOG_TRACE(LOG_DISPLAY) << "invalid write address: "
                                  << fmt::hex(address) << ' ' << len;
           return CStatus(CStatus::device_error);
    }

    return write_config_raw(address, &config, sizeof(config), len, bytes);
}

void
device_video::clear(uint32_t color)
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
    LOG_OUTPUT(LOG_DISPLAY) << "redraw"
        << fmt::endl;
}

} // namespace postrisc
