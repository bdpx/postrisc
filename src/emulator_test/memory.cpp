#include <boost/test/unit_test.hpp>

#include <atomic>
#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/DeviceMemory.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "emulator_app/TargetSingleApp.hpp"
#include "doom/CliDoomInterface.hpp"

namespace postrisc {

static void test(std::ostream& os)
{
    const u64 addr  = (U64C(1) << (bits_per_phys_address));
    const u64 io    = (U64C(1) << (bits_per_phys_address - bits_per_phys_mem_attrib));
    const u64 pal   = (U64C(1) << (bits_per_phys_address - bits_per_phys_mem_attrib - 4));
    const u64 pib   = (U64C(1) << 24);

    os  << "pa space:    "
        << fmt::hex(U64C(0))  << ' '
        << fmt::hex(addr - io - 1) << fmt::endl
        << "mmio:        "
        << fmt::hex(addr - io)       << ' '
        << fmt::hex(addr - pal - 1)  << fmt::endl
        << "pal:         "
        << fmt::hex(addr - pal)     << ' '
        << fmt::hex(addr - pib - 1)  << fmt::endl
        << "p-int-block: "
        << fmt::hex(addr - pib)     << ' '
        << fmt::hex(addr - 1) << fmt::endl;

    os << "asm_fmt_last=" << static_cast<unsigned>(asm_fmt_last) <<
          " inst_fmt_last=" << static_cast<unsigned>(inst_fmt_last) << fmt::endl;

    os << fmt::hex(std::bit_cast<u32>(2.33f)) << fmt::endl
       << fmt::hex(std::bit_cast<u64>(2.33)) << fmt::endl
       << fmt::hex(std::bit_cast<u128>((native::f128)2.33L)) << fmt::endl;

    os << "VM_PAGE_PHYS_ADDRESS_MASK=" << fmt::hex(VM_PAGE_PHYS_ADDRESS_MASK) << fmt::endl;
}

void TargetSystem::RunMemoryTests(std::ostream& os)
{
    test(os);
    Result result;

    Core *core = get_bootstrap_core();
    const TranslationUnit &mmu = core->GetMMU();

    u64 vbase = U64C(0xfffffffa23450000);
    BOOST_TEST(mmu.is_canonical(vbase));

    u64 const page_size = mmu.GetPageSize();
    const u64 vsize = page_size * 16;
    result = VirtualAllocate(mmu, vbase, vsize, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE);

    //os << "vbase=" << fmt::hex(vbase) << ", end=" << fmt::hex(vbase+vsize) << std::endl;
    // memory.dump(os);

    const int nrepeats = 500;
    {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    for (int i=0; i<nrepeats; i++) {
        for (u64 address = vbase; address < vbase + vsize; address += sizeof(u64)) {
            const u64 value = U64C(0xcececececececece);
            result = mmu_write(*core, VirtAddress(address), value);
        }
    }

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    const double run_time = std::chrono::duration <double/*, std::chrono::seconds*/> (diff).count();
    os << run_time << std::endl;
    }

    {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::vector<u64> test(vsize/sizeof(u64), 0);

    for (int i = 0; i < nrepeats; i++) {
        for (size_t k = 0; k < test.size(); k++) {
            test[k] = U64C(0x3434343434343434);
        }
    }
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    const double run_time = std::chrono::duration <double/*, std::chrono::seconds*/> (diff).count();
    os << run_time << std::endl;
    }

    //dump_virt_map(os);
    BOOST_CHECK(VirtualFree(mmu, vbase, vsize).success());
    //dump_virt_map(os);

    BOOST_CHECK(VirtualAllocate(mmu, vbase, vsize, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE).success());
    //std::cout << "vbase=" << fmt::hex(vbase) << std::endl;

    const unsigned np = 200;
    PhysAddress pages[np];
    for (unsigned i=0; i<np; i++) {
        BOOST_CHECK_NO_THROW(pages[i] = alloc_page());
        //os << "alloc:" << fmt::dec(pages[i]/GetPageSize()) << std::endl;
    }

    for (unsigned i=0; i<np; i++) {
        if ((i % 3) == 0) {
            //os << "free:" << pages[i]/GetPageSize() << std::endl;
            BOOST_CHECK(free_page(pages[i], page_size).success());
        }
    }

    BOOST_CHECK(!free_page(PhysAddress(vbase), page_size).success());
    //m_TargetSystem->dump(os);

    vbase = U64C(0x0000000712340000);
    u64 v2size = 1 << 20;

    BOOST_CHECK(VirtualAllocate(mmu, vbase, v2size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE).success());
    //std::cout << "vbase=" << fmt::hex(vbase) << std::endl;

    BOOST_CHECK(VirtualProtect(mmu, vbase, v2size, VM_PAGE_RIGHT_EXECUTE).success());

    //dump_virt_map(os);

    //u64 bbase = U64C(0xFFFFFF);
    //BOOST_CHECK_NO_THROW(m_TargetSystem->find_device(bbase, 1));
}

BOOST_AUTO_TEST_SUITE(test_memory)

BOOST_AUTO_TEST_CASE(test_vm)
{
    Decoder decoder;
    TargetSystem targetSystem(decoder);

    Configuration configuration;
    configuration.SetDefaults();
    Result result = targetSystem.Setup(configuration);
    UNREFERENCED_PARAMETER(result);
    targetSystem.RunMemoryTests(std::cout);
}

BOOST_AUTO_TEST_CASE(test_memory_wrapper)
{
    Decoder decoder;
    TargetSystem targetSystem(decoder);

    Configuration configuration;
    configuration.SetDefaults();
    Result result = targetSystem.Setup(configuration);
    UNREFERENCED_PARAMETER(result);

    Core *core = targetSystem.get_bootstrap_core();

    size_t size = 4 * 1024 * 1024;
#define TEST_PAGE_SIZE (4 * 1024)
    std::vector<u8, boost::alignment::aligned_allocator<u8, TEST_PAGE_SIZE>> data(size);
    CliDoomInterface doom;
    TargetSingleApp target(TEST_PAGE_SIZE, decoder, doom);
#undef TEST_PAGE_SIZE
    target.AddHostMemory(data.data(), size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE);
    u64 value = 4;
    BOOST_CHECK(target.mmu_write(*core, VirtAddress(std::bit_cast<uintptr_t>(data.data()) + 8), value).success());

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
