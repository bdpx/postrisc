#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "assembler/Assembler.hpp"

namespace postrisc {

class TranslationUnit_fixture {
public:
    TranslationUnit_fixture() {}
    void test();

public:
    void TestIndexing(u64 translated_address, const unsigned *PagingIndexes) const;
    void TestCanonical(u64 address, bool is_canonical) const;

private:
    TranslationUnit mmu;
};

void
TranslationUnit_fixture::TestCanonical(u64 address, bool is_canonical) const
{
    BOOST_TEST(mmu.is_canonical(address) == is_canonical,
        mmu << " is_canonical: " << is_canonical
            << " vs " << mmu.is_canonical(address)
            << " address=" << fmt::hex(address)
        );
}

void
TranslationUnit_fixture::TestIndexing(u64 translated_address, const unsigned *PagingIndexes) const
{
    for (int level = mmu.m_NumberOfPagingLevels - 1; level >= 0; --level) {
        const unsigned index = mmu.ComputePagingIndex(translated_address, level);
        BOOST_TEST(index == PagingIndexes[level],
            mmu << " index=" << index
                << " expected=" << PagingIndexes[level]);
    }
}

static constexpr unsigned GetPageIndex(u64 address, unsigned bitsPerPageOffset, unsigned level)
{
    const unsigned bitsPerAddress = 3; // log2(8byte)
    const unsigned bitsPerPageIndex = bitsPerPageOffset - bitsPerAddress;
    const unsigned shift = bitsPerPageOffset + level * bitsPerPageIndex;
    return (address >> shift) & util::makemask<u64>(bitsPerPageIndex);
}

void
TranslationUnit_fixture::test()
{
    const u64 addr = U64C(0x9876543223456789);

    static const unsigned PageIndexes_12[] = {
        GetPageIndex(addr, 12, 0),
        GetPageIndex(addr, 12, 1),
        GetPageIndex(addr, 12, 2),
        GetPageIndex(addr, 12, 3),
        GetPageIndex(addr, 12, 4),
    };
    static const unsigned PageIndexes_13[] = {
        GetPageIndex(addr, 13, 0),
        GetPageIndex(addr, 13, 1),
        GetPageIndex(addr, 13, 2),
        GetPageIndex(addr, 13, 3),
    };
    static const unsigned PageIndexes_14[] = {
        GetPageIndex(addr, 14, 0),
        GetPageIndex(addr, 14, 1),
        GetPageIndex(addr, 14, 2),
        GetPageIndex(addr, 14, 3),
    };
    static const unsigned PageIndexes_15[] = {
        GetPageIndex(addr, 15, 0),
        GetPageIndex(addr, 15, 1),
        GetPageIndex(addr, 15, 2),
        GetPageIndex(addr, 15, 3),
    };
    static const unsigned PageIndexes_16[] = {
       GetPageIndex(addr, 16, 0),
       GetPageIndex(addr, 16, 1),
       GetPageIndex(addr, 16, 2),
    };

    const unsigned bitsPerAddress = 3; // log2(8byte)
    const unsigned bitsPerPageTableIndex = 0; // user/kernel page table

    static const struct {
        unsigned        BitsPerPageOffset;
        unsigned        NumberOfPagingLevels;
        unsigned const *PagingIndexes;
    } configs[] = {
        { 12,  2,  PageIndexes_12 },
        { 12,  3,  PageIndexes_12 },
        { 12,  4,  PageIndexes_12 }, // like X64
        { 12,  5,  PageIndexes_12 }, // like X64 planned extension
        { 13,  2,  PageIndexes_13 },
        { 13,  3,  PageIndexes_13 }, // like 8KiB IA64
        { 13,  4,  PageIndexes_13 },
        { 14,  2,  PageIndexes_14 },
        { 14,  3,  PageIndexes_14 },
        { 14,  4,  PageIndexes_14 },
        { 15,  2,  PageIndexes_15 },
        { 15,  3,  PageIndexes_15 },
        { 15,  4,  PageIndexes_15 },
        { 16,  2,  PageIndexes_16 },
        { 16,  3,  PageIndexes_16 }, // like IBM POWER big pages
    };

    for (const auto& config : configs) {
        mmu.SetPagingParameters(PhysAddress(0), config.BitsPerPageOffset, config.NumberOfPagingLevels);

        BOOST_TEST_MESSAGE( "virtual address space: "
           << "depth=" << mmu.GetVirtualAddressBits()
           << " pageoffset=" << config.BitsPerPageOffset
           << " levels=" << config.NumberOfPagingLevels
           << " user=" << fmt::hex(mmu.GetHighestUserAddress())
           << " kernel=" << fmt::hex(mmu.GetLowestKernelAddress())
           );

        unsigned const VirtualAddressBits = config.BitsPerPageOffset
             + (config.BitsPerPageOffset - bitsPerAddress) * config.NumberOfPagingLevels
             + bitsPerPageTableIndex;
        BOOST_TEST(VirtualAddressBits == mmu.GetVirtualAddressBits(),
            mmu << " VirtualAddressBits: expected "
            << VirtualAddressBits << " != " << mmu.GetVirtualAddressBits()
            );

        u64 const HighestUserAddress = (U64C(1) << (VirtualAddressBits - 1)) - 1;
        BOOST_TEST(HighestUserAddress == mmu.GetHighestUserAddress(),
            mmu << " user limit: expected "
            << fmt::hex(HighestUserAddress) << " != " << fmt::hex(mmu.GetHighestUserAddress())
            << " bits=" << mmu.GetVirtualAddressBits()
            );

        u64 const LowestKernelAddress = ~HighestUserAddress;
        BOOST_TEST(LowestKernelAddress == mmu.GetLowestKernelAddress(),
            mmu << " kernel limit: expected "
            << fmt::hex(LowestKernelAddress) << " != " << fmt::hex(mmu.GetLowestKernelAddress())
            << " bits=" << mmu.GetVirtualAddressBits()
            );

        TestCanonical(HighestUserAddress, true);
        TestCanonical(HighestUserAddress+1, VirtualAddressBits == 64 ? true : false);
        TestCanonical(LowestKernelAddress, true);
        TestCanonical(LowestKernelAddress-1, VirtualAddressBits == 64 ? true : false);

        TestIndexing(addr, config.PagingIndexes);
    }
}

BOOST_AUTO_TEST_SUITE(test_memory)

BOOST_AUTO_TEST_CASE(test_pta_config)
{
    TranslationUnit_fixture().test();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
