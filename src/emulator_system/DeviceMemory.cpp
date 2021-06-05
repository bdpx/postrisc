#include "util/common.hpp"
#include "emulator/Result.hpp"
#include "config/configuration.hpp"
#include "DeviceMemory.hpp"

namespace postrisc {

MemoryConfigSpace::MemoryConfigSpace()
    : header(0x0014, sizeof(MemoryConfigSpace))
{
}

void
MemoryConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

DeviceMemory::DeviceMemory(const std::string& name_, size_t size)
    : Device(name_)
{
    Init(util::roundup_pow2(size, Device::minimal_size_alignment));
    config.size = m_Data.size();
}

DeviceMemory::~DeviceMemory()
{
}

u64 DeviceMemory::size(void) const
{
    return m_Data.size();
}

u64 DeviceMemory::config_size(void) const
{
    return sizeof(config);
}

#define READ_TYPED_HANDLER(TYPE)                                                        \
Result DeviceMemory::read_##TYPE(const DeviceOffset device_offset, TYPE& data) const    \
{                                                                                       \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                       \
    u64 const offset = device_offset.get_offset();                                      \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                     \
    TYPE const *host_address = reinterpret_cast<TYPE const *>(&m_Data[offset]);         \
    data = *host_address;                                                               \
    return Result::continue_execution;                                                  \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

#define WRITE_TYPED_HANDLER(TYPE)                                                   \
Result DeviceMemory::write_##TYPE(const DeviceOffset device_offset, TYPE data)      \
{                                                                                   \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                   \
    u64 const offset = device_offset.get_offset();                                  \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                 \
    TYPE *host_address = reinterpret_cast<TYPE *>(&m_Data[offset]);                 \
    *host_address = data;                                                           \
    return Result::continue_execution;                                              \
}

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

Result DeviceMemory::read(const DeviceOffset device_offset, size_t len, void* bytes) const
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << "len=" << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    std::byte const *host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(bytes, host_address, len);
    return Result::continue_execution;
}

Result DeviceMemory::write(const DeviceOffset device_offset, size_t len, const void* bytes)
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << "len=" << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    std::byte *host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(host_address, bytes, len);
    return Result::continue_execution;
}

Result DeviceMemory::complex_operation(const DeviceOffset device_offset, size_t len, MemoryOperation& op)
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << ' ' << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    std::byte *host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    return op.complex_operation(host_address);
}

Result DeviceMemory::read_config(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    return read_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

Result DeviceMemory::write_config(const DeviceOffset device_offset, size_t len, const void *bytes)
{
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << device_offset << ' ' << len;
    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

void DeviceMemory::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_Data.data(), m_Data.size(), out);
}

void DeviceMemory::dump_config(const PhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<const std::byte *>(&config), sizeof(config), out);
}

void DeviceMemory::dump_statistic(const DumpFormatter& out) const
{
    out << "memory: " << m_Data.size() << " bytes" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

void DeviceMemory::Init(u64 size)
{
    m_Data.assign(size, std::byte{0});
}

} // namespace postrisc
