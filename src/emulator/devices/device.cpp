#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "configuration.hpp"
#include "target_system.hpp"

namespace postrisc {

ConfigSpaceHeader::ConfigSpaceHeader(uint16_t device, uint32_t size_)
    : vendor_id(UINT16_C(404))
    , device_id(device)
    , command(0)
    , status(0)
    , subclass(0)
    , bist(0)
    , bar0(~UINT64_C(0))
    , bar1(~UINT64_C(0))
    , bar2(~UINT64_C(0))
    , cardbus(0)
    , subsystem(0)
    , rom_base(0xffffffff)
    , cap_pointer(0)
    , size(size_)
    , pad(0)
{
}

void
ConfigSpaceHeader::dump(const DumpFormatter& out) const
{
    out << fmt::table("vendor_id") << vendor_id << fmt::endl;
    out << fmt::table("device_id") << device_id << fmt::endl;
}

Device::Device(TargetSystem *target_system, const std::string& name)
    : m_reads_count(0)
    , m_writes_count(0)
    , m_complex_ops_count(0)
    , m_config_reads_count(0)
    , m_config_writes_count(0)
    , m_name(name)
    , m_target_system(target_system)
{
    LOG_DEBUG(LOG_PLATFORM) << "+device " << m_name;
}

Device::~Device(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "-device " << m_name;
}

CStatus
Device::read(device_address_t UNUSED(address), size_t UNUSED(len), void *UNUSED(bytes))
{
    return CStatus(CStatus::device_error);
}

CStatus
Device::write(device_address_t UNUSED(address), size_t UNUSED(len), const void *UNUSED(bytes))
{
    return CStatus(CStatus::device_error);
}

CStatus
Device::read_config_raw(device_address_t address, void const *config, size_t config_size, size_t len, void *bytes)
{
    m_config_reads_count++;

    if (address < config_size) {
        memcpy(bytes, reinterpret_cast<uint8_t const*>(config) + address, std::min(config_size - address, len));
    }
    if (address + len > config_size) {
        size_t data_offset = address < config_size ? config_size - address : 0;
        memset(reinterpret_cast<uint8_t*>(bytes) + data_offset, 0xff, address + len - config_size);
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Device::write_config_raw(device_address_t address, void *config, size_t config_size, size_t len, void const *bytes)
{
    m_config_writes_count++;

    if (address < config_size) {
        memcpy(reinterpret_cast<uint8_t*>(config) + address, bytes, std::min(config_size - address, len));
    }
    if (address + len > config_size) {
        // TODO: ignore write to non implemented part?
        //size_t data_offset = address < config_size ? config_size - address : 0;
        //memset(reinterpret_cast<uint8_t*>(bytes) + data_offset, 0xff, address + len - config_size);
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Device::complex_operation(device_address_t UNUSED(address), IMemoryOperation& UNUSED(op))
{
    return CStatus(CStatus::device_error);
}

void
Device::dump_mapping(const PhysMemoryMapping& UNUSED(mapping), const DumpFormatter& UNUSED(out)) const
{
}

void
Device::dump_config_raw(device_address_t address, uint8_t const *config, size_t config_size, const DumpFormatter& out)
{
    out.start_page(address, dump_page_mmio);
    dump_page(address, config, config_size, out);
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

void
Device::dump_page(uint64_t address, uint8_t const *base, size_t page_size, const DumpFormatter& out)
{
    size_t const linesize = 32;
    for (size_t i = 0; i < page_size; i += linesize) {
        out.dump_physical_address(address + i);
        out << ':';
        for (size_t k = 0; k < linesize; ++k) {
            if (k % 8 == 0) out << ' ';
            if (i + k < page_size) {
                out << fmt::hex(base[i+k]);
            } else {
                out << ' ' << ' ';
            }
        }
        out << ' ';
        for (size_t k = 0; k < linesize; ++k) {
            if (k % 8 == 0) out << ' ';
            if (i + k < page_size) {
                out.dump_byte(base[i+k]);
            } else {
                break;
            }
        }
        out << fmt::endl;
    }
}

void
PhysMemoryMapping::dump(uint8_t const *base, size_t device_size, const DumpFormatter& out) const
{
    uint64_t address = get_address();
    uint64_t size    = get_size();
    uint64_t offset  = get_offset();

    unsigned const page_size = m_device->get_target_system()->get_page_size();

    for (uint64_t page_address = address, iii = 0; page_address < address + size && offset < device_size; ) {
        if (page_address == address || page_address % page_size == 0) {
            dump_page_flags flags = dump_page_mmio;
            if (!m_page_bitmap.empty()) {
                flags |= dump_page_memory;
                if (m_page_bitmap.at(iii / page_size)) {
                    flags |= dump_page_allocated;
                }
            }
            out.start_page(page_address, flags);
        }
        size_t const page_rest = page_size - page_address % page_size;
        size_t const nbytes = std::min(device_size - offset, page_rest);
        Device::dump_page(page_address, base + offset, nbytes, out);
        offset += page_rest;
        page_address += page_rest;
        iii += page_rest;
        out.finish_page(page_address);
    }
}

} // namespace postrisc
