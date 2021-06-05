#pragma once

#include "DumpFormatter.hpp"

namespace postrisc {

enum CPUID_index {
    CPUID_SIZE,
    CPUID_VERSION,
    CPUID_CAPABILITIES,
    CPUID_L1I,
    CPUID_L1D,
    CPUID_L2D,
    CPUID_L3D,
    CPUID_L1_ITLB,
    CPUID_L1_DTLB,
    CPUID_L2_TLB,
    CPUID_PMR_MAX,
    CPUID_MAX
};

class Cpuid_CacheInfo {
public:
    Cpuid_CacheInfo(unsigned _rowsize, unsigned _associativity, unsigned _size)
        : rowsize(_rowsize)
        , size(_size)
        , associativity(_associativity)
    {}

    u64 Get(void) const {
        return (static_cast<u64>(rowsize)       <<  0)
             | (static_cast<u64>(associativity) <<  8)
             | (static_cast<u64>(size)          << 16);
    }


    friend std::ostream& operator<<(std::ostream& out, const Cpuid_CacheInfo& r) {
        return out << fmt::hex(r.Get())
            << " rowsize=" << r.rowsize
            << " assoc=" << r.associativity
            << " size=" << r.size;
    }

    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version)) {
        ar & BOOST_SERIALIZATION_NVP(rowsize);
        ar & BOOST_SERIALIZATION_NVP(size);
        ar & BOOST_SERIALIZATION_NVP(associativity);
    }

private:
    unsigned  rowsize;
    unsigned  size;
    unsigned  associativity;
};

class Cpuid_TLB {
public:
    Cpuid_TLB(unsigned _size) : size(_size) {}

    u64 Get(void) const { return size; }

    friend std::ostream& operator<<(std::ostream& out, const Cpuid_TLB& r) {
        return out << fmt::hex(r.Get()) << " size=" << r.size;
    }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version)) {
        ar & BOOST_SERIALIZATION_NVP(size);
    }

private:
    unsigned size;
};

class CpuIdentificationStore {
public:
    CpuIdentificationStore(void);
    u64 Get(u64 index) const;
    void dump(const DumpFormatter& out) const;

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

private:
    u64                version;
    u64                capabilities;
    Cpuid_CacheInfo    L1_ICache;
    Cpuid_CacheInfo    L1_DCache;
    Cpuid_CacheInfo    L2_DCache;
    Cpuid_CacheInfo    L3_DCache;
    Cpuid_TLB          L1_ITLB;
    Cpuid_TLB          L1_DTLB;
    Cpuid_TLB          L2_TLB;
};

template<typename Archive>
void CpuIdentificationStore::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(version);
    ar & BOOST_SERIALIZATION_NVP(capabilities);
    ar & BOOST_SERIALIZATION_NVP(L1_ICache);
    ar & BOOST_SERIALIZATION_NVP(L1_DCache);
    ar & BOOST_SERIALIZATION_NVP(L2_DCache);
    ar & BOOST_SERIALIZATION_NVP(L3_DCache);
    ar & BOOST_SERIALIZATION_NVP(L1_ITLB);
    ar & BOOST_SERIALIZATION_NVP(L1_DTLB);
    ar & BOOST_SERIALIZATION_NVP(L2_TLB);
}

} // namespace postrisc
