#pragma once

#include "emulator/Device.hpp"

namespace postrisc {

class TargetSystem;

/***********************************************************************
* the special device array device config space
************************************************************************/
class DeviceArrayConfigSpace {
friend class boost::serialization::access;
public:
    DeviceArrayConfigSpace(u32 max_config_space_size);
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader      header;
    u64                    pci_did = 0;
    u64                    pci_cmd = 0;
    u64                    device_array_address = 0;
    u64                    device_array_len = 0;
    u32                    max_size;
    u32                    pad = 0;
    space_filler<u64, 32 + 3> reserved;
};

static_assert(sizeof(DeviceArrayConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<DeviceArrayConfigSpace>::value);

template<typename Archive>
void
DeviceArrayConfigSpace::serialize(Archive& archive, const unsigned int UNUSED(version))
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
class DeviceArray final : public Device {
friend class TargetSystem;
public:
    DeviceArray(const std::string& name_, u32 max_config_space_size);
    ~DeviceArray() override {}

    STANDARD_DEVICE_INTERFACE

    Core *get_bootstrap_core(void) override;
    void FlushTranslationBuffer(void);
    void FlushTranslationBuffer(u64 address);

    Result broadcast_interrupt(Result vector) const;
    void activate_execution(TargetSystem& targetSystem) const;
    Result add_device(boost::intrusive_ptr<Device> device);

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

private:
    u32                                         m_max_config_space_size;
    std::vector<boost::intrusive_ptr<Device>>   m_Devices;
    DeviceArrayConfigSpace                      config;
};

template<typename Archive>
void
DeviceArray::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    ar & BOOST_SERIALIZATION_NVP(m_Devices);
}

} // namespace postrisc
