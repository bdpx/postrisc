#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "config/emulator_configuration.hpp"

#include "core.hpp"
#include "utils.hpp"
#include "debugger.hpp"

namespace postrisc {

std::ostream& operator<<(std::ostream& out, const SoftExceptionState& r)
{
    return out << "eip=" << fmt::hex(r.GetEIP())
               << " context=" << fmt::hex(r.GetContext());
}

std::ostream& operator<<(std::ostream& out, const CInstructionBreakpointRegister& r)
{
    return out << fmt::hex(r.address) << '_' << fmt::hex(r.mask);
}

std::ostream& operator<<(std::ostream& out, const CDataBreakpointRegister& r)
{
    return out << fmt::hex(r.address) << '_' << fmt::hex(r.mask);
}

std::ostream& operator<<(std::ostream& out, const CRegisterStackControlRegister& rsc)
{
    return out << fmt::hex(rsc.get())
               << " sof=" << rsc.m_SizeOfFrame
               << " bof=" << rsc.m_BottomOfFrame
               << " soc=" << rsc.m_SizeOfClear
               << " ndirty=" << rsc.m_SizeOfDirty
               << " bod=" << rsc.get_bod();
}

} // namespace postrisc
