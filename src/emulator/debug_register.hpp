#pragma once

namespace postrisc {

/***************************************************************************
* instruction breakpoint register
***************************************************************************/
class CInstructionBreakpointRegister {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version);

public:
    CInstructionBreakpointRegister(void) : address(0), mask(0) {}

    CGeneralRegister Get(void) const;
    void Set(CGeneralRegister value);
    void reset(void);

    friend std::ostream& operator<<(std::ostream& out, const CInstructionBreakpointRegister& r);

private:
    u64 address;
    u64 mask;
};

template<class Archive>
void
CInstructionBreakpointRegister::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
CGeneralRegister
CInstructionBreakpointRegister::Get(void) const
{
    return CGeneralRegister(address, mask);
}

inline
void
CInstructionBreakpointRegister::Set(CGeneralRegister value)
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
    CDataBreakpointRegister(void) : address(0), mask(0) {}

    CGeneralRegister Get(void) const;
    void Set(CGeneralRegister value);
    void reset(void);

    friend std::ostream& operator<<(std::ostream& out, const CDataBreakpointRegister& r);

private:
    u64 address;
    u64 mask;
};

template<class Archive>
void
CDataBreakpointRegister::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(address);
    ar & BOOST_SERIALIZATION_NVP(mask);
}

inline
CGeneralRegister
CDataBreakpointRegister::Get(void) const
{
    return CGeneralRegister(address, mask);
}

inline
void
CDataBreakpointRegister::Set(CGeneralRegister value)
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
