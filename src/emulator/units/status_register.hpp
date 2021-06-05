#pragma once

namespace postrisc {

namespace fmt {

class future {
  public:
      future(uint64_t val) : v(val) {}
      friend std::ostream& operator<< (std::ostream& out, const future r)
      {
          return out << std::bitset<16>(r.v);
      }

  private:
      uint64_t v;
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
    void ClearFuture(void);
    fpu::u128 get(void) const;
    void NullifyFuture(uint64_t mask);
    void PushBackFuture(void);
    bool PopFuture(void);
    unsigned GetShadowSize(void) const;
    bool IsShadowed(unsigned shadow_size);
    void ClearShadow(void);
    void Raise(void);
    void Trap(void);
    void set(uint64_t value);

    bool IsKernelMode(void) const { return (flags & FLAG_CURRENT_PRIVILEGY_LEVEL) == 0; }
    unsigned privilegy_level(void) const { return (flags & FLAG_CURRENT_PRIVILEGY_LEVEL) ? 0 : 1; }
    void set_kernel_mode(void) { flags &= ~FLAG_CURRENT_PRIVILEGY_LEVEL; }

    bool IsInterruptDisabled(void) const { return (flags & FLAG_INTERRUPTION) != 0; }

    bool is_interrupt_collection_enabled(void) const { return (flags & FLAG_INTERRUPT_COLLECTION) != 0; }
    void disable_interrupt_collection(void) { flags &= ~FLAG_INTERRUPT_COLLECTION; };

    std::ostream & dump(std::ostream & out) const;

    friend std::ostream & operator << (std::ostream & out, const CProcessorStatusRegister & r);

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    unsigned    flags;    // one-bit flags
    unsigned    ri;       // restart instruction slot (shadow zone between ip's)
    uint64_t    future;   // future nullification mask
};

template<class Archive>
void
CProcessorStatusRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(flags);
    ar & BOOST_SERIALIZATION_NVP(future);
    ar & BOOST_SERIALIZATION_NVP(ri);
}

ALWAYS_INLINE
unsigned
CProcessorStatusRegister::GetShadowSize(void) const
{
    return ri;
}

ALWAYS_INLINE
void
CProcessorStatusRegister::ClearShadow(void)
{
    // on the new bundle start
    ri = 0;
}

// check if we restart after interuption
// to skip first slots in bundle
ALWAYS_INLINE
bool
CProcessorStatusRegister::IsShadowed(unsigned shadow_size)
{
    if (ri < shadow_size) {
        LOG_TRACE(LOG_EMULATOR)
            << "ri=" << ri
            << " shadow=" << shadow_size;
        ++ri;
        return true;
    }
    return false;
}

ALWAYS_INLINE
void
CProcessorStatusRegister::ClearFuture(void)
{
    // on each branch/call/jump we clear future predication
    future = 0;
}

ALWAYS_INLINE
bool
CProcessorStatusRegister::PopFuture(void)
{
    // get least significant bit
    bool const is_nullified = future & 1;

    // drop least significant bit, add zero from end
    future >>= 1;

    // extend shadow size
    ri++;

    LOG_TRACE(LOG_PREDICATION)
        << "pop=" << is_nullified
        << " future=0b" << fmt::future(future)
        << " ri=" << ri;

    return is_nullified;
}

} // namespace postrisc
