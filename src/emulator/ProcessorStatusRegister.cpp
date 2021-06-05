#include "util/common.hpp"
#include "ProcessorStatusRegister.hpp"

namespace postrisc {

void
ProcessorStatusRegister::reset(void)
{
    flags = 0;
    future.reset();
    rs.reset();
}

u64
ProcessorStatusRegister::get(void) const
{
    return (
         (static_cast<u64>(flags)         <<  0)
       | (static_cast<u64>(rs.get())      << 16)
       | (static_cast<u64>(future.get())  << 32)
    );
}

void
ProcessorStatusRegister::set(u64 value)
{
    const unsigned mask = FLAGS_ALL & ~FLAG_CURRENT_PRIVILEGY_LEVEL;
    flags  = value & mask;
    future.set(0);
}

std::ostream& ProcessorStatusRegister::dump(std::ostream& out) const
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
     << " rs=" << rs
     << " future=" << future;
}

} // namespace postrisc
