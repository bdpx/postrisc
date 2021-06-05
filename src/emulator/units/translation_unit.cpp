#include "util/common.hpp"
#include "target_system.hpp"
#include "translation_unit.hpp"

namespace postrisc {

void
CTranslationEntry::dump(const DumpFormatter& out) const
{
    out.dump_physical_address_ref(GetAddress());

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


/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
TranslationUnit::TranslationUnit(void)
    : m_RootPageAddress(0)
    , m_NumberOfPagingLevels(3)
    , m_BitsPerPageOffset(13)
    , m_BitsPerPageIndex(13 - log_bytes_per_address)
    , m_Valid(false)
{
}

uint64_t
TranslationUnit::GetConfig(void) const
{
    return (m_Valid ? 1U : 0U)
         | (m_NumberOfPagingLevels << 1)
         | (m_BitsPerPageOffset    << 5)
         | m_RootPageAddress;
}

CStatus
TranslationUnit::SetConfig(uint64_t value)
{
    m_Valid                = (value >> 0) & 1U;
    m_NumberOfPagingLevels = (value >> 1) & util::makemask(4);
    m_BitsPerPageOffset    = (value >> 5) & util::makemask(4);
    m_RootPageAddress      = value & ~util::makemask(12);
    return CStatus(CStatus::continue_execution);
}

void
TranslationUnit::SetPagingParameters(phys_address_t pta,
    unsigned bitsPerPageOffset, unsigned numberOfPagingLevels)
{
    m_Valid                = true;
    m_RootPageAddress      = pta;
    m_NumberOfPagingLevels = numberOfPagingLevels;
    m_BitsPerPageOffset    = bitsPerPageOffset;
    m_BitsPerPageIndex     = bitsPerPageOffset - log_bytes_per_address;
}

std::ostream &
operator << (std::ostream & os, const TranslationUnit & mmu)
{
    return os
        << fmt::hex(mmu.GetConfig())
        << " v=" << (mmu.m_Valid ? '1' : '0')
        << " pta=" << fmt::hex(mmu.GetPageTableRootAddress())
        << " pageoffset=" << mmu.m_BitsPerPageOffset
        << " nlevels=" << mmu.m_NumberOfPagingLevels;
}

bool
TranslationUnit::IsSamePage(uint64_t p1, uint64_t p2) const
{
    return (p1 >> m_BitsPerPageOffset)
        == (p2 >> m_BitsPerPageOffset);
}

uint64_t
TranslationUnit::GetHighestUserAddress(void) const
{
    return (UINT64_C(1) << (GetVirtualAddressBits()-1)) - 1;
}

uint64_t
TranslationUnit::GetLowestKernelAddress(void) const
{
    return ~GetHighestUserAddress();
}

} // namespace postrisc
