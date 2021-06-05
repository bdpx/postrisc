#pragma once

namespace postrisc {

using postrisc::fpu::u128;

/***************************************************************************
* instruction breakpoint register
***************************************************************************/
class CInstructionBreakpointRegister {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

public:
    CInstructionBreakpointRegister() : address(0), mask(0) {}

    u128 Get(void) const;
    void Set(u128 value);
    void reset(void);

    friend std::ostream & operator << (std::ostream & out, const CInstructionBreakpointRegister & r);

private:
    uint64_t address;
    uint64_t mask;
};

template<class Archive>
void
CInstructionBreakpointRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
u128
CInstructionBreakpointRegister::Get(void) const
{
    return u128(address, mask);
}

inline
void
CInstructionBreakpointRegister::Set(u128 value)
{
    address = value.high();
    mask = value.low();
}

inline
void CInstructionBreakpointRegister::reset(void)
{
    address = 0;
    mask = 0;
}

/***************************************************************************
* data breakpoint register
***************************************************************************/
class CDataBreakpointRegister {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

public:
    CDataBreakpointRegister() : address(0), mask(0) {}

    u128 Get(void) const;
    void Set(u128 value);
    void reset(void);

    friend std::ostream & operator << (std::ostream & out, const CDataBreakpointRegister & r);

private:
    uint64_t address;
    uint64_t mask;
};

template<class Archive>
void
CDataBreakpointRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
u128
CDataBreakpointRegister::Get(void) const
{
    return u128(address, mask);
}

inline
void
CDataBreakpointRegister::Set(u128 value)
{
    address = value.high();
    mask = value.low();
}

inline
void CDataBreakpointRegister::reset(void)
{
    address = 0;
    mask = 0;
}

} // namespace postrisc
