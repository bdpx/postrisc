#pragma once

#include <random>

namespace postrisc {

class CRandomUnit {
public:
    CRandomUnit(uint64_t seed)
        : distribution(std::numeric_limits<uint64_t>::min(),
                       std::numeric_limits<uint64_t>::max())
    {
        Write(seed);
    }

    uint64_t Read()
    {
        const uint64_t result = value;
        value = distribution(generator);
        return result;
    }

    void Write(uint64_t seed)
    {
        generator.seed(seed);
        value = distribution(generator);
    }

    friend std::ostream & operator << (std::ostream & out, const CRandomUnit & r)
    {
        return out << fmt::hex(r.value);
    }

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(value);
    }

private:
    uint64_t                                   value;
    std::default_random_engine                 generator;
    std::uniform_int_distribution<uint64_t>    distribution;
};

} // namespace postrisc
