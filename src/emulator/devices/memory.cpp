#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "configuration.hpp"
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

device_memory::device_memory(TargetSystem * parent_, const std::string& name_, size_t size)
    : Device(parent_, name_)
{
    Init(util::align_up(size, Device::minimal_size_alignment));
    config.size = m_Data.size();
}

device_memory::~device_memory()
{
}

uint64_t
device_memory::size(void) const
{
    return m_Data.size();
}

uint64_t
device_memory::get_config_size(void) const
{
    return sizeof(config);
}

CStatus
device_memory::read(device_address_t address, size_t len, void* bytes)
{
    LOG_TRACE(LOG_PLATFORM) << "address=" << fmt::hex(address) << "len=" << len;
    assert(address + len <= m_Data.size());
    uint8_t const * host_address = &m_Data[address];
    LOG_TRACE(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(bytes, host_address, len);
    return CStatus(CStatus::continue_execution);
}

CStatus
device_memory::write(device_address_t address, size_t len, const void* bytes)
{
    LOG_TRACE(LOG_PLATFORM) << "address=" << fmt::hex(address) << "len=" << len;
    assert(address + len <= m_Data.size());
    uint8_t * const host_address = &m_Data[address];
    LOG_TRACE(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    memcpy(host_address, bytes, len);
    return CStatus(CStatus::continue_execution);
}

CStatus
device_memory::complex_operation(device_address_t address, IMemoryOperation& op)
{
    LOG_TRACE(LOG_PLATFORM) << "address=" << fmt::hex(address);
    assert(address + op.size() <= m_Data.size());
    uint8_t * const host_address = &m_Data[address];
    LOG_TRACE(LOG_PLATFORM) << "host_address=" << fmt::hex(host_address);
    return op.complex_operation(host_address);
}

CStatus
device_memory::read_config(device_address_t address, size_t len, void *bytes)
{
    return read_config_raw(address, &config, sizeof(config), len, bytes);
}

CStatus
device_memory::write_config(device_address_t address, size_t len, const void *bytes)
{
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << fmt::hex(address) << ' ' << len;
    return write_config_raw(address, &config, sizeof(config), len, bytes);
}

void
device_memory::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(m_Data.data(), m_Data.size(), out);
}

void
device_memory::dump_config(uint64_t address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<uint8_t const*>(&config), sizeof(config), out);
}

void
device_memory::dump_statistic(const DumpFormatter& out) const
{
    out << "memory: " << m_Data.size() << " bytes" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

void
device_memory::Init(uint64_t size)
{
    m_Data.assign(size, 0);
}

} // namespace postrisc
