#include "util/common.hpp"
#include "pic.hpp"

namespace postrisc {

std::ostream &
operator << (std::ostream& out, const CInterruptionControlRegister & icr)
{
    return out << fmt::hex(UINT64_C(0))
               << " pid=" << icr.pid
               << " dm=" << icr.dm
               << " h=" << icr.h
               << " vector=" << icr.vector;
}

std::ostream &
operator << (std::ostream& out, const event_redirector& rr)
{
    return out << fmt::hex(rr.get())
               << " vector=" << +rr.vector
               << " mask=" << +rr.mask;
}

std::ostream &
operator << (std::ostream& out, const local_pin_redirector& rr)
{
    return out << fmt::hex(rr.get())
               << " vector=" << +rr.vector
               << " mask=" << +rr.mask;
}

void
pic_t::reset(unsigned coreid)
{
    std::lock_guard<std::mutex> guard(lock);

    irr[0] = irr[1] = irr[2] = irr[3] = 0;
    isr[0] = isr[1] = isr[2] = isr[3] = 0;

    itcv.reset();
    tsv.reset();
    pmv.reset();
    cmcv.reset();
    //for (auto& l : lrr) {
    //    l.reset();
    //}

    tpr = 255;
    lid = coreid;

    m_IsInterruptRequest = false;
}

CStatus
pic_t::post_interrupt(CStatus event)
{
    std::lock_guard<std::mutex> guard(lock);

    const unsigned vector = event.GetVector();
    const unsigned index = (vector & 255) / 64;
    const unsigned offset = vector & 63;

    LOG_TRACE(LOG_EMULATOR) << "index=" << index << " offset=" << offset;
    irr[index] |= UINT64_C(1) << offset;
    m_IsInterruptRequest = true;
    return CStatus(CStatus::continue_execution);
}

unsigned
pic_t::read_vector()
{
    std::lock_guard<std::mutex> guard(lock);

    // search from highest priority
    for (int i = 3; i >= 0; --i) {
        if (irr[i] == 0) continue;

        unsigned const bitpos = util::count_trailing_zeros::func(irr[i]);
        LOG_TRACE(LOG_EMULATOR) << "index=" << i
                                << " irr=" << std::bitset<64>(irr[i])
                                << " offset=" << bitpos;

        const uint64_t flag = UINT64_C(1) << bitpos;

        irr[i] &= ~flag; // clear irr
        isr[i] |= flag;  // set isr

        if (   irr[0] == UINT64_C(0)
            && irr[1] == UINT64_C(0)
            && irr[2] == UINT64_C(0)
            && irr[3] == UINT64_C(0)
           ) {
            m_IsInterruptRequest = false;
        }

        unsigned const vector = i*64 + bitpos;
        return vector;
    }

    // spurious vector
    LOG_TRACE(LOG_EMULATOR) << "spurious vector 0";
    return 0;
}

void
pic_t::EndOfInterrupt(uint64_t vector)
{
    std::lock_guard<std::mutex> guard(lock);

    const unsigned index = (vector & 255) / 64;
    const unsigned offset = vector & 63;

    isr[index] &= ~(UINT64_C(1) << offset);
}

} // namespace postrisc
