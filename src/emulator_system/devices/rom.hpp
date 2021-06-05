#pragma once

#include "emulator/device.hpp"

namespace postrisc {

/***********************************************************************
* the rom device config space
************************************************************************/
class RomConfigSpace {
friend class boost::serialization::access;
public:
    RomConfigSpace();
    template<class Archive> void serialize(Archive& ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u64                     size;
    space_filler<u64, 15>   reserved;
};

static_assert(sizeof(RomConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<RomConfigSpace>::value);

template<class Archive>
void
RomConfigSpace::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(size);
}

/***********************************************************************
* the rom device
************************************************************************/
class device_rom final : public Device {
public:
    explicit device_rom(const std::string& name_, const std::vector<u8>& data);
    ~device_rom(void) override;

    STANDARD_DEVICE_INTERFACE

    const std::vector<u8>& contents(void) { return m_data; }

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& archive, const unsigned int version);

private:
    std::vector<u8>    m_data;
    RomConfigSpace     m_config;
};

template<class Archive>
void
device_rom::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";
    archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    archive & BOOST_SERIALIZATION_NVP(m_data);
}

} // namespace postrisc
