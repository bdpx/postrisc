#include "util/common.hpp"
#include "ProgrammableInterruptController.hpp"

namespace postrisc {

std::ostream& operator<<(std::ostream& out, const InterruptionControlRegister& icr)
{
    return out << fmt::hex(U64C(0))
               << " pid=" << icr.pid
               << " dm=" << icr.dm
               << " h=" << icr.h
               << " vector=" << icr.vector;
}

std::ostream& operator<<(std::ostream& out, const event_redirector& rr)
{
    return out << fmt::hex(rr.get())
               << " vector=" << +rr.vector
               << " mask=" << +rr.mask;
}

std::ostream& operator<<(std::ostream& out, const local_pin_redirector& rr)
{
    return out << fmt::hex(rr.get())
               << " vector=" << +rr.vector
               << " mask=" << +rr.mask;
}

void ProgrammableInterruptController::reset(unsigned coreid)
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

Result ProgrammableInterruptController::post_interrupt(Result event)
{
    std::lock_guard<std::mutex> guard(lock);

    const unsigned vector = event.GetVector();
    const unsigned index = (vector & 255) / 64;
    const unsigned offset = vector & 63;

    LOG_DEBUG(LOG_EMULATOR) << "index=" << index << " offset=" << offset;
    irr[index] |= U64C(1) << offset;
    m_IsInterruptRequest = true;
    return Result::continue_execution;
}

unsigned ProgrammableInterruptController::read_vector()
{
    std::lock_guard<std::mutex> guard(lock);

    // search from highest priority
    for (int i = 3; i >= 0; --i) {
        if (irr[i] == 0) continue;

        unsigned const bitpos = util::count_trailing_zeros::func(irr[i]);
        LOG_DEBUG(LOG_EMULATOR) << "index=" << i
                                << " irr=" << std::bitset<64>(irr[i])
                                << " offset=" << bitpos;

        const u64 flag = U64C(1) << bitpos;

        irr[i] &= ~flag; // clear irr
        isr[i] |= flag;  // set isr

        if (   irr[0] == U64C(0)
            && irr[1] == U64C(0)
            && irr[2] == U64C(0)
            && irr[3] == U64C(0)
           ) {
            m_IsInterruptRequest = false;
        }

        unsigned const vector = i*64 + bitpos;
        return vector;
    }

    // spurious vector
    LOG_DEBUG(LOG_EMULATOR) << "spurious vector 0";
    return 0;
}

void ProgrammableInterruptController::EndOfInterrupt(u64 vector)
{
    std::lock_guard<std::mutex> guard(lock);

    const unsigned index = (vector & 255) / 64;
    const unsigned offset = vector & 63;

    isr[index] &= ~(U64C(1) << offset);
}

} // namespace postrisc
