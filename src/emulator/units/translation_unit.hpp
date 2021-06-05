#pragma once

#include "arch/interruption.hpp"
#include "arch/instructions.hpp"
#include "emulator/target_system.hpp"
#include "instruction_cache.hpp"
#include "translation_cache.hpp"

namespace postrisc {

enum : phys_address_t {
    // low bits (minimal supported 12-bit page offset)
    VM_PAGE_FLAG_VALID                 = UINT64_C(0x0000000000000001),   // translation is valid, page is reserved
    VM_PAGE_FLAG_SUPERPAGE             = UINT64_C(0x0000000000000002),   // stop walking, super-page found
    VM_PAGE_FLAG_PRESENTED             = UINT64_C(0x0000000000000004),   // page is committed
    VM_PAGE_FLAG_ACCESS                = UINT64_C(0x0000000000000008),   // some access occurs
    VM_PAGE_FLAG_DIRTY                 = UINT64_C(0x0000000000000010),   // writes occurs
    VM_PAGE_FLAG_GLOBAL                = UINT64_C(0x0000000000000020),   // global page (not flushed on context switch)
    // rights
    VM_PAGE_RIGHT_READ                 = UINT64_C(0x0000000000000040),   // can read data
    VM_PAGE_RIGHT_WRITE                = UINT64_C(0x0000000000000080),   // can write data
    VM_PAGE_RIGHT_EXECUTE              = UINT64_C(0x0000000000000100),   // can execute code
    VM_PAGE_RIGHT_BACKING_STORE        = UINT64_C(0x0000000000000200),   // can backstore registers
    VM_PAGE_RIGHT_FINAL                = UINT64_C(0x0000000000000400),   // finalized page (not changed protection)
    VM_PAGE_RIGHT_PROMOTE              = UINT64_C(0x0000000000000800),   // kernel rights promotion

    // high bits (in unimplemented physical address bits)
    VM_PAGE_ATTRIBUTE_CACHEABLE        = UINT64_C(0x1000000000000000),   // enable caching
    VM_PAGE_ATTRIBUTE_WRITE_COALESCING = UINT64_C(0x2000000000000000),   // enable write combining
    VM_PAGE_ATTRIBUTE_WRITE_THROUGH    = UINT64_C(0x4000000000000000),   // write mode
    VM_PAGE_ATTRIBUTE_RESIDENT         = UINT64_C(0x8000000000000000),   // reserved, for future usage

    VM_PAGE_RIGHT_MASK =
          VM_PAGE_RIGHT_READ
        | VM_PAGE_RIGHT_WRITE
        | VM_PAGE_RIGHT_EXECUTE
        | VM_PAGE_RIGHT_BACKING_STORE
        | VM_PAGE_RIGHT_FINAL
        | VM_PAGE_RIGHT_PROMOTE,

    VM_PAGE_RIGHT_NONE = UINT64_C(0x0),

    VM_PAGE_ATTRIBUTES_MASK =
          VM_PAGE_ATTRIBUTE_CACHEABLE
        | VM_PAGE_ATTRIBUTE_WRITE_COALESCING
        | VM_PAGE_ATTRIBUTE_WRITE_THROUGH
        | VM_PAGE_ATTRIBUTE_RESIDENT,

    VM_PAGE_FLAGS_ALL =
          VM_PAGE_FLAG_VALID
        | VM_PAGE_FLAG_SUPERPAGE
        | VM_PAGE_FLAG_ACCESS
        | VM_PAGE_FLAG_DIRTY
        | VM_PAGE_FLAG_GLOBAL
        | VM_PAGE_FLAG_PRESENTED
        | VM_PAGE_RIGHT_MASK
        | VM_PAGE_ATTRIBUTES_MASK,

    VM_PAGE_PHYS_ADDRESS_MASK = PHYS_ADDRESS_MASK & ~VM_PAGE_FLAGS_ALL,  // UINT64_C(0x0fffffffffffc000)
};

using CCoreTranslationCache = TranslationCache<256>;

/////////////////////////////////////////////////////////////////////////////////////////////
// translation entry parse helper
/////////////////////////////////////////////////////////////////////////////////////////////
class CTranslationEntry {
public:
    explicit CTranslationEntry(uint64_t _data) : data(_data) {}

    uint64_t GetAddress(void) const
    {
        const uint64_t address = (data & VM_PAGE_PHYS_ADDRESS_MASK);
        return address;
    }

    bool is_valid(void) const
    {
        return (data & VM_PAGE_FLAG_VALID) != 0;
    }

    void reset()
    {
        data = 0;
    }

    void dump(const DumpFormatter& out) const;

private:
    uint64_t   data;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
class TranslationUnit {
public:
    explicit TranslationUnit(void);

    uint64_t GetConfig(void) const;
    CStatus SetConfig(uint64_t value);

    void SetPagingParameters(phys_address_t pta,
        unsigned bitsPerPageOffset, unsigned numberOfPagingLevels);
    unsigned GetVirtualAddressBits(void) const;
    unsigned GetPageOffsetSize(void) const;
    int GetNumberOfPagingLevels(void) const;
    unsigned GetBitsPerPageIndex(void) const;
    phys_address_t GetPageTableRootAddress(void) const;
    uint64_t compute_canonical_address(uint64_t address) const;
    bool is_canonical(uint64_t address) const;
    bool IsSamePage(uint64_t p1, uint64_t p2) const;
    bool is_real_mode(void) const;
    uint64_t GetPageSize(void) const;
    uint64_t ComputePageOffset(uint64_t address) const;
    uint64_t ComputePageBase(uint64_t address) const;
    uint64_t GetHighestUserAddress(void) const;
    uint64_t GetLowestKernelAddress(void) const;
    static bool IsKernelAddress(uint64_t address);
    phys_address_t get_next_te_pa(uint64_t vaddress, uint64_t paddress, int level) const;

    friend std::ostream& operator<< (std::ostream& os, const TranslationUnit& mmu);

private:
    friend class TranslationUnit_fixture; // security hole for unittests
    unsigned ComputePagingIndex(uint64_t address, unsigned level) const;

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

private:
    phys_address_t      m_RootPageAddress;
    unsigned            m_NumberOfPagingLevels;
    unsigned            m_BitsPerPageOffset;
    unsigned            m_BitsPerPageIndex;
    bool                m_Valid;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// inlines
/////////////////////////////////////////////////////////////////////////////////////////////

ALWAYS_INLINE
bool
TranslationUnit::is_real_mode(void) const
{
    return (m_NumberOfPagingLevels == 0);
}

ALWAYS_INLINE
int
TranslationUnit::GetNumberOfPagingLevels(void) const
{
    return m_NumberOfPagingLevels;
}

ALWAYS_INLINE
unsigned
TranslationUnit::GetBitsPerPageIndex(void) const
{
    return m_BitsPerPageIndex;
}

ALWAYS_INLINE
uint64_t
TranslationUnit::GetPageSize(void) const
{
    return UINT64_C(1) << m_BitsPerPageOffset;
}

ALWAYS_INLINE
unsigned
TranslationUnit::GetPageOffsetSize(void) const
{
    return m_BitsPerPageOffset;
}

ALWAYS_INLINE
uint64_t
TranslationUnit::ComputePageOffset(uint64_t address) const
{
    return address & (GetPageSize() - 1);
}

ALWAYS_INLINE
uint64_t
TranslationUnit::ComputePageBase(uint64_t address) const
{
    const uint64_t mask = ~ static_cast<uint64_t>(GetPageSize() - 1);
    return address & mask;
}

ALWAYS_INLINE
unsigned
TranslationUnit::GetVirtualAddressBits(void) const
{
    return m_BitsPerPageOffset + m_NumberOfPagingLevels * m_BitsPerPageIndex;
}

ALWAYS_INLINE
uint64_t
TranslationUnit::compute_canonical_address(uint64_t address) const
{
    unsigned const shift = 64 - GetVirtualAddressBits();
    uint64_t const canon = static_cast<int64_t>(address << shift) >> shift;
    return canon;
}

ALWAYS_INLINE
bool
TranslationUnit::is_canonical(uint64_t address) const
{
    return (address == compute_canonical_address(address));
}

ALWAYS_INLINE
bool
TranslationUnit::IsKernelAddress(uint64_t address)
{
    return static_cast<int64_t>(address) < 0;
}

ALWAYS_INLINE
phys_address_t
TranslationUnit::GetPageTableRootAddress(void) const
{
    return m_RootPageAddress;
}

ALWAYS_INLINE
unsigned
TranslationUnit::ComputePagingIndex(uint64_t address, unsigned level) const
{
    unsigned const mask  = ((UINT32_C(1) << m_BitsPerPageIndex) - 1);
    unsigned const shift = m_BitsPerPageOffset + level * m_BitsPerPageIndex;

    unsigned const index = (address >> shift) & mask;

    LOG_TRACE(LOG_PAGETABLE)
        << "address=0x" << fmt::hex(address)
        << " level=" << level
        << " index=0x" << fmt::hex(index);

    return index;
}

ALWAYS_INLINE
uint64_t
TranslationUnit::get_next_te_pa(uint64_t virt_address, uint64_t phys_address, int level) const
{
     unsigned const idx = ComputePagingIndex(virt_address, level);

     uint64_t const curpta = (phys_address & VM_PAGE_PHYS_ADDRESS_MASK) + (idx << log_bytes_per_address);

     LOG_TRACE(LOG_PAGETABLE)
         << "curpta=" << fmt::hex(curpta);

     return curpta;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// serialization
/////////////////////////////////////////////////////////////////////////////////////////////

template<class Archive>
void
TranslationUnit::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_RootPageAddress);
    ar & BOOST_SERIALIZATION_NVP(m_NumberOfPagingLevels);
    ar & BOOST_SERIALIZATION_NVP(m_BitsPerPageOffset);
    ar & BOOST_SERIALIZATION_NVP(m_BitsPerPageIndex);
    ar & BOOST_SERIALIZATION_NVP(m_Valid);
}

} // namespace postrisc
