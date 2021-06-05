#pragma once

namespace postrisc {

/***************************************************************************
* instruction breakpoint register
***************************************************************************/
class InstructionBreakpointRegister {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

public:
    InstructionBreakpointRegister(void) : address(0), mask(0) {}

    GeneralRegister Get(void) const;
    void Set(GeneralRegister value);
    void reset(void);

    friend std::ostream& operator<<(std::ostream& out, const InstructionBreakpointRegister& r)
    {
        return out << fmt::hex(r.address) << '_' << fmt::hex(r.mask);
    }

private:
    u64 address;
    u64 mask;
};

template<typename Archive>
void
InstructionBreakpointRegister::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
GeneralRegister
InstructionBreakpointRegister::Get(void) const
{
    return GeneralRegister(address, mask);
}

inline
void
InstructionBreakpointRegister::Set(GeneralRegister value)
{
    address = value.high();
    mask = value.low();
}

inline
void InstructionBreakpointRegister::reset(void)
{
    address = 0;
    mask = 0;
}

} // namespace postrisc
