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

device_rom::device_rom(TargetSystem * parent_, const std::string& name_, const std::vector<uint8_t>& _data)
    : Device(parent_, name_), data(_data)
{
    data.resize(util::align_up(data.size(), Device::minimal_size_alignment), 0xff);
    config.size = data.size();
}

device_rom::~device_rom()
{
}

uint64_t
device_rom::size(void) const
{
    return data.size();
}

uint64_t
device_rom::get_config_size(void) const
{
    return sizeof(config);
}

void
device_rom::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    Device::dump_mapping(mapping, out);
    mapping.dump(data.data(), data.size(), out);
}

void
device_rom::dump_config(uint64_t address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<uint8_t const*>(&config), sizeof(config), out);
}

void
device_rom::dump_statistic(const DumpFormatter& out) const
{
    out << "rom" << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
}

CStatus
device_rom::read(device_address_t address, size_t len, void * bytes)
{
    if (unlikely(address + len > data.size())) {
        LOG_ERROR(LOG_PLATFORM)
            << "invalid read: " << fmt::hex(address)
            << " size=" << len
            << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes));

        return CStatus(CStatus::device_error);
    }

    memcpy(bytes, &data[address], len);
    return CStatus(CStatus::continue_execution);
}

CStatus
device_rom::write(device_address_t address, size_t len, const void * bytes)
{
    LOG_ERROR(LOG_PLATFORM)
        << "write: " << fmt::hex(address)
        << " size=" << len
        << " hostptr=" << fmt::hex(reinterpret_cast<size_t>(bytes));

    return CStatus(CStatus::device_error);
}

CStatus
device_rom::complex_operation(device_address_t address, IMemoryOperation& op)
{
    LOG_ERROR(LOG_PLATFORM) << "address=" << fmt::hex(address)
                            << " size=" << op.size();
    return CStatus(CStatus::device_error);
}

CStatus
device_rom::read_config(device_address_t address, size_t len, void *bytes)
{
    return read_config_raw(address, &config, sizeof(config), len, bytes);
}

CStatus
device_rom::write_config(device_address_t address, size_t len, const void *bytes)
{
    // TODO
    LOG_ERROR(LOG_PLATFORM) << "invalid: " << fmt::hex(address) << ' ' << len;
    return write_config_raw(address, &config, sizeof(config), len, bytes);
}

} // namespace postrisc
