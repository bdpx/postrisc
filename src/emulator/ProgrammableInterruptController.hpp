#pragma once

#include <atomic>
#include <mutex>

#include "Result.hpp"

// programmable interrupts controller

namespace postrisc {

/***************************************************************************
* icr
*
* 3210987654321098765432109876543210987654321098765432109876543210
* pppppppppppppppppppppppppppppppp-----------------ddd----vvvvvvvv
*
***************************************************************************/
class InterruptionControlRegister {
public:
static const u64 MASK_VALID_ICR =   (
    (util::makemask<u64>(bits_per_processor_id)  << 32) |
    (util::makemask<u64>(bits_per_int_vector)    << 0) |
    (util::makemask<u64>(bits_per_delivery_mode) << (bits_per_int_vector+4)));

    u8     vector;
    u8     dm;
    u8     h;
    u32    pid;

    friend std::ostream& operator<<(std::ostream& out, const InterruptionControlRegister& icr);

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version));
};

template<typename Archive>
void
InterruptionControlRegister::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(vector);
    ar & BOOST_SERIALIZATION_NVP(dm);
    ar & BOOST_SERIALIZATION_NVP(h);
    ar & BOOST_SERIALIZATION_NVP(pid);
}

class event_redirector {
public:
    event_redirector(void) : vector(0), mask(1)
    {}

    u64 get(void) const
    {
        return static_cast<u64>(vector) | (static_cast<u64>(mask) << 15);
    }

    void Set(u64 value)
    {
        vector = value & 255;
        mask = (value >> 15) & 1;
    }

    void reset()
    {
        mask = 1;
        vector = 0;
    }

    friend std::ostream& operator<<(std::ostream& out, const event_redirector& rr);

private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(vector);
        ar & BOOST_SERIALIZATION_NVP(mask);
    }

private:
    u8 vector;
    u8 mask;
};

class local_pin_redirector {
public:
    local_pin_redirector(void) : vector(0), mask(1) {}
    u64 get(void) const
    {
        return static_cast<u64>(vector) | (static_cast<u64>(mask) << 15);
    }
    void set(u64 value)
    {
        vector = value & 255;
        mask = (value >> 15) & 1;
    }

    void reset()
    {
        mask = 1;
        vector = 0;
    }

    friend std::ostream& operator<<(std::ostream& out, const local_pin_redirector& rr);

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

private:
    u8 vector;
    u8 mask;
    u8 trigger_mode;
    u8 delivery_mode;
    u8 pin_polarity;
};

template<typename Archive>
void
local_pin_redirector::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(vector);
    ar & BOOST_SERIALIZATION_NVP(mask);
    ar & BOOST_SERIALIZATION_NVP(trigger_mode);
    ar & BOOST_SERIALIZATION_NVP(delivery_mode);
    ar & BOOST_SERIALIZATION_NVP(pin_polarity);
}

class ProgrammableInterruptController {
friend class Core;
public:
    void reset(unsigned coreid);
    Result post_interrupt(Result vector_index);
    unsigned read_vector();
    void EndOfInterrupt(u64 value);
    bool IsInterruptRequest(void) const { return m_IsInterruptRequest; }

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

private:
    static const unsigned map_size = (1 << bits_per_int_vector) / 64;

private:
    event_redirector                     itcv;
    event_redirector                     tsv;
    event_redirector                     pmv;
    event_redirector                     cmcv;
    //std::array<local_pin_redirector, 2>  lrr;
    std::array<std::atomic<u64>, 4>      irr; // 256 bit interrupt requests registers (pending interrupts)
                                              // possible writes from different threads
    std::array<u64, 4>                   isr; // 256 bit interrupt serviced registers (acknowledged interrupts)
    unsigned                             lid; // processor identification
    u8                                   tpr; // begin interrupt request, task priority
    bool                                 m_IsInterruptRequest;
    std::mutex                           lock;
};

template<typename Archive>
void ProgrammableInterruptController::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(tsv);
    ar & BOOST_SERIALIZATION_NVP(pmv);
    ar & BOOST_SERIALIZATION_NVP(cmcv);
    // ar & BOOST_SERIALIZATION_NVP(lrr);
    ar & BOOST_SERIALIZATION_NVP(irr);
    ar & BOOST_SERIALIZATION_NVP(isr);
    ar & BOOST_SERIALIZATION_NVP(lid);
    ar & BOOST_SERIALIZATION_NVP(tpr);
    ar & BOOST_SERIALIZATION_NVP(m_IsInterruptRequest);
}

} // namespace postrisc
