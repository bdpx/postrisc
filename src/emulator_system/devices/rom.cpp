#include "util/common.hpp"
#include "rom.hpp"

namespace postrisc {

RomConfigSpace::RomConfigSpace()
    : header(0x0013, sizeof(RomConfigSpace))
    , size(0)
{
}

void
RomConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

device_rom::device_rom(const std::string& name_, const std::vector<u8>& _data)
    : Device(name_), m_data(_data)
{
    m_data.resize(util::align_up(m_data.size(), Device::minimal_size_alignment), 0xff);
    m_config.size = m_data.size();
}

device_rom::~device_rom()
{
}

u64 device_rom::size(void) const
{
    return m_data.size();
}

u64 device_rom::get_config_size(void) const
{
    return sizeof(m_config);
}

void
device_rom::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_data.data(), m_data.size(), out);
}

void
device_rom::dump_config(const CPhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<u8 const*>(&m_config), sizeof(m_config), out);
}

void
device_rom::dump_statistic(const DumpFormatter& out) const
{
    out << "rom" << fmt::endl;
    Device::dump_statistic(out);
    m_config.dump(out);
}

#define READ_TYPED_HANDLER(TYPE)                                                     \
CStatus device_rom::read_##TYPE(const CDeviceOffset device_offset, TYPE& data) const \
{                                                                                    \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                    \
    u64 const offset = device_offset.get_offset();                                   \
    assert(offset + sizeof(TYPE) <= m_data.size());                                  \
    TYPE const *host_address = reinterpret_cast<TYPE const *>(&m_data[offset]);      \
    data = *host_address;                                                            \
    return CStatus::continue_execution;                                              \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

CStatus
device_rom::read(const CDeviceOffset device_offset, size_t len, void * bytes) const
{
    u64 const offset = device_offset.get_offset();
    if (unlikely(offset + len > m_data.size())) {
        LOG_ERROR(LOG_PLATFORM)
            << "invalid read: " << device_offset
            << " size=" << len;

        return CStatus::device_error;
    }

    memcpy(bytes, &m_data[offset], len);
    return CStatus::continue_execution;
}

// ROM ignores any writes
#define WRITE_TYPED_HANDLER(TYPE)                                                           \
CStatus device_rom::write_##TYPE(const CDeviceOffset device_offset, TYPE UNUSED(data))      \
{                                                                                           \
    LOG_WARNING(LOG_PLATFORM) << "device_offset=" << device_offset;                         \
    return CStatus::continue_execution;                                                     \
}

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

CStatus
device_rom::write(const CDeviceOffset device_offset, size_t len, const void *UNUSED(bytes))
{
    LOG_WARNING(LOG_PLATFORM)
        << "write: " << device_offset
        << " size=" << len;

    return CStatus::continue_execution;
}

CStatus
device_rom::complex_operation(const CDeviceOffset device_offset, size_t len, IMemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "offset=" << device_offset
                            << " len=" << len;
    return CStatus::device_error;
}

CStatus
device_rom::read_config(const CDeviceOffset device_offset, size_t len, void *bytes) const
{
    return read_config_raw(device_offset, &m_config, sizeof(m_config), len, bytes);
}

CStatus
device_rom::write_config(const CDeviceOffset device_offset, size_t len, const void *bytes)
{
    // TODO
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << device_offset << ' ' << len;
    return write_config_raw(device_offset, &m_config, sizeof(m_config), len, bytes);
}

} // namespace postrisc
