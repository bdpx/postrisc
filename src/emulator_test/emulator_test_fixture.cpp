#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "emulator/core.hpp"
#include "emulator/target_system.hpp"
#include "assembler/assembler.hpp"
#include "emulator_test_fixture.hpp"

namespace postrisc {

static bool setted = false;

Core *
emulator_test_fixture::prepare(TargetSystem& targetSystem, const std::string& program)
{
    if (!setted) { setted=true; postrisc::util::setup(); }

    std::istringstream input(program);
    std::ostringstream output;

    CAssembler assembler(decoder);
    assembler.Assemble(input, output, CAssembler::assemble_mode_obj);

    std::istringstream input_module(output.str());

    EmulatorConfiguration configuration;
    configuration.SetDefaults();
    CStatus status = targetSystem.Setup(configuration);

    Core * core = targetSystem.get_bootstrap_core();

    status = core->LoadModule(vbase, input_module, header);

    status = core->PrepareThreadStorage(vbase + UINT64_C(0x1000000), 0x4000);
    status = core->PrepareStack        (vbase + UINT64_C(0x2000000), header.size_stack);
    status = core->PrepareBackingStore (vbase + UINT64_C(0x4000000), header.size_stack);

    return core;
}

void
emulator_test_fixture::run(Core * core)
{
    core->SetIp(vbase + header.entry_point);
    CStatus status = core->execute_until_interruption();
    BOOST_CHECK(status == CStatus(CStatus::halt));
}

void
emulator_test_fixture::test_alloc()
{
    static const char program[] =
     ".text\n"
     "   alloc  r20, alloc_label\n"
     "alloc_label:\n"
     "   halt\n"
     ".end\n"
     ;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program);
    run(core);

    // TODO fix
#if 0
    for (unsigned r = 0; r <= 20; ++r) {
        BOOST_CHECK_NO_THROW(core->rget(r));
        BOOST_CHECK_NO_THROW(core->wget(r));
    }

    for (unsigned r = 21; r < max_stacked_framesize; ++r) {
        BOOST_CHECK_THROW(core->rget(r), CEmulatorException);
        BOOST_CHECK_THROW(core->wget(r), CEmulatorException);
    }

    for (unsigned r = max_stacked_framesize; r < (1U << bits_per_register); ++r)
    {
        BOOST_CHECK_NO_THROW(core->rget(r));
        BOOST_CHECK_NO_THROW(core->wget(r));
    }
#endif
}

void
emulator_test_fixture::test_binop(const char * insn_name, const TestData_binop & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   " << insn_name << "    r3, r1, r2" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());

    core->wget(1) = data.s1;
    core->wget(2) = data.s2;

    run(core);

    const CGeneralRegister result = core->rget(3);

    BOOST_TEST(result == data.result,
        fmt::endl
        << "source1 = " << data.s1 << fmt::endl
        << "source2 = " << data.s2 << fmt::endl
        << "result  = " << result << fmt::endl
        << "example = " << data.result
      );
}

void
emulator_test_fixture::test_binimm(const char * insn_name, const TestData_binimm & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   " << insn_name << "    r3, r1, 0x" << fmt::hex(data.imm) << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(core);
    const CGeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_bin_shamt(const char * insn_name, const TestData_RaAssignRbShamt & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   " << insn_name << "    r3, r1, " << data.shamt << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(core);
    const CGeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_clmul(const char * insn_name, const TestData_clmul & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   " << insn_name << "    r3, r1, r2, " << data.index << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    core->wget(2) = data.s2;
    run(core);
    const CGeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_compare_and_branch(const char * insn_name, const TestData_CompareAndBranch & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   ldi    r3, 1" << fmt::endl
            << "   " << insn_name << "    r1, r2, label" << fmt::endl
            << "   ldi    r3, 0" << fmt::endl
            << "label:" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    core->wget(2) = data.s2;
    run(core);

    const CGeneralRegister result = core->rget(3);

    BOOST_TEST(result == u128(0, data.isTaken),
           "s1=" << data.s1
        << " s2=" << data.s2
        << " data.isTaken=" << data.isTaken
        << " result=" << result
    );
}

void
emulator_test_fixture::TestCompareWithZeroAndBranch(const char * insn_name, const TestData_CompareWithZeroAndBranch & data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   ldi    r3, 1" << fmt::endl
            << "   " << insn_name << "    r1, label" << fmt::endl
            << "   ldi    r3, 0" << fmt::endl
            << "label:" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    TargetSystem targetSystem(decoder);
    Core * core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(core);

    const CGeneralRegister result = core->rget(3);

    BOOST_TEST(result == u128(0, data.isTaken),
           "s1=" << data.s1
        << " data.isTaken=" << data.isTaken
        << " result=" << result
    );
}

} // namespace postrisc
