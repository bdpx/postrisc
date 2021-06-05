#pragma once

#include "device.hpp"

namespace postrisc {

/***********************************************************************
* the rom device config space
************************************************************************/
class RomConfigSpace {
friend class boost::serialization::access;
public:
    RomConfigSpace();
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader            header;
    uint64_t                     size;
    space_filler<uint64_t, 15>   reserved;
};

STATIC_ASSERT(sizeof(RomConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
STATIC_ASSERT(std::is_standard_layout<RomConfigSpace>::value);

template<class Archive>
void
RomConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(size);
}

/***********************************************************************
* the rom device
************************************************************************/
class device_rom : public Device {
public:
    explicit device_rom(TargetSystem * parent_, const std::string& name_, const std::vector<uint8_t>& data);
    ~device_rom(void) override;

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

    const std::vector<uint8_t>& contents(void) { return data; }

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & archive, const unsigned int version);

private:
    std::vector<uint8_t>  data;
    RomConfigSpace        config;
};

template<class Archive>
void
device_rom::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";
    archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    archive & BOOST_SERIALIZATION_NVP(data);
}

} // namespace postrisc
