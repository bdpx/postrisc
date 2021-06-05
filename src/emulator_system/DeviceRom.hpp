#pragma once

#include "emulator/Device.hpp"

namespace postrisc {

/***********************************************************************
* the rom device config space
************************************************************************/
class RomConfigSpace {
friend class boost::serialization::access;
public:
    RomConfigSpace();
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u64                     size = 0;
    space_filler<u64, 15>   reserved;
};

static_assert(sizeof(RomConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<RomConfigSpace>::value);

template<typename Archive>
void
RomConfigSpace::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(size);
}

/***********************************************************************
* the rom device
************************************************************************/
class DeviceRom final : public Device {
public:
    explicit DeviceRom(const std::string& name_, const std::vector<std::byte>& data);
    ~DeviceRom(void) override;

    STANDARD_DEVICE_INTERFACE

    const std::vector<std::byte>& contents(void) { return m_data; }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& archive, const unsigned int version);

private:
    std::vector<std::byte>  m_data;
    RomConfigSpace          m_config;
};

template<typename Archive>
void
DeviceRom::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";
    archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    archive & BOOST_SERIALIZATION_NVP(m_data);
}

} // namespace postrisc
