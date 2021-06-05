#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "arch/isa.hpp"
#include "emulator/core.hpp"
#include "target_system.hpp"
#include "array.hpp"

namespace postrisc {

DeviceArrayConfigSpace::DeviceArrayConfigSpace(u32 max_config_space_size)
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

DeviceArray::DeviceArray(const std::string& name_, u32 max_config_space_size)
    : Device(name_)
    , m_max_config_space_size(max_config_space_size)
    , config(max_config_space_size)
{
}

u64 DeviceArray::size(void) const
{
    // +1 for DeviceArray itself as the first device in device array
    return (1 + m_Devices.size()) * m_max_config_space_size;
}

u64 DeviceArray::get_config_size(void) const
{
    return sizeof(config);
}

CStatus
DeviceArray::add_device(boost::intrusive_ptr<Device> device)
{
    u64 const config_size = device->get_config_size();
    if (config_size > m_max_config_space_size) {
        LOG_ERROR(LOG_PLATFORM) << "too big device config size: " << config_size;
        return CStatus::device_error;
    }
    m_Devices.emplace_back(device);
    return CStatus::continue_execution;
}

void
DeviceArray::dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const
{
    u64 address = mapping.get_address();
    out.start_section();
    out << "device[0] " << get_name() << fmt::endl;
    out.finish_section();

    dump_config(CPhysAddress(address), out);
    address += m_max_config_space_size;

    for (size_t i = 0; i < m_Devices.size(); ++i) {
        out.start_section();
        out << "device[" << (i+1) << "] " << m_Devices[i]->get_name() << fmt::endl;
        out.finish_section();

        m_Devices[i]->dump_config(CPhysAddress(address), out);
        address += m_max_config_space_size;
    }
}

void
DeviceArray::dump_config(const CPhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<const std::byte*>(&config), sizeof(config), out);
}

void
DeviceArray::dump_statistic(const DumpFormatter& out) const
{
    out.chapter("devices bus statistic");

    out.start_section();
    out << (1 + m_Devices.size()) << " devices\n";
    out << "device[0] size=" << size() << " name=";
    out.finish_section();

    out.start_device(get_name());
    Device::dump_statistic(out);
    config.dump(out);
    out << "max_config_space_size: " << m_max_config_space_size << fmt::endl;
    out.finish_device(get_name());

    for (size_t i = 0; i < m_Devices.size(); ++i) {
        out.start_section();
        out << "device[" << (i+1) << "] size=" << m_Devices[i]->size() << " name=";
        out.finish_section();
        out.start_device(m_Devices[i]->get_name());
        m_Devices[i]->dump_statistic(out);
        out.finish_device(m_Devices[i]->get_name());
    }
    out << fmt::endl;
}

#define READ_TYPED_HANDLER(TYPE)                                                        \
    CStatus DeviceArray::read_##TYPE(const CDeviceOffset address, TYPE& data) const {   \
        return read(address, sizeof(TYPE), &data);                                      \
    }

READ_TYPED_HANDLER(u8)
READ_TYPED_HANDLER(u16)
READ_TYPED_HANDLER(u32)
READ_TYPED_HANDLER(u64)
READ_TYPED_HANDLER(u128)

CStatus
DeviceArray::read(const CDeviceOffset device_offset, size_t len, void* bytes) const
{
    u64 offset = device_offset.get_offset();
    if (offset + len > size()) {
        LOG_ERROR(LOG_INTERRUPT) << "unsupported offset=" << device_offset;
        return CStatus::bus_unimplemented_address;
    }

    if (offset < m_max_config_space_size) {
        return read_config(device_offset, len, bytes);
    }

    offset -= m_max_config_space_size;
    size_t const id = offset / m_max_config_space_size;
    CDeviceOffset daddr2(offset % m_max_config_space_size);

    LOG_DEBUG(LOG_PLATFORM) << " id=" << id
                            << " offset=" << daddr2;

    return m_Devices[id]->read_config(daddr2, len, bytes);
}

#define WRITE_TYPED_HANDLER(TYPE)                                               \
    CStatus DeviceArray::write_##TYPE(const CDeviceOffset offset, TYPE data) {    \
        return write(offset, sizeof(TYPE), &data);                             \
    }

WRITE_TYPED_HANDLER(u8)
WRITE_TYPED_HANDLER(u16)
WRITE_TYPED_HANDLER(u32)
WRITE_TYPED_HANDLER(u64)
WRITE_TYPED_HANDLER(u128)

CStatus
DeviceArray::write(const CDeviceOffset device_offset, size_t len, const void* bytes)
{
    u64 offset = device_offset.get_offset();
    if (offset + len > size()) {
        LOG_ERROR(LOG_INTERRUPT) << "unsupported offset=" << device_offset;
        return CStatus::data_unimplemented_address;
    }

    if (offset < m_max_config_space_size) {
        return write_config(device_offset, len, bytes);
    }

    offset -= m_max_config_space_size;
    size_t const id = offset / m_max_config_space_size;
    CDeviceOffset daddr2(offset % m_max_config_space_size);

    LOG_DEBUG(LOG_PLATFORM) << " id=" << id
                            << " device_offset=" << daddr2;

    return m_Devices[id]->write_config(daddr2, len, bytes);
}

CStatus
DeviceArray::complex_operation(const CDeviceOffset device_offset, size_t UNUSED(len), IMemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "invalid atomic: " << device_offset;
    return CStatus::device_error;
}

CStatus
DeviceArray::read_config(const CDeviceOffset device_offset, size_t len, void *bytes) const
{
    typedef DeviceArrayConfigSpace space;
    u64 const offset = device_offset.get_offset();
    switch (offset) {
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
            LOG_ERROR(LOG_DISPLAY) << "can't read reserved control.register\n";
            return CStatus::device_error;
    }

    return read_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

CStatus
DeviceArray::write_config(const CDeviceOffset device_offset, size_t len, const void *bytes)
{
    typedef DeviceArrayConfigSpace space;
    LOG_DEBUG(LOG_PLATFORM) << device_offset << ' ' << len;

    u64 const offset = device_offset.get_offset();
    switch (offset) {
        case offsetof(space, pci_did):
            if (len != STRUCT_MEMBER_SIZEOF(space, pci_did)) {
                LOG_ERROR(LOG_DISPLAY) << "pci_did is not " << STRUCT_MEMBER_SIZEOF(space, pci_did) << "bytes";
                return CStatus::device_error;
            }
            LOG_OUTPUT(LOG_DISPLAY) << "writing pci_did: 0x"
                << fmt::hex(*reinterpret_cast<u64 const*>(bytes)) << fmt::endl;
            break;
        case offsetof(space, pci_cmd):
            if (len != STRUCT_MEMBER_SIZEOF(space, pci_cmd)) {
                LOG_ERROR(LOG_DISPLAY) << "pci_cmd is not " << STRUCT_MEMBER_SIZEOF(space, pci_cmd) << " bytes";
                return CStatus::device_error;
            }
            LOG_OUTPUT(LOG_DISPLAY) << "writing pci_cmd: 0x"
                << fmt::hex(*reinterpret_cast<u64 const*>(bytes)) << fmt::endl;
            break;
        case offsetof(space, device_array_address):
            LOG_ERROR(LOG_DISPLAY) << "can't change control.device_array_address";
            return CStatus::device_error;
        case offsetof(space, device_array_len):
            LOG_ERROR(LOG_DISPLAY) << "can't change control.device_array_len";
            return CStatus::device_error;
        default:
            LOG_DEBUG(LOG_DISPLAY) << "can't write to reserved control.register";
            return CStatus::device_error;
    }

    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
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

void DeviceArray::FlushTranslationBuffer(void)
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core) core->FlushTranslationBuffer();
    }
}

void DeviceArray::FlushTranslationBuffer(u64 address)
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core) core->FlushTranslationBuffer(address);
    }
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
DeviceArray::broadcast_interrupt(CStatus ivec) const
{
    for (auto const & device : m_Devices) {
        Core *core = device->get_bootstrap_core();
        if (core)
            CHECK_STATUS(core->post_interrupt(ivec));
    }

    return CStatus::continue_execution;
}

} // namespace postrisc
