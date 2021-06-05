#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "arch/isa.hpp"
#include "utils.hpp"
#include "core.hpp"
#include "target_system.hpp"
#include "array.hpp"

namespace postrisc {

DeviceArrayConfigSpace::DeviceArrayConfigSpace(uint32_t max_config_space_size)
    : header(0x0015, sizeof(DeviceArrayConfigSpace))
    , pci_did(0)
    , pci_cmd(0)
    , device_array_address(0)
    , device_array_len(0)
    , max_size(max_config_space_size)
    , pad(0)
{
}

void
DeviceArrayConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
    out << "pci_did:      " << fmt::hex(pci_did);
    out << "\npci_cmd:      " << fmt::hex(pci_cmd);
    out << "\ndevice_array_address: " << fmt::hex(device_array_address);
    out << "\ndevice_array_len: " << device_array_len << " bytes\n";
}

DeviceArray::DeviceArray(TargetSystem * parent_, const std::string& name_, uint32_t max_config_space_size)
    : Device(parent_, name_)
    , m_max_config_space_size(max_config_space_size)
    , config(max_config_space_size)
{
}

uint64_t
DeviceArray::size(void) const
{
    // +1 for DeviceArray itself as the first device in device array
    return (1 + m_Devices.size()) * m_max_config_space_size;
}

uint64_t
DeviceArray::get_config_size(void) const
{
    return sizeof(config);
}

CStatus
DeviceArray::add_device(boost::intrusive_ptr<Device> device)
{
    uint64_t const config_size = device->get_config_size();
    if (config_size > m_max_config_space_size) {
        LOG_ERROR(LOG_PLATFORM) << "too big device config size: " << config_size;
        return CStatus(CStatus::device_error);
    }
    m_Devices.emplace_back(device);
    return CStatus(CStatus::continue_execution);
}

void
DeviceArray::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    uint64_t address = mapping.get_address();
    out << "device[0] " << get_name() << fmt::endl;
    dump_config(address, out);
    address += m_max_config_space_size;

    for (size_t i = 0; i < m_Devices.size(); ++i) {
        out << "device[" << (i+1) << "] " << m_Devices[i]->get_name() << fmt::endl;
        m_Devices[i]->dump_config(address, out);
        address += m_max_config_space_size;
    }
}

void
DeviceArray::dump_config(uint64_t address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<uint8_t const*>(&config), sizeof(config), out);
}

void
DeviceArray::dump_statistic(const DumpFormatter& out) const
{
    out.chapter("devices bus statistic");
    out << (1 + m_Devices.size()) << " devices\n";

    out << "device[0] size=" << size() << " name=";
    out.start_device(get_name());
    Device::dump_statistic(out);
    config.dump(out);
    out << "max_config_space_size: " << m_max_config_space_size << fmt::endl;
    out.finish_device(get_name());

    for (size_t i = 0; i < m_Devices.size(); ++i) {
        out << "device[" << (i+1) << "] size=" << m_Devices[i]->size() << " name=";
        out.start_device(m_Devices[i]->get_name());
        m_Devices[i]->dump_statistic(out);
        out.finish_device(m_Devices[i]->get_name());
    }
    out << fmt::endl;
}

CStatus
DeviceArray::read(device_address_t address, size_t len, void* bytes)
{
    if (address + len > size()) {
        LOG_ERROR(LOG_INTERRUPT) << "unsupported address=" << fmt::hex(address);
        return CStatus(CStatus::data_unimplemented_address);
    }

    if (address < m_max_config_space_size) {
        return read_config(address, len, bytes);
    }

    address -= m_max_config_space_size;
    size_t const id = address / m_max_config_space_size;

    LOG_DEBUG(LOG_PLATFORM) << " id=" << id
                            << " address=" << fmt::hex(address);

    return m_Devices[id]->read_config(address % m_max_config_space_size, len, bytes);
}

CStatus
DeviceArray::write(device_address_t address, size_t len, const void* bytes)
{
    if (address + len > size()) {
        LOG_ERROR(LOG_INTERRUPT) << "unsupported address=" << fmt::hex(address);
        return CStatus(CStatus::data_unimplemented_address);
    }

    if (address < m_max_config_space_size) {
        return write_config(address, len, bytes);
    }

    address -= m_max_config_space_size;
    size_t const id = address / m_max_config_space_size;

    LOG_DEBUG(LOG_PLATFORM) << " id=" << id
                            << " address=" << fmt::hex(address);

    return m_Devices[id]->write_config(address % m_max_config_space_size, len, bytes);
}

CStatus
DeviceArray::complex_operation(device_address_t address, IMemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "invalid atomic: " << fmt::hex(address);
    return CStatus(CStatus::device_error);
}

CStatus
DeviceArray::read_config(device_address_t address, size_t len, void *bytes)
{
    typedef DeviceArrayConfigSpace space;
    switch (address) {
        case offsetof(space, pci_did):
            LOG_OUTPUT(LOG_DISPLAY) << "read pci_did" << fmt::endl;
            break;
        case offsetof(space, pci_cmd):
            LOG_OUTPUT(LOG_DISPLAY) << "read pci_cmd" << fmt::endl;
            break;
        case offsetof(space, device_array_address):
            LOG_OUTPUT(LOG_DISPLAY) << "read device_array_address" << fmt::endl;
            break;
        case offsetof(space, device_array_len):
            LOG_OUTPUT(LOG_DISPLAY) << "read device_array_len" << fmt::endl;
            break;
        default:
            LOG_FATAL(LOG_DISPLAY) << "can't read reserved control.register\n";
            return CStatus(CStatus::device_error);
    }

    return read_config_raw(address, &config, sizeof(config), len, bytes);
}

CStatus
DeviceArray::write_config(device_address_t address, size_t len, const void *bytes)
{
    typedef DeviceArrayConfigSpace space;
    LOG_DEBUG(LOG_PLATFORM) << fmt::hex(address) << ' ' << len;

    switch (address) {
        case offsetof(space, pci_did):
            if (len != STRUCT_MEMBER_SIZEOF(space, pci_did)) {
                LOG_FATAL(LOG_DISPLAY) << "pci_did is not " << STRUCT_MEMBER_SIZEOF(space, pci_did) << "bytes";
                return CStatus(CStatus::device_error);
            }
            LOG_OUTPUT(LOG_DISPLAY) << "writing pci_did: 0x"
                << fmt::hex(*reinterpret_cast<uint64_t const*>(bytes)) << fmt::endl;
            break;
        case offsetof(space, pci_cmd):
            if (len != STRUCT_MEMBER_SIZEOF(space, pci_cmd)) {
                LOG_FATAL(LOG_DISPLAY) << "pci_cmd is not " << STRUCT_MEMBER_SIZEOF(space, pci_cmd) << " bytes";
                return CStatus(CStatus::device_error);
            }
            LOG_OUTPUT(LOG_DISPLAY) << "writing pci_cmd: 0x"
                << fmt::hex(*reinterpret_cast<uint64_t const*>(bytes)) << fmt::endl;
            break;
        case offsetof(space, device_array_address):
            LOG_FATAL(LOG_DISPLAY) << "can't change control.device_array_address";
            return CStatus(CStatus::device_error);
        case offsetof(space, device_array_len):
            LOG_FATAL(LOG_DISPLAY) << "can't change control.device_array_len";
            return CStatus(CStatus::device_error);
        default:
            LOG_DEBUG(LOG_DISPLAY) << "can't write to reserved control.register";
            return CStatus(CStatus::device_error);
    }

    return write_config_raw(address, &config, sizeof(config), len, bytes);
}

Core *
DeviceArray::get_bootstrap_core(void)
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core) return core;
    }
    return nullptr;
}

void
DeviceArray::activate_execution(TargetSystem& targetSystem) const
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core)
            targetSystem.activate_execution(core);
    }
}

CStatus
DeviceArray::post_interrupt(CStatus ivec) const
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core)
            CHECK_STATUS(core->post_interrupt(ivec));
    }

    return CStatus(CStatus::continue_execution);
}

} // namespace postrisc
