#include <atomic>
#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "emulator/utils.hpp"
#include "emulator/core.hpp"
#include "emulator/devices/memory.hpp"
#include "emulator/target_system.hpp"

namespace postrisc {

static void
test(std::ostream & os)
{
    const uint64_t addr  = (UINT64_C(1) << (bits_per_phys_address));
    const uint64_t io    = (UINT64_C(1) << (bits_per_phys_address - bits_per_phys_mem_attrib));
    const uint64_t pal   = (UINT64_C(1) << (bits_per_phys_address - bits_per_phys_mem_attrib - 4));
    const uint64_t pib   = (UINT64_C(1) << 24);

    os  << "pa space:    "
        << fmt::hex(UINT64_C(0))  << ' '
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

    os << fmt::hex(util::change_bit_presentation<float,uint32_t>(2.33f)) << fmt::endl
       << fmt::hex(util::change_bit_presentation<double,uint64_t>(2.33)) << fmt::endl
       << util::change_bit_presentation<quadruple,u128>((quadruple)2.33L) << fmt::endl
       << util::change_bit_presentation<long double,u128>((long double)2.33L) << fmt::endl;

    os << "VM_PAGE_PHYS_ADDRESS_MASK=" << fmt::hex(VM_PAGE_PHYS_ADDRESS_MASK) << fmt::endl;
}

void
Core::RunMemoryTests(std::ostream & os)
{
    test(os);
    CStatus status;
    uint64_t vbase = UINT64_C(0xfffffffa23450000);
    BOOST_TEST(mmu.is_canonical(vbase));

    uint64_t const page_size = mmu.GetPageSize();
    const uint64_t vsize = page_size * 16;
    status = VirtualAllocate(vbase, vsize, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE);

    //os << "vbase=" << fmt::hex(vbase) << ", end=" << fmt::hex(vbase+vsize) << std::endl;
    // memory.dump(os);

    const int nrepeats = 500;
    {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    for (int i=0; i<nrepeats; i++) {
        for (uint64_t address = vbase; address < vbase + vsize; address += sizeof(uint64_t)) {
            const uint64_t value = UINT64_C(0xcececececececece);
            status = mmu_write(address, sizeof(uint64_t), &value);
        }
    }

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    const double run_time = std::chrono::duration <double/*, std::chrono::seconds*/> (diff).count();
    os << run_time << std::endl;
    }

    {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::vector<uint64_t> test(vsize/sizeof(uint64_t), 0);

    for (int i = 0; i < nrepeats; i++) {
        for (size_t k = 0; k < test.size(); k++) {
            test[k] = UINT64_C(0x3434343434343434);
        }
    }
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    const double run_time = std::chrono::duration <double/*, std::chrono::seconds*/> (diff).count();
    os << run_time << std::endl;
    }

    //dump_virt_map(os);
    BOOST_CHECK(VirtualFree(vbase, vsize).success());
    //dump_virt_map(os);

    BOOST_CHECK(VirtualAllocate(vbase, vsize, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE).success());
    //std::cout << "vbase=" << fmt::hex(vbase) << std::endl;

    const unsigned np = 200;
    uint64_t pages[np];
    for (unsigned i=0; i<np; i++) {
        BOOST_CHECK_NO_THROW(pages[i] = get_target_system()->alloc_page());
        //os << "alloc:" << fmt::dec(pages[i]/GetPageSize()) << std::endl;
    }

    for (unsigned i=0; i<np; i++) {
        if ((i % 3) == 0) {
            //os << "free:" << pages[i]/GetPageSize() << std::endl;
            BOOST_CHECK(get_target_system()->free_page(pages[i], page_size).success());
        }
    }

    BOOST_CHECK(!get_target_system()->free_page(vbase, page_size).success());
    //m_TargetSystem->dump(os);

    vbase = UINT64_C(0x0000000712340000);
    uint64_t v2size = 1 << 20;

    BOOST_CHECK(VirtualAllocate(vbase, v2size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE).success());
    //std::cout << "vbase=" << fmt::hex(vbase) << std::endl;

    BOOST_CHECK(VirtualProtect(vbase, v2size, VM_PAGE_RIGHT_EXECUTE).success());

    //dump_virt_map(os);

    //uint64_t bbase = UINT64_C(0xFFFFFF);
    //BOOST_CHECK_NO_THROW(m_TargetSystem->find_device(bbase, 1));
}

BOOST_AUTO_TEST_SUITE(test_memory)

BOOST_AUTO_TEST_CASE(test_vm)
{
    Decoder decoder;
    TargetSystem targetSystem(decoder);

    EmulatorConfiguration configuration;
    configuration.SetDefaults();
    CStatus status = targetSystem.Setup(configuration);
    UNREFERENCED_PARAMETER(status);

    Core * core = targetSystem.get_bootstrap_core();

    core->RunMemoryTests(std::cout);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
