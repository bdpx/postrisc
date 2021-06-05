#include "Device.hpp"
#include "util/common.hpp"
#include "config/configuration.hpp"

namespace postrisc {

ConfigSpaceHeader::ConfigSpaceHeader(u16 device, u32 size_)
    : device_id(device)
    , size(size_)
{
}

void
ConfigSpaceHeader::dump(const DumpFormatter& out) const
{
    out << fmt::table("vendor_id") << vendor_id << fmt::endl;
    out << fmt::table("device_id") << device_id << fmt::endl;
}

Device::Device(const std::string& name)
    : m_name(name)
{
    LOG_DEBUG(LOG_PLATFORM) << "+device " << m_name;
}

Device::~Device(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "-device " << m_name;
}

Result
Device::read(const DeviceOffset UNUSED(device_offset), size_t UNUSED(len), void *UNUSED(bytes)) const
{
    return Result::device_error;
}

Result
Device::write(const DeviceOffset UNUSED(device_offset), size_t UNUSED(len), const void *UNUSED(bytes))
{
    return Result::device_error;
}

Result
Device::read_config_raw(const DeviceOffset device_offset, void const *config, size_t config_size, size_t len, void *bytes) const
{
    u64 const offset = device_offset.get_offset();
    m_config_reads_count++;

    if (offset < config_size) {
        memcpy(bytes, reinterpret_cast<u8 const*>(config) + offset, std::min(config_size - offset, len));
    }
    if (offset + len > config_size) {
        size_t data_offset = offset < config_size ? config_size - offset : 0;
        memset(reinterpret_cast<u8*>(bytes) + data_offset, 0xff, offset + len - config_size);
    }

    return Result::continue_execution;
}

Result
Device::write_config_raw(const DeviceOffset device_offset, void *config, size_t config_size, size_t len, void const *bytes)
{
    u64 const offset = device_offset.get_offset();
    m_config_writes_count++;

    if (offset < config_size) {
        memcpy(reinterpret_cast<u8*>(config) + offset, bytes, std::min(config_size - offset, len));
    }
    if (offset + len > config_size) {
        // TODO: ignore write to non implemented part?
        //size_t data_offset = offset < config_size ? config_size - offset : 0;
        //memset(reinterpret_cast<u8*>(bytes) + data_offset, 0xff, offset + len - config_size);
    }

    return Result::continue_execution;
}

void
Device::dump_mapping(const PhysMemoryMapping& UNUSED(mapping), const DumpFormatter& UNUSED(out)) const
{
}

void
Device::dump_config_raw(const PhysAddress pa, const std::byte *config, size_t config_size, const DumpFormatter& out)
{
    u64 const address = pa.get_address();
    out.start_page(address, dump_page_mmio);
    out.dump_page(address, config, config_size);
    out.finish_page(address + sizeof(config));
}

void
Device::dump_statistic(const DumpFormatter& out) const
{
    out << fmt::table("reads") << m_reads_count << fmt::endl;
    out << fmt::table("writes") << m_writes_count << fmt::endl;
    out << fmt::table("complex_ops") << m_complex_ops_count << fmt::endl;
    out << fmt::table("config_reads") << m_config_reads_count << fmt::endl;
    out << fmt::table("config_writes") << m_config_writes_count << fmt::endl;
}

Core *
Device::get_bootstrap_core(void)
{
    // device isn't core by default
    return nullptr;
}

} // namespace postrisc
