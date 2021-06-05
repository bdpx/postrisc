#pragma once

/***************************************************************************
* data breakpoint register
***************************************************************************/

namespace postrisc {

class DataBreakpointRegister {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

public:
    DataBreakpointRegister(void) : address(0), mask(0) {}

    GeneralRegister Get(void) const;
    void Set(GeneralRegister value);
    void reset(void);

    friend std::ostream& operator<<(std::ostream& out, const DataBreakpointRegister& r)
    {
        return out << fmt::hex(r.address) << '_' << fmt::hex(r.mask);
    }

private:
    u64 address;
    u64 mask;
};

template<typename Archive>
void
DataBreakpointRegister::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
GeneralRegister
DataBreakpointRegister::Get(void) const
{
    return GeneralRegister(address, mask);
}

inline
void
DataBreakpointRegister::Set(GeneralRegister value)
{
    address = value.high();
    mask = value.low();
}

inline
void DataBreakpointRegister::reset(void)
{
    address = 0;
    mask = 0;
}

} // namespace postrisc
