#pragma once

namespace postrisc {

template<size_t LINESIZE>
class alignas(16) InstructionCacheLine {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
public:
    ALWAYS_INLINE const CBundle& GetBundle(uint64_t ip) const { return data[(ip / sizeof(CBundle)) % data.size()]; }

private:
    std::array<CBundle, LINESIZE / sizeof(CBundle)> data;
};

template<size_t LINESIZE>
template<class Archive>
void
InstructionCacheLine<LINESIZE>::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(data);
}

} // namespace postrisc
