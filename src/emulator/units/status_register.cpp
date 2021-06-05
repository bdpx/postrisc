#include "util/common.hpp"
#include "status_register.hpp"

namespace postrisc {

void
CProcessorStatusRegister::reset(void)
{
    flags = 0;
    future = 0;
    ri = 0;
}

fpu::u128
CProcessorStatusRegister::get(void) const
{
    return fpu::u128(future, 
            (static_cast<uint64_t>(flags)  << 0)
          | (static_cast<uint64_t>(ri)     << 16)
          );
}

void
CProcessorStatusRegister::NullifyFuture(uint64_t mask)
{
    // future nullification
    future |= mask;

    LOG_TRACE(LOG_PREDICATION)
        << "nullification=0b" << std::bitset<16>(mask)
        << " future=0b" << fmt::future(future);
}

void
CProcessorStatusRegister::set(uint64_t value)
{
    const unsigned mask = FLAGS_ALL & ~FLAG_CURRENT_PRIVILEGY_LEVEL;
    flags  = value & mask;

    future = 0;
}

void
CProcessorStatusRegister::PushBackFuture()
{
    // on exception:
    // mark already raised instructions (current shadow) as non-qualified nop for restart

    future = (future << 1) | 0;
}

void
CProcessorStatusRegister::Raise(void)
{
    assert(ri > 0);

    // enable raised instruction for restart
    PushBackFuture();
    --ri;

    LOG_TRACE(LOG_PREDICATION) << "future=0b" << fmt::future(future) << " ri=" << ri;
}

void
CProcessorStatusRegister::Trap(void)
{
    assert(ri > 0);
    // do nothing in case of trap - instruction already finished
    LOG_TRACE(LOG_PREDICATION) << "future=0b" << fmt::future(future) << " ri=" << ri;
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
