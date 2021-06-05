#pragma once

#include "general_register.hpp"

namespace postrisc {

namespace fmt {

class future {
  public:
      future(u32 val) : v(val) {}
      friend std::ostream& operator<< (std::ostream& out, const future r)
      {
          return out << std::bitset<sizeof(u32)*8>(r.v);
      }

  private:
      u32 v;
};
}

/***************************************************************************
* psr, ipsr
***************************************************************************/
class CProcessorStatusRegister {
public:
    enum {
        FLAG_PERFORMANCE_MONITOR            = 1 << 0,
        FLAG_INSTRUCTION_DEBUG_TRAP         = 1 << 1,
        FLAG_DATA_DEBUG_TRAP                = 1 << 2,
        FLAG_LOVER_PRIVILEGED_TRAP          = 1 << 3,
        FLAG_TAKEN_BRANCH_TRAP              = 1 << 4,
        FLAG_SINGLE_STEP_TRAP               = 1 << 5,
        FLAG_INTERRUPT_COLLECTION           = 1 << 6,
        FLAG_INTERRUPTION                   = 1 << 7,
        FLAG_MACHINE_CHECK                  = 1 << 8,
        FLAG_PRIVILEGED_PERFORMANCE_MONITOR = 1 << 9,
        FLAG_CURRENT_PRIVILEGY_LEVEL        = 1 << 10,

        FLAGS_DEBUG = FLAG_INSTRUCTION_DEBUG_TRAP
                    | FLAG_DATA_DEBUG_TRAP
                    | FLAG_LOVER_PRIVILEGED_TRAP
                    | FLAG_TAKEN_BRANCH_TRAP
                    | FLAG_SINGLE_STEP_TRAP,
        FLAGS_ALL = FLAGS_DEBUG
                  | FLAG_PERFORMANCE_MONITOR
                  | FLAG_INTERRUPT_COLLECTION
                  | FLAG_INTERRUPTION
                  | FLAG_MACHINE_CHECK
                  | FLAG_PRIVILEGED_PERFORMANCE_MONITOR
                  | FLAG_CURRENT_PRIVILEGY_LEVEL
    };

public:
    void reset(void);
    void SetFuture(u32 f);
    void ClearFuture(void);
    u32 GetFuture(void) const;
    u64 get(void) const;
    void NullifyFuture(u32 mask);
    bool PopFuture(void);
    unsigned GetShadowSize(void) const;
    unsigned PopShadow(void) { return ++ri; }
    void SetShadow(unsigned _ri) { ri = _ri; }
    void ClearShadow(void);
    void set(u64 value);
    bool IsLastInstructionInBundle(void) const { return isLastInstructionInBundle; }
    void SetIsLastInstructionInBundle(bool v) { isLastInstructionInBundle = v; }

    bool IsKernelMode(void) const { return (flags & FLAG_CURRENT_PRIVILEGY_LEVEL) == 0; }
    unsigned privilegy_level(void) const { return (flags & FLAG_CURRENT_PRIVILEGY_LEVEL) ? 0 : 1; }
    void set_kernel_mode(void) { flags &= ~FLAG_CURRENT_PRIVILEGY_LEVEL; }

    bool IsInterruptDisabled(void) const { return (flags & FLAG_INTERRUPTION) != 0; }

    bool is_interrupt_collection_enabled(void) const { return (flags & FLAG_INTERRUPT_COLLECTION) != 0; }
    void disable_interrupt_collection(void) { flags &= ~FLAG_INTERRUPT_COLLECTION; };

    std::ostream& dump(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const CProcessorStatusRegister& r) {
        return r.dump(out);
    }

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    bool        isLastInstructionInBundle = false;
    unsigned    flags = 0;    // one-bit flags: start in kernel mode
    unsigned    ri = 0;       // restart instruction slot (shadow zone between ip's)
    u32         future = 0;   // future nullification mask
};

template<class Archive> void CProcessorStatusRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(flags);
    ar & BOOST_SERIALIZATION_NVP(future);
    ar & BOOST_SERIALIZATION_NVP(ri);
    ar & BOOST_SERIALIZATION_NVP(isLastInstructionInBundle);
}

ALWAYS_INLINE void CProcessorStatusRegister::NullifyFuture(u32 mask)
{
    // future nullification
    future |= mask;

    LOG_DEBUG(LOG_PREDICATION)
        << "nullification=0b" << fmt::future(mask)
        << " future=0b" << fmt::future(future);
}

ALWAYS_INLINE unsigned CProcessorStatusRegister::GetShadowSize(void) const
{
    return ri;
}

ALWAYS_INLINE void CProcessorStatusRegister::ClearShadow(void)
{
    // on the new bundle start
    ri = 0;
}

ALWAYS_INLINE void CProcessorStatusRegister::ClearFuture(void)
{
    // on each branch/jump/call we clear future predication
    future = 0;
}

ALWAYS_INLINE void CProcessorStatusRegister::SetFuture(u32 f)
{
    // on each return we restore saved future predication
    future = f;
}

ALWAYS_INLINE u32 CProcessorStatusRegister::GetFuture(void) const
{
    return future;
}

ALWAYS_INLINE bool CProcessorStatusRegister::PopFuture(void)
{
    // get least significant bit
    bool const is_nullified = future & 1;
    // drop least significant bit, add zero from end
    future >>= 1;

    LOG_DEBUG(LOG_PREDICATION)
        << "pop=" << is_nullified
        << " future=0b" << fmt::future(future);

    return is_nullified;
}

} // namespace postrisc
