#pragma once

namespace postrisc {

template<size_t LINESIZE>
class alignas(16) InstructionCacheLine {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);
public:
    ALWAYS_INLINE const Bundle& GetBundle(u64 ip) const { return data[(ip / sizeof(Bundle)) % data.size()]; }

private:
    std::array<Bundle, LINESIZE / sizeof(Bundle)> data;
};

template<size_t LINESIZE>
template<typename Archive>
void
InstructionCacheLine<LINESIZE>::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(data);
}

} // namespace postrisc
