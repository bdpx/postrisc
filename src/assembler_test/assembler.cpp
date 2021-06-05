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

    CInsn2 Assemble(const std::string & string);
    CInsn2 AssembleBranch(const std::string & string, const std::string & labelName,
                          int64_t ip, int64_t offset, uint32_t sectionId = ESectionId_code);

    // declare methods for all assembler formats
#define DECLARE_TEST_HANDLER_X(NAME, fields) void Test_##NAME(const std::string& name, EInstructionID realInsnId);
    DECLARE_ASM_FORMATS(DECLARE_TEST_HANDLER_X)

    // definition of test handler
#define DEFINE_TEST_HANDLER(NAME) void CAssemblerTester::Test_##NAME(const std::string& name, EInstructionID realInsnId)
};

CInsn2
CAssemblerTester::Assemble(const std::string & string)
{
    Setup();

    std::istringstream input(string);
    CTokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    CInsn2 cmd(0,0);
    const int64_t ip = sizeof(CBundle);
    BOOST_CHECK_NO_THROW(CompileCode(ip, cmd));
    return cmd;
}

CInsn2
CAssemblerTester::AssembleBranch(
    const std::string & string, const std::string & labelName,
    int64_t ip, int64_t offset, uint32_t sectionId)
{
    Setup();

    std::istringstream input(string);
    CTokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    const CParserToken token = AddIdentifier(labelName);
    CAssemblerIdentifier & id = GetIdentifierInfo(token.GetData());

    id.SetSectionInfo(sectionId, offset);

    CInsn2 cmd(0,0);
    //const int64_t ip = m_CodeSection.ComputeNextIp();
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

DEFINE_TEST_HANDLER(Ra)
{
    // mnem ra
    static const struct {
      uint8_t ra;
    } test_cases[] = {
        { 0 },
        { 10 },
        { 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rb)
{
    // mnem rb
    static const struct {
      uint8_t rb;
    } test_cases[] = {
        { 0 },
        { 10 },
        { 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.rb]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rab)
{
    // mnem ra, rb
    static const struct {
      uint8_t ra, rb;
    } test_cases[] = {
        { 0,  reg_sp },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rbc)
{
    // mnem   rb, rc
    static const struct {
      uint8_t rb, rc;
    } test_cases[] = {
        { 6,  65 },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rcb)
{
    // mnem   rc, rb
    static const struct {
      uint8_t rc, rb;
    } test_cases[] = {
        { 6,  65 },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Round)
{
    // mnem ra, rb, round_mode
    static const struct {
      uint8_t ra, rb;
      unsigned mode;
    } test_cases[] = {
        { 0,  reg_sp, 0 },
        { 10, 120, 2 },
        { 20, 127, 4 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << test.mode
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetIndexedScale() == test.mode);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(xchg)
{
    // mnem ra, rb
    static const struct {
      uint8_t ra, rb;
    } test_cases[] = {
        { 0,  reg_sp },
        { 10, 120 },
        { 20, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(Rabc)
{
    // mnem ra, rb, rc
    static const struct {
      uint8_t ra, rb, rc;
    } test_cases[] = {
        { 0,  1,  reg_sp },
        { 10, 12, 120 },
        { 20, 13, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
      uint8_t ra, rb, rc;
    } test_cases[] = {
        { 0,  1,  reg_sp },
        { 10, 12, 120 },
        { 20, 13, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
      uint8_t ra, rb;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  0 },
        { 10, 12, 12 },
        { 20, 13, 63 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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
      uint8_t ra, rb;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  0 },
        { 10, 12, 127 },
        { 20, 13, 63 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(Rabcd)
{
    // addadd ra, rb, rc, rd
    static const struct {
      uint8_t ra, rb, rc, rd;
    } test_cases[] = {
        { 0,  1,  33, reg_sp },
        { 10, 12, 57, 120 },
        { 20, 13, 118, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << m_decoder.gpr_names[test.rd]
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
      uint8_t ra, rb, rc;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 118, 32 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
      uint8_t ra, rb, rc;
      unsigned shamt;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 118, 127 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
      uint8_t ra, rb;
      unsigned sc, sd;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 63 },
        { 20, 13, 63, 32 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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
      uint8_t ra, rb, rc;
      unsigned sd, se;
    } test_cases[] = {
        { 0,  1,  33, 0, 32 },
        { 10, 12, 57, 63, 63 },
        { 20, 13, 63, 32, 32 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
      uint8_t ra, rb;
      unsigned sc, sd, se;
    } test_cases[] = {
        { 0,  1,  33, 0, 32 },
        { 10, 12, 57, 63, 63 },
        { 20, 13, 63, 32, 32 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(RabI21)
{
    // addi ra, rb, I21
    static const struct {
      uint8_t ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -512000 },
        { 10, 12, -1 },
        { 20, 13, 512000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(trap)
{
    // teq ra, rb, I21
    static const struct {
      uint8_t ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -512000 },
        { 10, 12, -1 },
        { 20, 13, 512000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(RabU21)
{
    // maxui ra, rb, U21
    static const struct {
      uint8_t ra, rb;
      unsigned imm;
    } test_cases[] = {
        { 0,  1,  512000 },
        { 10, 12, 1 },
        { 20, 13, 1024000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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

    for (const auto & test : test_cases) {
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

DEFINE_TEST_HANDLER(RaxbI21)
{
    // mnem rb, I21
    static const struct {
      uint8_t rb;
      int imm;
    } test_cases[] = {
        { 0,  -512000 },
        { 10, -1 },
        { 20, 512000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(RaxbU21)
{
    // mnem rb, U21
    static const struct {
      uint8_t rb;
      int imm;
    } test_cases[] = {
        { 0,  512000 },
        { 10, 100 },
        { 20, 512000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.rb]
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

DEFINE_TEST_HANDLER(ldi)
{
    // mnem ra, I28
    static const struct {
      uint8_t ra;
      int imm;
    } test_cases[] = {
        { 30,  -512000 },
        { 102, -1 },
        { 120, 512000 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
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
      uint8_t ra, rb, rc;
      unsigned scale;
      int disp;
    } test_cases[] = {
        { 0,  1,  120, 0, -256 },
        { 10, 12, 33, 1, -1 },
        { 20, 13, 44, 2, 255 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
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
    // mnem ra, rb, I14
    static const struct {
      uint8_t ra, rb;
      int stride;
    } test_cases[] = {
        { 0,  1, -512 },
        { 10, 12, -1},
        { 20, 13, 511 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << " , "
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

DEFINE_TEST_HANDLER(RabU14)
{
    // mnem ra, rb, I14
    static const struct {
      uint8_t ra, rb;
      uint32_t stride;
    } test_cases[] = {
        { 0,  1,  512 },
        { 10, 12, 1},
        { 20, 13, 10511 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << " , "
           << test.stride
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUintStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(gmemux)
{
    // mnem ra, rb, rc, scale
    static const struct {
      uint8_t ra, rb, rc;
      unsigned scale;
    } test_cases[] = {
        { 0,  1, 51, 2 },
        { 10, 12, 32, 1 },
        { 20, 13, 51, 1 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << test.scale
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetIndexedScale() == test.scale);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(loop)
{
    // mnem ra, rb, step, label14
    static const struct {
      uint8_t ra, rb;
      int8_t step;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  45,  33,  2, 44 },
        { 10, 45, -12,  3, 23 },
        { 20, 45, -13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << (int)test.step
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetLoopStep() == test.step);
        BOOST_TEST(insn.GetShortBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(clmul)
{
    // clmul ra, rb, rc, U2
    static const struct {
      uint8_t ra, rb, rc;
      unsigned index;
    } test_cases[] = {
        { 0,  1,  0, 0 },
        { 10, 12, 127, 1 },
        { 20, 13, 63, 2 },
        { 20, 13, 63, 3 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << test.index
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetIndexedScale() == test.index);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(aload)
{
    // mnem ra, rb [, order]
    static const struct {
      uint8_t ra, rb;
      EMemoryOrdering order;
      char orderName[16];
    } test_cases[] = {
        { 0,  23, order_relaxed,         "" },
        { 10, 1,  order_acquire,         ", acquire" },
        { 20, 63, order_release,         ", release" },
        { 20, 63, order_acquire_release, ", acqrel" },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << test.orderName
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetMemoryOrdering() == test.order);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(amo)
{
    // mnem ra, rb, rc [, order]
    static const struct {
      uint8_t ra, rb, rc;
      EMemoryOrdering order;
      char orderName[16];
    } test_cases[] = {
        { 0,  23, 120, order_relaxed,         "" },
        { 10, 1,   22, order_acquire,         ", acquire" },
        { 20, 63,  22, order_release,         ", release" },
        { 20, 63,  22, order_acquire_release, ", acqrel" },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << test.orderName
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetMemoryOrdering() == test.order);
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

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
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
      uint8_t ra;
      unsigned shamt;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  1,  13, 10 },
        { 10, 12, 27, 13 },
        { 20, 63, 23, 23 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
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

DEFINE_TEST_HANDLER(RaI14m2)
{
    // mnem ra, simm14, dist_yes, dist_no
    static const struct {
      uint8_t ra;
      int imm;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        { 0,  -7964,  23, 10 },
        { 10,    12,  23, 13 },
        { 20,  7963,  13, 10 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
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

DEFINE_TEST_HANDLER(RaU14m2)
{
    // mnem ra, uimm14, dist_yes, dist_no
    static const struct {
      uint8_t ra;
      unsigned imm;
      unsigned mask_yes, mask_no;
    } test_cases[] = {
        {  0,   127, 10, 20 },
        { 10,    12, 10, 13 },
        { 20, 15063, 23, 23 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
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
      uint8_t ra, spr;
    } test_cases[] = {
        { 0,  reg_ip },
        { 10, reg_rsc },
        { 20, reg_psr },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.spr_names[test.spr]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rs() == test.spr);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(sprsr)
{
    // mnem ra, spr, rc, rd
    static const struct {
      uint8_t ra, spr, rc, rd;
    } test_cases[] = {
        { 0,  reg_ip, 11, 23 },
        { 10, reg_rsc, 11, 23 },
        { 20, reg_psr, 11, 23 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.spr_names[test.spr]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << m_decoder.gpr_names[test.rd]
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rs() == test.spr);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.rd() == test.rd);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RabB21)
{
    // mnem ra, rb, label
    static const struct {
      uint8_t ra, rb;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RbaB21)
{
    // mnem ra, rb, label
    static const struct {
      uint8_t ra, rb;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.rb);  // switch ra <-> rb
        BOOST_TEST(insn.rb() == test.ra);  // switch ra <-> rb
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaSbB21)
{
    // mnem ra, shamt, label
    static const struct {
      uint8_t ra;
      unsigned shamt;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  2, 44 },
        { 10, 12,  3, 23 },
        { 20, 13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << test.shamt
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_shamt_b() == test.shamt);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaI14B14)
{
    // mnem ra, I14, label
    static const struct {
      uint8_t ra;
      int imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,    -33,  2, 44 },
        { 10,  2000,  3, 23 },
        { 20,   -13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm);
        BOOST_TEST(insn.GetShortBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(RaU14B14)
{
    // mnem ra, U14, label
    static const struct {
      uint8_t ra;
      unsigned imm;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,   6000,  2, 44 },
        { 10,  2000,  3, 23 },
        { 20,    13, 44,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << test.imm
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_uimm() == test.imm);
        BOOST_TEST(insn.GetShortBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(callr)
{
    // mnem ra = label
    static const struct {
      uint8_t ra;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  2, 44 },
        { 10, 3, 23 },
        { 20, 444,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

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

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

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
      uint8_t ra;
      int ip;
      int offset;
      ESectionId section;
    } test_cases[] = {
        { 0,  112, 40004000, ESectionId_rodata },
        { 10, 223, 23000000, ESectionId_rodata },
        { 20, 44,     30000, ESectionId_data },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetForwardBranch() == test.offset - ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(ldar)
{
    // mnem ra, label
    static const struct {
      uint8_t ra;
      int ip;
      int offset;
      ESectionId section;
    } test_cases[] = {
        { 0,  112, 40004000, ESectionId_rodata },
        { 10, 223, 23000000, ESectionId_rodata },
        { 20, 44,     30000, ESectionId_rodata },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << labelName
           << "\n";

        const int64_t offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(fpcmp)
{
    // mnem ra, rb, rc, FCMPMODEMASK
    static const struct {
      uint8_t ra, rb, rc;
      unsigned mode;
    } test_cases[] = {
        { 0,  1,  33, 0 },
        { 10, 12, 57, 31 },
        { 20, 13, 118, 15 },
    };

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << m_decoder.gpr_names[test.rb]
           << ", "
           << m_decoder.gpr_names[test.rc]
           << ", "
           << test.mode
           << "\n";

        const CInsn2 insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetFpCompareMode() == test.mode);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(brzr)
{
    // mnem ra, label
    static const struct {
      uint8_t ra;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  2, 44 },
        { 10, 3, 23 },
        { 20, 444,  3 },
    };

    const std::string labelName = "LabelName";

    for (const auto & test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << m_decoder.gpr_names[test.ra]
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * bytes_per_bundle;
        const int64_t ip = test.ip * bytes_per_bundle;

        const CInsn2 insn = AssembleBranch(ss.str(), labelName, ip, offset);
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
DEFINE_TEST_HANDLER(fldqi)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(fldqih) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }

/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestAssembler)
{
    static const struct {
        void (CAssemblerTester::*ptr)(const std::string& name, EInstructionID realInsnId);
        EInstructionID origInsnId;
        EInstructionID realInsnId;
    } test_cases[] = {
#define X(name, subset, inst_format, format, iic, tmplt, descr) \
    { &CAssemblerTester::Test_##format, insn_##name, insn_##tmplt },

#include "arch/insn_table.hpp"

#undef X
    };

    Decoder decoder;
    for (const auto& test : test_cases) {
        CAssemblerTester tester(decoder);
        (tester.*test.ptr)(tester.GetDecoder().get_insn_name(test.origInsnId), test.realInsnId);
    }
}

} // namespace postrisc
