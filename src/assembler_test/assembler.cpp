#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "disassembler/decoder.hpp"
#include "arch/abi.hpp"
#include "assembler/assembler.hpp"

namespace postrisc {

class CAssemblerTester : public CAssembler {
public:
    CAssemblerTester(const Decoder& decoder_) : CAssembler(decoder_) {}
    ~CAssemblerTester() {}

    CInsn2 Assemble(const std::string& string);
    CInsn2 AssembleBranch(const std::string& string, const std::string& labelName,
                          i64 ip, i64 offset, u32 sectionId = ESectionId_code);

    // declare methods for all assembler formats
#define DECLARE_TEST_HANDLER_X(NAME, fields) void Test_##NAME(const std::string& name, EInstructionID realInsnId);
    DECLARE_ASM_FORMATS(DECLARE_TEST_HANDLER_X)

    // definition of test handler
#define DEFINE_TEST_HANDLER(NAME) void CAssemblerTester::Test_##NAME(const std::string& name, EInstructionID realInsnId)
};

CInsn2
CAssemblerTester::Assemble(const std::string& string)
{
    Setup();

    std::istringstream input(string);
    CTokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    CInsn2 cmd(0, 0);
    BOOST_CHECK_NO_THROW(CompileCode(0, cmd));
    return cmd;
}

CInsn2
CAssemblerTester::AssembleBranch(const std::string& string, const std::string& labelName,
    i64 ip, i64 offset, u32 sectionId)
{
    Setup();

    std::istringstream input(string);
    CTokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    const CParserToken token = AddIdentifier(labelName);
    CAssemblerIdentifier& id = GetIdentifierInfo(token.GetData());

    id.SetSectionInfo(sectionId, offset);

    CInsn2 cmd(0, 0);
    BOOST_CHECK_NO_THROW(CompileCode(ip, cmd));
    return cmd;
}

DEFINE_TEST_HANDLER(NoArgs)
{
    // no cases, just mnem without args
    std::ostringstream ss;

    ss << name
       << "\n";

    const CInsn2 insn = Assemble(ss.str());
    BOOST_TEST(GetInstructionLength() == cmd_short);
    BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
}

DEFINE_TEST_HANDLER(r2)
{
    // mnem ra, rb
    static const struct {
      unsigned ra, rb;
    } test_cases[] = {
        { 0,  reg_sp },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Not)
{
    // mnem ra, rb (simm21 == 0)
    static const struct {
      unsigned ra, rb;
    } test_cases[] = {
        { 0,  reg_sp },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == 0);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rbc)
{
    // mnem   rb, rc
    static const struct {
      unsigned rb, rc;
    } test_cases[] = {
        { 6,  65 },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(RbcScale)
{
    // mnem  rb, rc, scale
    static const struct {
      unsigned rb, rc;
      unsigned scale;
    } test_cases[] = {
        {  1,  reg_sp, 2 },
        {  12, 120,    3 },
        {  13, 127,    4 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << test.scale
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetIndexedScale() == test.scale);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(alloc)
{
    // mnem   framesize
    static const struct {
      unsigned rb;
    } test_cases[] = {
        { 6 },
        { 10 },
        { 20 },
        { 120 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << test.rb
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetFrameSize() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(allocsp)
{
    // mnem   framesize, uimm21
    static const struct {
      unsigned rb;
      unsigned imm;
    } test_cases[] = {
        { 6, 0 },
        { 10, 123 },
        { 20, 34 },
        { 120, 4000 },
        { 120, 1004000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << test.rb
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetFrameSize() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Round)
{
    // mnem ra, rb, round_mode
    static const struct {
      unsigned ra, rb;
      unsigned mode;
    } test_cases[] = {
        { 0,  reg_sp, 0 },
        { 10, 120, 2 },
        { 20, 127, 4 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.mode
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetRoundingMode() == test.mode);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(r3)
{
    // mnem ra, rb, rc
    static const struct {
      unsigned ra, rb, rc;
    } test_cases[] = {
        { 0,  1,  reg_sp },
        { 10, 12, 120 },
        { 20, 13, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(Racb)
{
    // add ra, rc, rb
    static const struct {
      unsigned ra, rb, rc;
    } test_cases[] = {
        { 0,  1,  reg_sp },
        { 10, 12, 120 },
        { 20, 13, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rc); // swapped rb <-> rc
        BOOST_TEST(insn.rc() == test.rb); // swapped rb <-> rc
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(RabSc)
{
    // mnem ra, rc, shamt
    static const struct {
      unsigned ra, rb;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  0 },
        { 10, 12, 12 },
        { 20, 13, 63 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.shamt
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_shamt_c() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(FabSc)
{
    // mnem ra, rc, shamt7
    static const struct {
      unsigned ra, rb;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  0 },
        { 10, 12, 127 },
        { 20, 13, 63 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.shamt
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_vshamt_c() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(r4)
{
    // addadd ra, rb, rc, rd
    static const struct {
      unsigned ra, rb, rc, rd;
    } test_cases[] = {
        { 0,  1,  33, reg_sp },
        { 10, 12, 57, 120 },
        { 20, 13, 118, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << '%' << Decoder::GetGprName(test.rd)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.rd() == test.rd);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabcSd)
{
    // mnem ra, rb, rc, shamt
    static const struct {
      unsigned ra, rb, rc;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 118, 32 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << test.shamt
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.get_shamt_d() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(FabcSd)
{
    // mnem ra, rb, rc, shamt7
    static const struct {
      unsigned ra, rb, rc;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 118, 127 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << test.shamt
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.get_vshamt_d() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabScd)
{
    // mnem ra, rb, rc, sd
    static const struct {
      unsigned ra, rb;
      unsigned sc, sd;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 63, 32 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.sc
           << ", "
           << test.sd
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_shamt_c() == test.sc);
        BOOST_TEST(insn.get_shamt_d() == test.sd);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabcSde)
{
    // mnem ra, rb, rc, sd, se
    static const struct {
      unsigned ra, rb, rc;
      unsigned sd, se;
    } test_cases[] = {
        { 0,  1,  33, 0, 32 },
        { 10, 12, 57, 63, 63 },
        { 20, 13, 63, 32, 32 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << test.sd
           << ", "
           << test.se
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.get_shamt_d() == test.sd);
        BOOST_TEST(insn.get_shamt_e() == test.se);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabScde)
{
    // mnem ra, rb, sc, sd, se
    static const struct {
      unsigned ra, rb;
      unsigned sc, sd, se;
    } test_cases[] = {
        { 0,  1,  33, 0, 32 },
        { 10, 12, 57, 63, 63 },
        { 20, 13, 63, 32, 32 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.sc
           << ", "
           << test.sd
           << ", "
           << test.se
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_shamt_c() == test.sc);
        BOOST_TEST(insn.get_shamt_d() == test.sd);
        BOOST_TEST(insn.get_shamt_e() == test.se);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabSI21)
{
    // addi ra, rb, simm21
    static const struct {
      unsigned ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -512000 },
        { 10, 12, -1 },
        { 20, 13, 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabUI21)
{
    // addi ra, rb, uimm21
    static const struct {
      unsigned ra, rb;
      unsigned imm;
    } test_cases[] = {
        { 0,  1,  512000 },
        { 10, 12, 1 },
        { 20, 13, 2012000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabSI21P)
{
    // addi ra, rb, simm21
    static const struct {
      unsigned ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -512000 },
        { 10, 12, -1 },
        { 20, 13, 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm + 1);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabUI21P)
{
    // addi ra, rb, uimm21
    static const struct {
      unsigned ra, rb;
      unsigned imm;
    } test_cases[] = {
        { 0,  1,  512000 },
        { 10, 12, 1 },
        { 20, 13, 2012000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm + 1);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabSI21M)
{
    // addi ra, rb, simm21
    static const struct {
      unsigned ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -512000 },
        { 10, 12, -1 },
        { 20, 13, 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm - 1);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabUI21M)
{
    // addi ra, rb, uimm21
    static const struct {
      unsigned ra, rb;
      unsigned imm;
    } test_cases[] = {
        { 0,  1,  512000 },
        { 10, 12, 1 },
        { 20, 13, 2012000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm - 1);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaxU28)
{
    // nop U28
    static const struct {
      unsigned imm;
    } test_cases[] = {
        { 512000 },
        { 1 },
        { 12000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.get_ldi() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaxbSI21)
{
    // mnem rb, simm21
    static const struct {
      unsigned rb;
      int imm;
    } test_cases[] = {
        { 0,  -512000 },
        { 10, -1 },
        { 20, 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaxUI21)
{
    // mnem uimm21
    static const struct {
      int imm;
    } test_cases[] = {
        { 1000000 },
        { 1 },
        { 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.get_bin_uimm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(ldi)
{
    // mnem ra, simm28
    static const struct {
      unsigned ra;
      int imm;
    } test_cases[] = {
        { 30,  -512000 },
        { 102, -1 },
        { 120, 512000 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.CInsn1::get_ldi() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(gmemx)
{
    // mnem ra, rb, rc, scale, disp10
    static const struct {
      unsigned ra, rb, rc;
      unsigned scale;
      int disp;
    } test_cases[] = {
        { 0,  1,  120, 0, -256 },
        { 10, 12, 33, 1, -1 },
        { 20, 13, 44, 2, 255 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << '%' << Decoder::GetGprName(test.rc)
           << ", "
           << test.scale
           << ", "
           << test.disp
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetIndexedScale() == test.scale);
        BOOST_TEST(insn.CInsn1::GetIndexedDisplacement() == test.disp);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(gmemu)
{
    // mnem ra, rb, imm11_nonzero
    static const struct {
      unsigned ra, rb;
      int stride;
    } test_cases[] = {
        { 20,  1, -512 },
        { 10, 12, -1 },
        { 20, 13, 511 },
        { 20,  1,  -412 },
        { 20,  1,  500 },
        { 10, 12, 1 },
        { 20, 13, -411 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.stride
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUpdateStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabS10)
{
    // mnem ra, rb, imm10_nonzero
    static const struct {
      unsigned ra, rb;
      int stride;
    } test_cases[] = {
        { 20,  1, -512 },
        { 10, 12, -1 },
        { 20, 13, 511 },
        { 20,  1,  -412 },
        { 20,  1,  500 },
        { 10, 12, 1 },
        { 20, 13, -411 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.stride
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUpdateStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RbS10)
{
    // mnem rb, imm10_nonzero
    static const struct {
      unsigned rb;
      int stride;
    } test_cases[] = {
        {  1, -512 },
        { 12, -1 },
        { 13, 511 },
        {  1,  -412 },
        {  1,  500 },
        { 12, 1 },
        { 13, -411 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.stride
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUpdateStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(r2m2)
{
    // mnem ra, rb, dist_yes, dist_no
    static const struct {
      unsigned ra, rb;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  1,  23, 0 },
        { 10, 12, 17, 13 },
        { 20, 13, 23, 23 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << test.mask_yes
           << ", "
           << test.mask_no
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSbm2)
{
    // mnem ra, rb, dist_yes, dist_no
    static const struct {
      unsigned ra;
      unsigned shamt;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  1,  13, 10 },
        { 10, 12, 27, 13 },
        { 20, 63, 23, 23 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.shamt
           << ", "
           << test.mask_yes
           << ", "
           << test.mask_no
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_shamt_b() == test.shamt);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSIm2)
{
    // mnem ra, simm11, dist_yes, dist_no
    static const struct {
      unsigned ra;
      int imm;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  -1024,  23, 10 },
        { 10,    12,  23, 13 },
        { 20,  1023,  13, 10 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << test.mask_yes
           << ", "
           << test.mask_no
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaUIm2)
{
    // mnem ra, uimm11, dist_yes, dist_no
    static const struct {
      unsigned ra;
      unsigned imm;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  1024,  23, 10 },
        { 10,   12,  23, 13 },
        { 20, 2023,  13, 10 },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << test.mask_yes
           << ", "
           << test.mask_no
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_uimm() == test.imm);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(mspr)
{
    // mnem ra, spr
    static const struct {
      unsigned ra, spr;
    } test_cases[] = {
        { 0,  reg_ip },
        { 10, reg_rsc },
        { 20, reg_psr },
    };

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetSprName(test.spr)
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rs() == test.spr);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabBBS)
{
    // mnem ra, rb, label17
    static const struct {
      unsigned ra, rb;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaxbBBS)
{
    // mnem  rb, label17
    static const struct {
      unsigned  rb;
      int ip;
      int offset;
    } test_cases[] = {
        { 33,  2, 44 },
        { 12,  3, 23 },
        { 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RbaBBS)
{
    // mnem ra, rb, label17
    static const struct {
      unsigned ra, rb;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << '%' << Decoder::GetGprName(test.rb)
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.rb);  // switch ra <-> rb
        BOOST_TEST(insn.rb() == test.ra);  // switch ra <-> rb
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSbBBS)
{
    // mnem ra, shamt, label17
    static const struct {
      unsigned ra;
      unsigned shamt;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.shamt
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_shamt_b() == test.shamt);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSIBBS)
{
    // mnem ra, simm11, label17
    static const struct {
      unsigned ra;
      int imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,    -33,  2, 44 },
        { 10,  1000,  3, 23 },
        { 20,   -13, 44,  3 },
        { 10,   535,  3, 23 },
        { 10, -1023,  3, 23 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaUIBBS)
{
    // mnem ra, uimm11, label17
    static const struct {
      unsigned ra;
      unsigned imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,     33,  2, 44 },
        { 10,  2000,  3, 23 },
        { 20,    13, 44,  3 },
        { 10,  1535,  3, 23 },
        { 10,  2023,  3, 23 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_uimm() == test.imm);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSIPBBS)
{
    // mnem ra, simm11, label17
    static const struct {
      unsigned ra;
      int imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,    -33,  2, 44 },
        { 10,  1000,  3, 23 },
        { 20,   -13, 44,  3 },
        { 10,   535,  3, 23 },
        { 10, -1023,  3, 23 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm + 1);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaUIPBBS)
{
    // mnem ra, uimm11, label17
    static const struct {
      unsigned ra;
      unsigned imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,     33,  2, 44 },
        { 10,  2000,  3, 23 },
        { 20,    13, 44,  3 },
        { 10,  1535,  3, 23 },
        { 10,  2023,  3, 23 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_uimm() == test.imm + 1);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(callr)
{
    // mnem ra = label
    static const struct {
      unsigned ra;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  2, 44 },
        { 10, 3, 23 },
        { 20, 444,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br)
{
    // mnem label
    static const struct {
      int ip;
      int offset;
    } test_cases[] = {
        { 22, 44 },
        { 33, 23 },
        { 444,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(gmemr)
{
    // mnem ra, label (forward 28 bit)
    static const struct {
      unsigned ra;
      int ip;
      int offset;
      ESectionId section;
    } test_cases[] = {
        { 0,  112, 40004000, ESectionId_rodata },
        { 10, 223, 23000000, ESectionId_rodata },
        { 20, 44,     30000, ESectionId_data },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset;
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.ComputeForwardRelative(ip) == test.offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(ldar)
{
    // mnem ra, label
    static const struct {
      unsigned ra;
      int ip;
      int offset;
      ESectionId section;
    } test_cases[] = {
        { 0,  112, 40004000, ESectionId_rodata },
        { 10, 223, 23000000, ESectionId_rodata },
        { 20, 44,     30000, ESectionId_rodata },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << '%' << Decoder::GetGprName(test.ra)
           << ", "
           << labelName
           << "\n";

        const i64 offset = test.offset * sizeof(CBundle);
        const i64 ip = test.ip * sizeof(CBundle);

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

// TODO
DEFINE_TEST_HANDLER(write)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(fldsi)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(flddi)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(fldsri) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(flddri) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(fldqri) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }

/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestAssembler)
{
    static const struct {
        void (CAssemblerTester::*ptr)(const std::string& name, EInstructionID realInsnId);
        EInstructionID origInsnId;
        u64       Tmplt;
    } test_cases[] = {
#define X(name, subset, format, descr) \
    { &CAssemblerTester::Test_##format, insn_##name, INSN_TMPLT(name) },
#include "arch/insn_table.hpp"
#undef X
    };

    Decoder decoder;
    for (const auto& test : test_cases) {
        CAssemblerTester tester(decoder);
        EInstructionID realInsnId = Decoder::Decode(CInsn1(test.Tmplt));
        (tester.*test.ptr)(tester.GetDecoder().get_insn_name(test.origInsnId), realInsnId);
    }
}

} // namespace postrisc
