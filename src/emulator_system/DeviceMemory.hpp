#pragma once

#include <boost/align/aligned_allocator.hpp>

#include "emulator/Device.hpp"

namespace postrisc {

/***********************************************************************
* the memory device config space
************************************************************************/
class MemoryConfigSpace {
friend class boost::serialization::access;
public:
    MemoryConfigSpace();
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u64                     size = 0;
    space_filler<u64, 23>   reserved;
};

static_assert(sizeof(MemoryConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<MemoryConfigSpace>::value);

template<typename Archive>
void
MemoryConfigSpace::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(size);
}

/***********************************************************************
* the memory device
************************************************************************/
template<typename T>
using
aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 64 * 1024>>;

class DeviceMemory final : public Device {
public:
    explicit DeviceMemory(const std::string& name_, size_t size);
    ~DeviceMemory() override;

    STANDARD_DEVICE_INTERFACE

    size_t ComputeAllocatedSize(void) const;

private:
    void Init(u64 size);

    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

private:
    aligned_vector<std::byte>  m_Data;
    MemoryConfigSpace          config;
};

template<typename Archive>
void
DeviceMemory::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);
    ar & BOOST_SERIALIZATION_NVP(m_Data);
}

} // namespace postrisc
