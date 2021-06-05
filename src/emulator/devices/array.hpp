#pragma once

#include "device.hpp"

namespace postrisc {

/***********************************************************************
* the special device array device config space
************************************************************************/
class DeviceArrayConfigSpace {
friend class boost::serialization::access;
public:
    DeviceArrayConfigSpace(uint32_t max_config_space_size);
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader           header;
    uint64_t                    pci_did;
    uint64_t                    pci_cmd;
    uint64_t                    device_array_address;
    uint64_t                    device_array_len;
    uint32_t                    max_size;
    uint32_t                    pad;
    space_filler<uint64_t, 32>  reserved;
};

STATIC_ASSERT(sizeof(DeviceArrayConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
STATIC_ASSERT(std::is_standard_layout<DeviceArrayConfigSpace>::value);

template<class Archive>
void
DeviceArrayConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(pci_did);
    archive & BOOST_SERIALIZATION_NVP(pci_cmd);
}

/********************************************************************
* special device to control another devices in the system
* similar to PCI-Express memory-mapped config space (PCIE ECAM)
* 4KiB config space per device is available
********************************************************************/
class DeviceArray : public Device {
friend class TargetSystem;
public:
    DeviceArray(TargetSystem * parent_, const std::string& name_, uint32_t max_config_space_size);
    ~DeviceArray() override {}

    CStatus read(device_address_t address, size_t len, void* bytes) override;
    CStatus write(device_address_t address, size_t len, const void* bytes) override;
    CStatus complex_operation(device_address_t address, IMemoryOperation& op) override;
    CStatus read_config(device_address_t address, size_t len, void *bytes) override;
    CStatus write_config(device_address_t address, size_t len, const void *bytes) override;
    uint64_t size(void) const override;
    uint64_t get_config_size(void) const override;
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;
    void dump_config(uint64_t address, const DumpFormatter& out) const override;
    void dump_statistic(const DumpFormatter& out) const override;
    Core *get_bootstrap_core(void) override;

    CStatus post_interrupt(CStatus vector) const;
    void activate_execution(TargetSystem& targetSystem) const;
    CStatus add_device(boost::intrusive_ptr<Device> device);

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    uint32_t                                    m_max_config_space_size;
    std::vector<boost::intrusive_ptr<Device>>   m_Devices;
    DeviceArrayConfigSpace                      config;
};

template<class Archive>
void
DeviceArray::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    ar & BOOST_SERIALIZATION_NVP(m_Devices);
}

} // namespace postrisc
