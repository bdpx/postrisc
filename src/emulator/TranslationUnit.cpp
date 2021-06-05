#include "util/common.hpp"
#include "TranslationUnit.hpp"
#include "DumpFormatter.hpp"

namespace postrisc {

void TranslationEntry::dump(const DumpFormatter& out) const
{
    out.dump_physical_address_ref(GetAddress().get_address());

    out << " "
        << ((data & VM_PAGE_FLAG_VALID                 ) ? 'V' : 'v')
        << ((data & VM_PAGE_FLAG_SUPERPAGE             ) ? 'S' : 's')
        << ((data & VM_PAGE_FLAG_ACCESS                ) ? 'A' : 'a')
        << ((data & VM_PAGE_FLAG_DIRTY                 ) ? 'D' : 'd')
        << ((data & VM_PAGE_FLAG_GLOBAL                ) ? 'G' : 'g')
        << ((data & VM_PAGE_FLAG_PRESENTED             ) ? 'P' : 'p')
        ;

    out << " "
        << ((data & VM_PAGE_RIGHT_READ                 ) ? 'R' : 'r')
        << ((data & VM_PAGE_RIGHT_WRITE                ) ? 'W' : 'w')
        << ((data & VM_PAGE_RIGHT_EXECUTE              ) ? 'X' : 'x')
        << ((data & VM_PAGE_RIGHT_BACKING_STORE        ) ? 'B' : 'b')
        << ((data & VM_PAGE_RIGHT_FINAL                ) ? 'F' : 'f')
        << ((data & VM_PAGE_RIGHT_PROMOTE              ) ? 'K' : 'k')
        ;

    out << " "
        << ((data & VM_PAGE_ATTRIBUTE_CACHEABLE        ) ? 'C' : 'c')
        << ((data & VM_PAGE_ATTRIBUTE_WRITE_COALESCING ) ? 'O' : 'o')
        << ((data & VM_PAGE_ATTRIBUTE_WRITE_THROUGH    ) ? 'T' : 't')
        << ((data & VM_PAGE_ATTRIBUTE_RESIDENT         ) ? 'R' : 'r')
        ;
}


/***************************************************************************
*
***************************************************************************/
TranslationUnit::TranslationUnit(void)
{
}

u64 TranslationUnit::GetConfig(void) const
{
    return (m_Valid ? 1U : 0U)
         | (m_NumberOfPagingLevels << 1)
         | (m_BitsPerPageOffset    << 5)
         | m_RootPageAddress.get_address();
}

Result TranslationUnit::SetConfig(u64 value)
{
    m_Valid                = (value >> 0) & 1U;
    m_NumberOfPagingLevels = (value >> 1) & util::makemask<u64>(4);
    m_BitsPerPageOffset    = (value >> 5) & util::makemask<u64>(4);
    m_RootPageAddress      = PhysAddress(value & ~util::makemask<u64>(12));
    return Result::continue_execution;
}

void TranslationUnit::SetPagingParameters(PhysAddress pta, unsigned bitsPerPageOffset, unsigned numberOfPagingLevels)
{
    m_Valid                = true;
    m_RootPageAddress      = pta;
    m_NumberOfPagingLevels = numberOfPagingLevels;
    m_BitsPerPageOffset    = bitsPerPageOffset;
    m_BitsPerPageIndex     = bitsPerPageOffset - log_bytes_per_address;
}

std::ostream& operator<<(std::ostream& os, const TranslationUnit& mmu)
{
    return os
        << fmt::hex(mmu.GetConfig())
        << " v=" << (mmu.m_Valid ? '1' : '0')
        << " pta=" << mmu.GetPageTableRootAddress()
        << " pageoffset=" << mmu.m_BitsPerPageOffset
        << " nlevels=" << mmu.m_NumberOfPagingLevels;
}

bool TranslationUnit::IsSamePage(u64 p1, u64 p2) const
{
    return (p1 >> m_BitsPerPageOffset)
        == (p2 >> m_BitsPerPageOffset);
}

u64 TranslationUnit::GetHighestUserAddress(void) const
{
    return (U64C(1) << (GetVirtualAddressBits()-1)) - 1;
}

u64 TranslationUnit::GetLowestKernelAddress(void) const
{
    return ~GetHighestUserAddress();
}

} // namespace postrisc
