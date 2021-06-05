#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "emulator/Result.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "assembler/Assembler.hpp"
#include "emulator_test_fixture.hpp"

namespace postrisc {

Core *
emulator_test_fixture::prepare(TargetSystem& targetSystem, const std::string& program)
{
    //util::Logger::set_level(LOG_LEVEL_DEBUG);
    //util::Logger::set_subsystem_mask(0xFFFFFFFF);
    //util::Logger::OpenLogFile("EmulatorTestLog.txt");


    std::istringstream input(program);
    std::ostringstream output;

    Decoder decoder;
    Assembler assembler(decoder);
    assembler.Assemble(input, output, Assembler::assemble_mode_obj);

    std::istringstream input_module(output.str());

    Configuration configuration;
    configuration.SetDefaults();
    const Result result = targetSystem.Setup(configuration);
    BOOST_TEST(result == Result::continue_execution);
    if (result != Result::continue_execution) {
        std::exit(EXIT_FAILURE);
    }

    Core *core = targetSystem.get_bootstrap_core();
    BOOST_TEST(core != nullptr);
    BOOST_TEST(targetSystem.LoadModule(*core, vbase, input_module, header) == Result::continue_execution);
    BOOST_TEST(targetSystem.PrepareThread(*core, vbase, header) == Result::continue_execution);
    return core;
}

void
emulator_test_fixture::run(AddressSpace& as, Core *core)
{
    core->SetIp(vbase + header.entry_point);
    Result result = core->execute_until_interruption(as);
    BOOST_CHECK(result == Result::halt);
}

void
emulator_test_fixture::test_alloc()
{
    static const char program[] =
     ".text\n"
     "   alloc 21\n"
     "   halt\n"
     ".end\n"
     ;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program);
    run(targetSystem, core);

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
emulator_test_fixture::test_binop(const char *insn_name, const TestData_binop& data)
{
    std::ostringstream program;

    program << ".text\n"
            << "   alloc 5\n"
            << "   " << insn_name << "    %r3, %r1, %r2\n"
            << "   halt\n"
            << ".end\n";

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());

    core->wget(1) = data.s1;
    core->wget(2) = data.s2;

    run(targetSystem, core);

    const GeneralRegister result = core->rget(3);

    BOOST_TEST(result == data.result,
        fmt::endl
        << "source1 = " << data.s1 << fmt::endl
        << "source2 = " << data.s2 << fmt::endl
        << "result  = " << result << fmt::endl
        << "example = " << data.result
      );
}

void
emulator_test_fixture::test_binimm(const char *insn_name, const TestData_binimm& data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   alloc 5\n"
            << "   " << insn_name << "    %r3, %r1, 0x" << fmt::hex(data.imm) << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(targetSystem, core);
    const GeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_bin_shamt(const char *insn_name, const TestData_RaAssignRbShamt& data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   alloc 5\n"
            << "   " << insn_name << "    %r3, %r1, " << data.shamt << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(targetSystem, core);
    const GeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_clmul(const char *insn_name, const TestData_clmul& data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   alloc 5\n"
            << "   " << insn_name << "    %r3, %r1, %r2" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    core->wget(2) = data.s2;
    run(targetSystem, core);
    const GeneralRegister result = core->rget(3);
    BOOST_TEST(result == data.result);
}

void
emulator_test_fixture::test_compare_and_branch(const char *insn_name, const TestData_CompareAndBranch& data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   alloc 5\n"
            << "   ld_imm    %r3, 1" << fmt::endl
            << "   " << insn_name << "    %r1, %r2, label" << fmt::endl
            << "   ld_imm    %r3, 0" << fmt::endl
            << "label:" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    core->wget(2) = data.s2;
    run(targetSystem, core);

    const GeneralRegister result = core->rget(3);

    BOOST_TEST(result == GeneralRegister(0, data.isTaken),
           "s1=" << data.s1
        << " s2=" << data.s2
        << " data.isTaken=" << data.isTaken
        << " result=" << result
    );
}

void
emulator_test_fixture::TestCompareWithZeroAndBranch(const char *insn_name, const TestData_CompareWithZeroAndBranch& data)
{
    std::ostringstream program;

    program << ".text" << fmt::endl
            << "   alloc 5\n"
            << "   ld_imm    %r3, 1" << fmt::endl
            << "   " << insn_name << "    %r1, label" << fmt::endl
            << "   ld_imm    %r3, 0" << fmt::endl
            << "label:" << fmt::endl
            << "   halt" << fmt::endl
            << ".end" << fmt::endl;

    Decoder decoder;
    TargetSystem targetSystem(decoder);
    Core *core = prepare(targetSystem, program.str());
    core->wget(1) = data.s1;
    run(targetSystem, core);

    const GeneralRegister result = core->rget(3);

    BOOST_TEST(result == GeneralRegister(0, data.isTaken),
           "s1=" << data.s1
        << " data.isTaken=" << data.isTaken
        << " result=" << result
    );
}

} // namespace postrisc
