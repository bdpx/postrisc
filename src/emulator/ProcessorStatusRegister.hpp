#pragma once

#include "GeneralRegister.hpp"

namespace postrisc {

// future nullification mask
class Future {
public:
    friend std::ostream& operator<<(std::ostream& out, const Future& r) {
        return out << std::bitset<32>(r.m_future);
    }

    // on each return we restore saved future predication
    void set(u32 value) { m_future = value; }

    // on each branch/jump/call we clear future predication
    void clear(void) { set(0); }

    u32 get(void) const { return m_future; }

    void nullify(u32 mask) {
        m_future |= mask;
        LOG_DEBUG(LOG_PREDICATION)
            << "nullification=0b" << std::bitset<32>(mask)
            << " future=0b" << std::bitset<32>(m_future);
    }

    bool pop(void) {
        // get least significant bit
        bool const is_nullified = m_future & 1;
        // drop least significant bit, add zero from end
        m_future >>= 1;

        LOG_DEBUG(LOG_PREDICATION)
            << "pop=" << is_nullified
            << " future=0b" << std::bitset<32>(m_future);

        return is_nullified;
    }

    void reset(void) { clear(); }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)) {
        ar & BOOST_SERIALIZATION_NVP(m_future);
    }

private:
    u32 m_future = 0;
};


// restarting instruction slot
class RestartingSlot {
public:
    friend std::ostream& operator<<(std::ostream& out, const RestartingSlot& r) {
        return out << r.m_rs;
    }

    u32 get(void) const { return m_rs; }

    void set(u32 value) { m_rs = value; }

    void clear(void) { set(0); }

    void reset(void) { clear(); }

    void extend(unsigned size) {
        assert(size == 1 || size == 2);
        assert(m_rs < slots_per_bundle);
        m_rs += size;
        assert(m_rs <= slots_per_bundle);
    }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)) {
        ar & BOOST_SERIALIZATION_NVP(m_rs);
    }

private:
    u32 m_rs = 0;
};

/***************************************************************************
* psr, ipsr
***************************************************************************/
class ProcessorStatusRegister {
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
    u64 get(void) const;
    void set(u64 value);

    bool IsKernelMode(void) const { return (flags & FLAG_CURRENT_PRIVILEGY_LEVEL) == 0; }
    void set_kernel_mode(void) { flags &= ~FLAG_CURRENT_PRIVILEGY_LEVEL; }

    bool IsInterruptDisabled(void) const { return (flags & FLAG_INTERRUPTION) != 0; }

    bool is_interrupt_collection_enabled(void) const { return (flags & FLAG_INTERRUPT_COLLECTION) != 0; }
    void disable_interrupt_collection(void) { flags &= ~FLAG_INTERRUPT_COLLECTION; };

    std::ostream& dump(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const ProcessorStatusRegister& r) {
        return r.dump(out);
    }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

public:
    Future          future;
    RestartingSlot  rs;

private:
    unsigned    flags = 0;             // one-bit flags: start in kernel mode
};

template<typename Archive>
void ProcessorStatusRegister::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(flags);
    ar & BOOST_SERIALIZATION_NVP(future);
    ar & BOOST_SERIALIZATION_NVP(rs);
}

} // namespace postrisc
