#include "util/common.hpp"
#include "emulator/interruption.hpp"
#include "config/emulator_configuration.hpp"
#include "memory.hpp"

namespace postrisc {

MemoryConfigSpace::MemoryConfigSpace()
    : header(0x0014, sizeof(MemoryConfigSpace))
    , size(0)
{
}

void
MemoryConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
}

device_memory::device_memory(const std::string& name_, size_t size)
    : Device(name_)
{
    Init(util::align_up(size, Device::minimal_size_alignment));
    config.size = m_Data.size();
}

device_memory::~device_memory()
{
}

u64 device_memory::size(void) const
{
    return m_Data.size();
}

u64 device_memory::get_config_size(void) const
{
    return sizeof(config);
}

#define READ_TYPED_HANDLER(TYPE)                                                        \
CStatus device_memory::read_##TYPE(const CDeviceOffset device_offset, TYPE& data) const \
{                                                                                       \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                       \
    u64 const offset = device_offset.get_offset();                                      \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                     \
    TYPE const *host_address = reinterpret_cast<TYPE const *>(&m_Data[offset]);         \
    data = *host_address;                                                               \
    return CStatus::continue_execution;                                                 \
}

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

#define WRITE_TYPED_HANDLER(TYPE)                                                   \
CStatus device_memory::write_##TYPE(const CDeviceOffset device_offset, TYPE data)   \
{                                                                                   \
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset;                   \
    u64 const offset = device_offset.get_offset();                                  \
    assert(offset + sizeof(TYPE) <= m_Data.size());                                 \
    TYPE *host_address = reinterpret_cast<TYPE *>(&m_Data[offset]);                 \
    *host_address = data;                                                           \
    return CStatus::continue_execution;                                             \
}

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

CStatus
device_memory::read(const CDeviceOffset device_offset, size_t len, void* bytes) const
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << "len=" << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    u8 const * host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(bytes, host_address, len);
    return CStatus::continue_execution;
}

CStatus
device_memory::write(const CDeviceOffset device_offset, size_t len, const void* bytes)
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << "len=" << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    u8 * const host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(host_address, bytes, len);
    return CStatus::continue_execution;
}

CStatus
device_memory::complex_operation(const CDeviceOffset device_offset, size_t len, IMemoryOperation& op)
{
    LOG_DEBUG(LOG_PLATFORM) << "device_offset=" << device_offset << ' ' << len;
    u64 const offset = device_offset.get_offset();
    assert(offset + len <= m_Data.size());
    u8 * const host_address = &m_Data[offset];
    LOG_DEBUG(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    return op.complex_operation(host_address);
}

CStatus
device_memory::read_config(const CDeviceOffset device_offset, size_t len, void *bytes) const
{
    return read_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

CStatus
device_memory::write_config(const CDeviceOffset device_offset, size_t len, const void *bytes)
{
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << device_offset << ' ' << len;
    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

void
device_memory::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_Data.data(), m_Data.size(), out);
}

void
device_memory::dump_config(const CPhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<u8 const*>(&config), sizeof(config), out);
}

void
device_memory::dump_statistic(const DumpFormatter& out) const
{
    out << "memory: " << m_Data.size() << " bytes" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

void
device_memory::Init(u64 size)
{
    m_Data.assign(size, 0);
}

} // namespace postrisc
