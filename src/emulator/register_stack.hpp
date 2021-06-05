#pragma once

/*
initial state:         UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
1 frame:               11111111UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
2 frame:               111111112222222222222222222UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
3 frame:               1111111122222222222222222223333333333333333333333333UUUUUUUUUUUU
4 frame (16 spilled):  44SSSSSSSSSSSSSS222222222223333333333333333333333333444444444444
5 frame (16 spilled):  445555555555SSSS222222222223333333333333333333333333444444444444
6 frame (32 spilled):  44555555555566666666SSSSSSSSSSSS33333333333333333333444444444444
*/

namespace postrisc {

/***************************************************************************
* Register rotate and frame state: rsc
***************************************************************************/
class CRegisterStackControlRegister {
    enum {
         nbits         = 8,
         sof_offset    = 0 * nbits,
         bof_offset    = 1 * nbits,
         soc_offset    = 2 * nbits,
         ndirty_offset = 3 * nbits,
    };

public:
    CRegisterStackControlRegister(void);

    void set(u64 value);
    u64 get(void) const;
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
    void AllocateFrame(unsigned newSizeOfFrame);
    CStatus ShrinkFrame(unsigned newSizeOfFrame);
    void RestoreOnReturn(unsigned prevSizeOfFrame, unsigned framesize);
    unsigned PreserveFrame(unsigned int numregs);

    void CoverFrame(void);
    void reset(void);

    friend std::ostream& operator<<(std::ostream& out, const CRegisterStackControlRegister& rsc);

public:
    // number of the one spilling/filling batch for rotating general registers
    static const unsigned BatchGranularity = 4; // 64 bytes

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version);

private:
    unsigned  m_BottomOfFrame;   // bottom of frame
    unsigned  m_SizeOfFrame;     // size of frame
    unsigned  m_SizeOfClear;     // size of clean
    unsigned  m_SizeOfDirty;     // size of dirty
};

ALWAYS_INLINE
CRegisterStackControlRegister::CRegisterStackControlRegister(void)
    : m_BottomOfFrame(0)
    , m_SizeOfFrame(0)
    , m_SizeOfClear(0)
    , m_SizeOfDirty(0)
{}

ALWAYS_INLINE
void
CRegisterStackControlRegister::set(u64 value)
{
    const u64 mask = util::makemask(nbits);
    m_SizeOfFrame  = static_cast<unsigned>((value >>    sof_offset) & mask);
    SetBottomOfFrame(static_cast<unsigned>((value >>    bof_offset) & mask));
    m_SizeOfClear  = static_cast<unsigned>((value >>    soc_offset) & mask);
    m_SizeOfDirty  = static_cast<unsigned>((value >> ndirty_offset) & mask);
}

ALWAYS_INLINE
u64
CRegisterStackControlRegister::get(void) const
{
    return (static_cast<u64>(m_SizeOfFrame)    <<    sof_offset) |
           (static_cast<u64>(m_BottomOfFrame)  <<    bof_offset) |
           (static_cast<u64>(m_SizeOfClear)    <<    soc_offset) |
           (static_cast<u64>(m_SizeOfDirty)    << ndirty_offset);
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
    // return gpr[-sod] position
    unsigned const result = rotate(phys_stacked_regs - m_SizeOfDirty);
    assert(result % BatchGranularity == 0);
    return result;
}

ALWAYS_INLINE
unsigned int
CRegisterStackControlRegister::GetFillEntry(void) const
{
    return rotate(phys_stacked_regs - BatchGranularity - m_SizeOfDirty);
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
CRegisterStackControlRegister::AllocateFrame(unsigned newSizeOfFrame)
{
    assert(newSizeOfFrame <= max_stacked_framesize);
    // extend but not shrink
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "old sof=" << m_SizeOfFrame;
    m_SizeOfFrame = std::max(m_SizeOfFrame, newSizeOfFrame);
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "new sof=" << m_SizeOfFrame;
}

ALWAYS_INLINE
CStatus
CRegisterStackControlRegister::ShrinkFrame(unsigned newSizeOfFrame)
{
    assert(m_SizeOfFrame <= max_stacked_framesize);
    // shrink during subroutine return can't be more than current frame
    if (newSizeOfFrame > m_SizeOfFrame) {
        LOG_ERROR(LOG_REGISTER_ROTATION) << "newSizeOfFrame=" << newSizeOfFrame;
        return CStatus::invalid_parameter;
    }
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "sof: old=" << m_SizeOfFrame << " new=" <<newSizeOfFrame;
    m_SizeOfFrame = newSizeOfFrame;
    return CStatus::continue_execution;
}

ALWAYS_INLINE
unsigned
CRegisterStackControlRegister::PreserveFrame(unsigned int numregs)
{
    assert(numregs <= m_SizeOfFrame);
    SetBottomOfFrame((m_BottomOfFrame + numregs) % phys_stacked_regs);
    m_SizeOfDirty += numregs;
    assert(m_SizeOfDirty <= phys_stacked_regs);
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "old sof=" << m_SizeOfFrame;
    m_SizeOfFrame -= numregs;
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "new sof=" << m_SizeOfFrame;

    return m_SizeOfFrame;
}

ALWAYS_INLINE
void
CRegisterStackControlRegister::RestoreOnReturn(unsigned prevSizeOfFrame, unsigned framesize)
{
    assert(m_SizeOfDirty >= prevSizeOfFrame);
    m_SizeOfDirty -= prevSizeOfFrame;
    SetBottomOfFrame(rotate(phys_stacked_regs - prevSizeOfFrame));
    // AllocateFrame(prevSizeOfFrame + framesize);
    assert(prevSizeOfFrame + framesize <= max_stacked_framesize);
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "old sof=" << m_SizeOfFrame;
    m_SizeOfFrame = prevSizeOfFrame + framesize;
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "new sof=" << m_SizeOfFrame;
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
    m_SizeOfFrame = 0; //max_stacked_framesize;
    LOG_DEBUG(LOG_REGISTER_ROTATION) << "sof=" << m_SizeOfFrame;
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
