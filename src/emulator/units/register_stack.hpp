#pragma once

namespace postrisc {

/***************************************************************************
* Register rotate and frame state: rsc
***************************************************************************/
class CRegisterStackControlRegister {
    enum {
         nbits         = 7,
         sof_offset    = 0,
         bof_offset    = nbits,
         soc_offset    = 2 * nbits,
         ndirty_offset = 3 * nbits
    };

public:
    void set(uint64_t value);
    uint64_t get(void) const;
    void SetBottomOfFrame(unsigned newBottomOfFrame);
    unsigned GetSizeOfFrame(void) const;
    bool inFrame(unsigned int r) const;
    bool IsOutOfFrame(unsigned regnum) const;
    unsigned int rotate(unsigned int regnum) const;
    unsigned int get_bod(void) const;
    unsigned int GetFillEntry(void) const;
    static unsigned int RegStaticToPhys(unsigned int regnum);
    static bool IsStaticPhysical(unsigned regnum);
    unsigned int PhysicalToLogical(unsigned regnum);
    bool CanAllocateFrame(int newSizeOfFrame) const;
    void ShrinkDirtySection(int count);
    bool CanRestoreFrame(unsigned sizeOfFrame) const;
    void EnhanceDirtySection(int count);

    // update register stack, mark numregs registers as dirty
    void AllocateFrame(int newSizeOfFrame);
    void RestoreOnReturn(int prevSizeOfFrame, int framesize);
    unsigned PreserveFrame(unsigned int numregs);

    void CoverFrame(void);
    void reset(void);

    friend std::ostream& operator << (std::ostream& out, const CRegisterStackControlRegister & rsc);

public:
    // number of the one spilling/filling batch for rotating general registers
    unsigned GetBatchGranularity(void) const { return 8; } // 128 bytes

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    unsigned  m_BottomOfFrame;   // bottom of frame
    unsigned  m_SizeOfFrame;     // size of frame
    unsigned  m_SizeOfClear;     // size of clean
    unsigned  m_SizeOfDirty;     // size of dirty
};

ALWAYS_INLINE
void
CRegisterStackControlRegister::set(uint64_t value)
{
    const uint64_t mask = util::makemask(nbits);
    m_SizeOfFrame  = static_cast<unsigned>((value >>    sof_offset) & mask);
    SetBottomOfFrame(static_cast<unsigned>((value >>    bof_offset) & mask));
    m_SizeOfClear  = static_cast<unsigned>((value >>    soc_offset) & mask);
    m_SizeOfDirty  = static_cast<unsigned>((value >> ndirty_offset) & mask);
}

ALWAYS_INLINE
uint64_t
CRegisterStackControlRegister::get(void) const
{
    return (static_cast<uint64_t>(m_SizeOfFrame)    <<    sof_offset) |
           (static_cast<uint64_t>(m_BottomOfFrame)  <<    bof_offset) |
           (static_cast<uint64_t>(m_SizeOfClear)    <<    soc_offset) |
           (static_cast<uint64_t>(m_SizeOfDirty)    << ndirty_offset);
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::SetBottomOfFrame(unsigned newBottomOfFrame)
{
    m_BottomOfFrame = newBottomOfFrame;
}

ALWAYS_INLINE
unsigned
CRegisterStackControlRegister::GetSizeOfFrame(void) const
{
    return m_SizeOfFrame;
}

ALWAYS_INLINE
bool
CRegisterStackControlRegister::inFrame(unsigned int r) const
{
    return r < base_rotate_regs + m_SizeOfFrame;
}

ALWAYS_INLINE
bool
CRegisterStackControlRegister::IsOutOfFrame(unsigned regnum) const
{
    return regnum >= m_SizeOfFrame;
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::rotate(unsigned int regnum) const
{
    assert(m_SizeOfFrame <= max_stacked_framesize);
    assert(m_BottomOfFrame < phys_stacked_regs);

    return (regnum + m_BottomOfFrame) % phys_stacked_regs;
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::get_bod(void) const
{
    return rotate(phys_stacked_regs - m_SizeOfDirty);
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::GetFillEntry(void) const
{
    return rotate(phys_stacked_regs - GetBatchGranularity() - m_SizeOfDirty);
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::RegStaticToPhys(unsigned int regnum)
{
    return regnum + (phys_stacked_regs - max_stacked_framesize);
}

ALWAYS_INLINE
bool
CRegisterStackControlRegister::IsStaticPhysical(unsigned regnum)
{
    return regnum > phys_stacked_regs;
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::PhysicalToLogical(unsigned regnum)
{
    if (IsStaticPhysical(regnum))
        return regnum - (phys_stacked_regs - max_stacked_framesize);
    else
        return (regnum + phys_stacked_regs - m_BottomOfFrame) % phys_stacked_regs;
}

ALWAYS_INLINE
bool
CRegisterStackControlRegister::CanAllocateFrame(int newSizeOfFrame) const
{
    return newSizeOfFrame + m_SizeOfDirty <= phys_stacked_regs;
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::ShrinkDirtySection(int count)
{
    m_SizeOfDirty -= count;
}

ALWAYS_INLINE
bool
CRegisterStackControlRegister::CanRestoreFrame(unsigned sizeOfFrame) const
{
    return m_SizeOfDirty >= sizeOfFrame;
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::EnhanceDirtySection(int count)
{
    m_SizeOfDirty += count;
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::AllocateFrame(int newSizeOfFrame)
{
    m_SizeOfFrame = newSizeOfFrame;
}

ALWAYS_INLINE
unsigned
CRegisterStackControlRegister::PreserveFrame(unsigned int numregs)
{
    assert(numregs <= m_SizeOfFrame);
    SetBottomOfFrame((m_BottomOfFrame + numregs) % phys_stacked_regs);
    m_SizeOfDirty += numregs;
    assert(m_SizeOfDirty <= phys_stacked_regs);
    m_SizeOfFrame -= numregs;

    return m_SizeOfFrame;
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::RestoreOnReturn(int prevSizeOfFrame, int framesize)
{
    m_SizeOfDirty -= prevSizeOfFrame;
    SetBottomOfFrame(rotate(phys_stacked_regs - prevSizeOfFrame));
    AllocateFrame(prevSizeOfFrame + framesize);
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::CoverFrame(void)
{
    PreserveFrame(m_SizeOfFrame);
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::reset(void)
{
    // after reset all stacked registers are available
    m_SizeOfFrame = max_stacked_framesize;
    SetBottomOfFrame(0);
    m_SizeOfClear = 0;
    m_SizeOfDirty = 0;
}

template<class Archive>
void
CRegisterStackControlRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_BottomOfFrame);
    ar & BOOST_SERIALIZATION_NVP(m_SizeOfFrame);
    ar & BOOST_SERIALIZATION_NVP(m_SizeOfClear);
    ar & BOOST_SERIALIZATION_NVP(m_SizeOfDirty);
}

} // namespace postrisc
