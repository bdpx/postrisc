#pragma once

#include <random>

namespace postrisc {

class CRandomUnit {
public:
    CRandomUnit(u64 seed)
        : distribution(std::numeric_limits<u64>::min(),
                       std::numeric_limits<u64>::max())
    {
        Write(seed);
    }

    u64 Read()
    {
        const u64 result = value;
        value = distribution(generator);
        return result;
    }

    void Write(u64 seed)
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
    u64                                   value;
    std::default_random_engine            generator;
    std::uniform_int_distribution<u64>    distribution;
};

} // namespace postrisc
