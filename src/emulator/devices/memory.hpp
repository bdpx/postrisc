#pragma once

#include <boost/align/aligned_allocator.hpp>

#include "device.hpp"

namespace postrisc {

/***********************************************************************
* the memory device config space
************************************************************************/
class MemoryConfigSpace {
friend class boost::serialization::access;
public:
    MemoryConfigSpace();
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader            header;
    uint64_t                     size;
    space_filler<uint64_t, 23>   reserved;
};

STATIC_ASSERT(sizeof(MemoryConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
STATIC_ASSERT(std::is_standard_layout<MemoryConfigSpace>::value);

template<class Archive>
void
MemoryConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(size);
}

/***********************************************************************
* the memory device
************************************************************************/
template<class T>
using
aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 64 * 1024>>;

class device_memory : public Device {
public:
    explicit device_memory(TargetSystem * parent_, const std::string& name_, size_t size);
    ~device_memory() override;

    CStatus read(device_address_t address, size_t len, void* bytes) override;
    CStatus write(device_address_t address, size_t len, const void* bytes) override;
    CStatus complex_operation(device_address_t address, IMemoryOperation& op) override;
    CStatus read_config(device_address_t address, size_t len, void *bytes) override;
    CStatus write_config(device_address_t addr, size_t len, const void *bytes) override;
    uint64_t size(void) const override;
    uint64_t get_config_size(void) const override;
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;
    void dump_config(uint64_t address, const DumpFormatter& out) const override;
    void dump_statistic(const DumpFormatter& out) const override;

    size_t ComputeAllocatedSize(void) const;

private:
    void Init(uint64_t size);

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    aligned_vector<uint8_t>   m_Data;
    MemoryConfigSpace         config;
};

template<class Archive>
void
device_memory::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    ar & BOOST_SERIALIZATION_NVP(m_Data);
}

} // namespace postrisc
