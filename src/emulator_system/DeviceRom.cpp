#include "util/common.hpp"
#include "DeviceRom.hpp"

namespace postrisc {

RomConfigSpace::RomConfigSpace()
    : header(0x0013, sizeof(RomConfigSpace))
{
}

void
RomConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

DeviceRom::DeviceRom(const std::string& name_, const std::vector<std::byte>& _data)
    : Device(name_), m_data(_data)
{
    m_data.resize(util::roundup_pow2(m_data.size(), Device::minimal_size_alignment), std::byte{0xff});
    m_config.size = m_data.size();
}

DeviceRom::~DeviceRom()
{
}

u64 DeviceRom::size(void) const
{
    return m_data.size();
}

u64 DeviceRom::config_size(void) const
{
    return sizeof(m_config);
}

void DeviceRom::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_data.data(), m_data.size(), out);
}

void DeviceRom::dump_config(const PhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<const std::byte*>(&m_config), sizeof(m_config), out);
}

void DeviceRom::dump_statistic(const DumpFormatter& out) const
{
    out << "rom" << fmt::endl;
    Device::dump_statistic(out);
    m_config.dump(out);
}

#define READ_TYPED_HANDLER(TYPE)                                                     \
Result DeviceRom::read_##TYPE(const DeviceOffset device_offset, TYPE& data) const \
{                                                                                    \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                    \
    u64 const offset = device_offset.get_offset();                                   \
    assert(offset + sizeof(TYPE) <= m_data.size());                                  \
    TYPE const *host_address = reinterpret_cast<TYPE const *>(&m_data[offset]);      \
    data = *host_address;                                                            \
    return Result::continue_execution;                                              \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

Result DeviceRom::read(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    u64 const offset = device_offset.get_offset();
    if (unlikely(offset + len > m_data.size())) {
        LOG_ERROR(LOG_PLATFORM)
            << "invalid read: " << device_offset
            << " size=" << len;

        return Result::device_error;
    }

    memcpy(bytes, &m_data[offset], len);
    return Result::continue_execution;
}

// ROM ignores any writes
#define WRITE_TYPED_HANDLER(TYPE)                                                           \
Result DeviceRom::write_##TYPE(const DeviceOffset device_offset, TYPE UNUSED(data))      \
{                                                                                           \
    LOG_WARNING(LOG_PLATFORM) << "device_offset=" << device_offset;                         \
    return Result::continue_execution;                                                     \
}

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

Result DeviceRom::write(const DeviceOffset device_offset, size_t len, const void *UNUSED(bytes))
{
    LOG_WARNING(LOG_PLATFORM)
        << "write: " << device_offset
        << " size=" << len;

    return Result::continue_execution;
}

Result DeviceRom::complex_operation(const DeviceOffset device_offset, size_t len, MemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "offset=" << device_offset
                            << " len=" << len;
    return Result::device_error;
}

Result DeviceRom::read_config(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    return read_config_raw(device_offset, &m_config, sizeof(m_config), len, bytes);
}

Result DeviceRom::write_config(const DeviceOffset device_offset, size_t len, const void *bytes)
{
    // TODO
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << device_offset << ' ' << len;
    return write_config_raw(device_offset, &m_config, sizeof(m_config), len, bytes);
}

} // namespace postrisc
