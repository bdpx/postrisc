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

u128
ProcessorStatusRegister::get(void) const
{
    return (
         (static_cast<u128>(flags)         <<  0)
       | (static_cast<u128>(rs.get())      << 16)
       | (static_cast<u128>(future.get())  << 32)
    );
}

void
ProcessorStatusRegister::set(u128 value)
{
    const u32 mask = FLAGS_ALL & ~FLAG_CURRENT_PRIVILEGY_LEVEL;
    flags  = value & mask;
    future.set(0);
}

std::ostream& ProcessorStatusRegister::dump(std::ostream& out) const
{
    const u128 value = get();

    out << fmt::hex(static_cast<u64>(value >> 64))
        << '_'
        << fmt::hex(static_cast<u64>(value))
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

    return out;
}

} // namespace postrisc
