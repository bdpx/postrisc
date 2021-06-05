#include "util/common.hpp"
#include "status_register.hpp"

namespace postrisc {

void
CProcessorStatusRegister::reset(void)
{
    flags = 0;
    future = 0;
    ri = 0;
    isLastInstructionInBundle = false;
}

u64
CProcessorStatusRegister::get(void) const
{
    return (
         (static_cast<u64>(flags)  << 0)
       | (static_cast<u64>(ri)     << 16)
       | (static_cast<u64>(future) << 32)
    );
}

void
CProcessorStatusRegister::set(u64 value)
{
    const unsigned mask = FLAGS_ALL & ~FLAG_CURRENT_PRIVILEGY_LEVEL;
    flags  = value & mask;
    future = 0;
}

void
CProcessorStatusRegister::Raise(void)
{
    assert(ri > 0);

    // enable raised instruction for restart
    future = (future << 1) | 0;
    --ri;
    // mark already commited instructions as nullified
    while (ri > 0) {
        future = (future << 1) | 1U;
        --ri;
    }
    LOG_DEBUG(LOG_PREDICATION) << " future=" << fmt::future(future);
}

void
CProcessorStatusRegister::Trap(void)
{
    assert(ri > 0);

    // enable instructions after trapped for restart
    // mark already commited instructions as nullified
    while (ri > 0) {
        future = (future << 1) | 1U;
        --ri;
    }
    LOG_DEBUG(LOG_PREDICATION) << " future=" << fmt::future(future);
}

std::ostream & operator << (std::ostream & out, const CProcessorStatusRegister & r)
{
    return r.dump(out);
}

std::ostream &
CProcessorStatusRegister::dump(std::ostream & out) const
{
    return out
     << fmt::hex(get())
     << " pm=" << (flags & FLAG_PERFORMANCE_MONITOR            ? 1 : 0)
     << " id=" << (flags & FLAG_INSTRUCTION_DEBUG_TRAP         ? 1 : 0)
     << " dd=" << (flags & FLAG_DATA_DEBUG_TRAP                ? 1 : 0)
     << " lp=" << (flags & FLAG_LOVER_PRIVILEGED_TRAP          ? 1 : 0)
     << " tb=" << (flags & FLAG_TAKEN_BRANCH_TRAP              ? 1 : 0)
     << " ss=" << (flags & FLAG_SINGLE_STEP_TRAP               ? 1 : 0)
     << " ic=" << (flags & FLAG_INTERRUPT_COLLECTION           ? 1 : 0)
     << " ib=" << (flags & FLAG_INTERRUPTION                   ? 1 : 0)
     << " pl=" << (flags & FLAG_CURRENT_PRIVILEGY_LEVEL        ? 1 : 0)
     << " pp=" << (flags & FLAG_PRIVILEGED_PERFORMANCE_MONITOR ? 1 : 0)
     << " ri=" << ri
     << " future=" << fmt::future(future);
}

} // namespace postrisc
