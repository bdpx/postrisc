#pragma once

#include "arch/Bundle.hpp"
#include "Result.hpp"
#include "PerformanceMonitor.hpp"
#include "AddressSpace.hpp"
#include "TranslationHelpers.hpp"

namespace postrisc {

/***************************************************************************
* translation cache (TLB) entry
***************************************************************************/
class TranslationCacheEntry {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);
public:
    TranslationCacheEntry() : virt_address(invalid_tag), phys_address(0) {}
    void reset() { virt_address = invalid_tag; }

public:
    static const u64 invalid_tag = ~U64C(0);

public:
    u64 virt_address;
    u64 phys_address;
};

template<typename Archive>
void
TranslationCacheEntry::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(virt_address);
    ar & BOOST_SERIALIZATION_NVP(phys_address);
}

/***************************************************************************
* translation cache (TLB)
***************************************************************************/
template<size_t SIZE> class TranslationCache {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);
public:
    TranslationCache();

    ALWAYS_INLINE TranslationCacheEntry& Find(u64 vpn) {
       return cache[static_cast<size_t>(vpn % cache.size())];
    }
    void Flush();
    void Flush(u64 virt_address, unsigned bitsPerPageOffset);

private:
    std::array<TranslationCacheEntry, SIZE>     cache;
};

template<size_t SIZE>
template<typename Archive>
void
TranslationCache<SIZE>::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(cache);
}

template<size_t SIZE>
TranslationCache<SIZE>::TranslationCache()
{
    Flush();
}

template<size_t SIZE>
void
TranslationCache<SIZE>::Flush()
{
    for (auto& x : cache) {
        x.reset();
    }
}

template<size_t SIZE>
void
TranslationCache<SIZE>::Flush(u64 virt_address, unsigned bitsPerPageOffset)
{
    u64 const vpn = virt_address >> bitsPerPageOffset;
    TranslationCacheEntry& tce = Find(vpn);

    if (tce.virt_address == vpn) {
        LOG_DEBUG(LOG_PAGETABLE) << "TLB flush: virt_address=" << fmt::hex(virt_address)
                                 << " phys_address=" << fmt::hex(tce.phys_address);
        tce.reset();
    }
}

using CCoreTranslationCache = TranslationCache<256>;

/***************************************************************************
* translation entry in memory
***************************************************************************/
class TranslationEntry {
public:
    explicit TranslationEntry(u64 _data) : data(_data) {}
    explicit TranslationEntry(PhysAddress pa) : data(pa.get_address()) {}
    ALWAYS_INLINE PhysAddress GetAddress(void) const {
        const u64 address = (data & VM_PAGE_PHYS_ADDRESS_MASK);
        return PhysAddress(address);
    }
    ALWAYS_INLINE bool is_valid(void) const {
        return (data & VM_PAGE_FLAG_VALID) != 0;
    }
    ALWAYS_INLINE void reset() {
        data = 0;
    }
    void dump(const DumpFormatter& out) const;

private:
    u64   data;
};

/***************************************************************************
*
***************************************************************************/
class TranslationUnit {
public:
    explicit TranslationUnit(void);

    u64 GetConfig(void) const;
    Result SetConfig(u64 value);

    void SetPagingParameters(PhysAddress pta,
        unsigned bitsPerPageOffset, unsigned numberOfPagingLevels);
    unsigned GetVirtualAddressBits(void) const;
    unsigned GetPageOffsetSize(void) const;
    int GetNumberOfPagingLevels(void) const;
    unsigned GetBitsPerPageIndex(void) const;
    PhysAddress GetPageTableRootAddress(void) const;
    u64 compute_canonical_address(u64 address) const;
    bool is_canonical(u64 address) const;
    bool IsSamePage(u64 p1, u64 p2) const;
    bool is_real_mode(void) const;
    u64 GetPageSize(void) const;
    u64 ComputePageOffset(u64 address) const;
    u64 ComputePageBase(u64 address) const;
    u64 GetHighestUserAddress(void) const;
    u64 GetLowestKernelAddress(void) const;
    static bool IsKernelAddress(u64 address);
    PhysAddress get_next_te_pa(u64 vaddress, PhysAddress pa, int level) const;

    void FlushTranslationBuffer(void) {
        m_DataTranslationCache.Flush();
        m_CodeTranslationCache.Flush();
    }
    void FlushTranslationBuffer(u64 virt_address) {
        m_DataTranslationCache.Flush(virt_address, GetPageOffsetSize());
        m_CodeTranslationCache.Flush(virt_address, GetPageOffsetSize());
    }

    friend std::ostream& operator<< (std::ostream& os, const TranslationUnit& mmu);

    template<typename helper>
    std::tuple<Result, PhysAddress>
    TranslateAddress(const AddressSpace &as, PerformanceMonitor &performance_monitor,
                     const VirtAddress va, u64 requested_rights, u64 unexpected_rights);

private:
    friend class TranslationUnit_fixture; // security hole for unittests
    unsigned ComputePagingIndex(u64 address, unsigned level) const;

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

    // interface to obtain access to specialised translation cache
    // (code or data) depend on helper type (code/data/backstore)
    template<typename T> CCoreTranslationCache& ChooseTranslationCache(void);

private:
    PhysAddress              m_RootPageAddress = PhysAddress(0);
    unsigned                  m_NumberOfPagingLevels = 3;
    unsigned                  m_BitsPerPageOffset = 13;
    unsigned                  m_BitsPerPageIndex = 13 - log_bytes_per_address;
    bool                      m_Valid = false;
    CCoreTranslationCache     m_DataTranslationCache;
    CCoreTranslationCache     m_CodeTranslationCache;
};

/***************************************************************************
* inlines
***************************************************************************/

template<> ALWAYS_INLINE CCoreTranslationCache& TranslationUnit::ChooseTranslationCache<InstructionHelper>(void) { return m_CodeTranslationCache; }
template<> ALWAYS_INLINE CCoreTranslationCache& TranslationUnit::ChooseTranslationCache<DataHelper>(void) { return m_DataTranslationCache; }
template<> ALWAYS_INLINE CCoreTranslationCache& TranslationUnit::ChooseTranslationCache<BackstoreHelper>(void) { return m_DataTranslationCache; }

ALWAYS_INLINE bool TranslationUnit::is_real_mode(void) const
{
    return (m_NumberOfPagingLevels == 0);
}

ALWAYS_INLINE int TranslationUnit::GetNumberOfPagingLevels(void) const
{
    return m_NumberOfPagingLevels;
}

ALWAYS_INLINE unsigned TranslationUnit::GetBitsPerPageIndex(void) const
{
    return m_BitsPerPageIndex;
}

ALWAYS_INLINE u64 TranslationUnit::GetPageSize(void) const
{
    return U64C(1) << m_BitsPerPageOffset;
}

ALWAYS_INLINE unsigned TranslationUnit::GetPageOffsetSize(void) const
{
    return m_BitsPerPageOffset;
}

ALWAYS_INLINE u64 TranslationUnit::ComputePageOffset(u64 address) const
{
    return address & (GetPageSize() - 1);
}

ALWAYS_INLINE u64 TranslationUnit::ComputePageBase(u64 address) const
{
    const u64 mask = ~ static_cast<u64>(GetPageSize() - 1);
    return address & mask;
}

ALWAYS_INLINE unsigned TranslationUnit::GetVirtualAddressBits(void) const
{
    return m_BitsPerPageOffset + m_NumberOfPagingLevels * m_BitsPerPageIndex;
}

ALWAYS_INLINE u64 TranslationUnit::compute_canonical_address(u64 address) const
{
    unsigned const shift = 64 - GetVirtualAddressBits();
    u64 const canon = static_cast<i64>(address << shift) >> shift;
    return canon;
}

ALWAYS_INLINE bool TranslationUnit::is_canonical(u64 address) const
{
    return (address == compute_canonical_address(address));
}

ALWAYS_INLINE bool TranslationUnit::IsKernelAddress(u64 address)
{
    return static_cast<i64>(address) < 0;
}

ALWAYS_INLINE PhysAddress TranslationUnit::GetPageTableRootAddress(void) const
{
    return m_RootPageAddress;
}

ALWAYS_INLINE unsigned TranslationUnit::ComputePagingIndex(u64 address, unsigned level) const
{
    unsigned const mask  = ((U32C(1) << m_BitsPerPageIndex) - 1);
    unsigned const shift = m_BitsPerPageOffset + level * m_BitsPerPageIndex;

    unsigned const index = (address >> shift) & mask;

    LOG_DEBUG(LOG_PAGETABLE)
        << "address=0x" << fmt::hex(address)
        << " level=" << level
        << " index=0x" << fmt::hex(index);

    return index;
}

ALWAYS_INLINE PhysAddress
TranslationUnit::get_next_te_pa(u64 virt_address, PhysAddress pa, int level) const
{
     unsigned const idx = ComputePagingIndex(virt_address, level);

     u64 const curpta = (pa.get_address() & VM_PAGE_PHYS_ADDRESS_MASK) + (idx << log_bytes_per_address);

     LOG_DEBUG(LOG_PAGETABLE)
         << "curpta=" << fmt::hex(curpta);

     return PhysAddress(curpta);
}

template<typename helper>
ALWAYS_INLINE
std::tuple<Result, PhysAddress>
TranslationUnit::TranslateAddress(const AddressSpace& as, PerformanceMonitor &performance_monitor,
    const VirtAddress va, u64 requested_rights, u64 unexpected_rights)
{
    if (unlikely(is_real_mode())) {
        LOG_DEBUG(LOG_PAGETABLE) << "va==pa: " << va;
        return { Result::continue_execution, PhysAddress(va.get_address()) };
    }

    LOG_DEBUG(LOG_PAGETABLE)
        << "va=" << va
        << " rights=" << fmt::hex(requested_rights);

    if (unlikely(!is_canonical(va.get_address()))) {
        LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
            << "noncanonical virtual address=" << va;

        return { helper::invalid_address(), PhysAddress(0) };
    }

    CCoreTranslationCache& tc = ChooseTranslationCache<helper>();

    u64 const virt_address = va.get_address();
    // u64 ptx[numberOfPagingLevels];
    u64 phys_address;
    u64 const vpn = virt_address >> GetPageOffsetSize();
    TranslationCacheEntry& tce = tc.Find(vpn);

    if (likely(tce.virt_address == vpn)) {
        performance_monitor.report_event(helper::translation_hit);
        phys_address = tce.phys_address;

        LOG_DEBUG(LOG_PAGETABLE)
            << "TLB hit: va=" << fmt::hex(virt_address)
            << " pa=" << fmt::hex(phys_address);
    } else {
        LOG_DEBUG(LOG_PAGETABLE | LOG_EXCEPTION) << "TLB miss: va=" << fmt::hex(virt_address);
        performance_monitor.report_event(helper::translation_miss);
        // walk page table
        phys_address = GetPageTableRootAddress().get_address();
        for (int level = GetNumberOfPagingLevels() - 1; level >= 0; --level) {
            PhysAddress const next_te_pa = get_next_te_pa(virt_address, PhysAddress(phys_address), level);
            u64 next_pta;
            Result result = as.bus_read(next_te_pa, sizeof(next_pta), &next_pta);
            if (unlikely(!result.success())) return { result, PhysAddress(0) };

            if (unlikely(!TranslationEntry(next_pta).is_valid())) {
                LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                    << "invalid pta: level=" << level
                    << " va=" << fmt::hex(virt_address)
                    << " pta=" << fmt::hex(next_pta);

                return { helper::page_not_present(), PhysAddress(0) };
            }
            phys_address = next_pta;
        }

        LOG_DEBUG(LOG_PAGETABLE)
            << "walk pt: va=" << fmt::hex(virt_address)
            << " pa=" << fmt::hex(phys_address);

        tce.virt_address = vpn;
        tce.phys_address = phys_address;
    }

    // check access
    if (unlikely((phys_address & (requested_rights | unexpected_rights)) != requested_rights)) {
        LOG_DEBUG(LOG_PAGETABLE | LOG_EXCEPTION)
                  << "requested rights error: virt_address=" << fmt::hex(virt_address)
                  << " requested_rights=" << fmt::hex(requested_rights)
                  << " unexpected_rights=" << fmt::hex(unexpected_rights)
                  << " current_rights=" << fmt::hex(phys_address & VM_PAGE_RIGHT_MASK);

        return { helper::access_right(), PhysAddress(0) };
    }

    u64 const page_offset = ComputePageOffset(virt_address);

    // reconstruct
    // u64 raddress = page_offset;

    //for(int i=GetNumberOfPagingLevels() - 1; i >= 0; --i) {
    //   raddress |= u64(idx[i]) << (GetPageOffsetSize() + i * GetBitsPerPageIndex());
    //}

    //LOG_DEBUG(LOG_PAGETABLE) << "reconstruction=" << fmt::hex(raddress)
    //          << " " << fmt::hex(compute_canonical_address(raddress)) << std::endl;

    phys_address = (phys_address & VM_PAGE_PHYS_ADDRESS_MASK) | page_offset;

    LOG_DEBUG(LOG_PAGETABLE)
        << "page_offset=" << fmt::hex(page_offset)
        << " result=" << fmt::hex(phys_address);

    return { Result::continue_execution, PhysAddress(phys_address) };
}

/***************************************************************************
* serialization
***************************************************************************/
template<typename Archive> void
TranslationUnit::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_RootPageAddress);
    ar & BOOST_SERIALIZATION_NVP(m_NumberOfPagingLevels);
    ar & BOOST_SERIALIZATION_NVP(m_BitsPerPageOffset);
    ar & BOOST_SERIALIZATION_NVP(m_BitsPerPageIndex);
    ar & BOOST_SERIALIZATION_NVP(m_Valid);
    ar & BOOST_SERIALIZATION_NVP(m_DataTranslationCache);
    ar & BOOST_SERIALIZATION_NVP(m_CodeTranslationCache);
}

} // namespace postrisc
