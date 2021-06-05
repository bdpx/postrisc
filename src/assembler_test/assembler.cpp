#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"
#include "assembler/Assembler.hpp"

namespace postrisc {

class AssemblerTester : public Assembler {
public:
    AssemblerTester(const Decoder& decoder_) : Assembler(decoder_) {}
    ~AssemblerTester() {}

    InstructionDouble Assemble(const std::string& string);
    InstructionDouble AssembleBranch(const std::string& string, const std::string& labelName,
                          i64 ip, i64 offset, u32 sectionId = ESectionId_code);

    // declare methods for all assembler formats
#define DECLARE_TEST_HANDLER_X(NAME, fields) void Test_##NAME(const std::string& name, EInstructionID realInsnId);
    DECLARE_ASM_FORMATS(DECLARE_TEST_HANDLER_X)

    // definition of test handler
#define DEFINE_TEST_HANDLER(NAME) void AssemblerTester::Test_##NAME(const std::string& name, EInstructionID realInsnId)
};

InstructionDouble
AssemblerTester::Assemble(const std::string& string)
{
    Setup();

    std::istringstream input(string);
    Tokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    InstructionDouble cmd(0, 0);
    BOOST_CHECK_NO_THROW(CompileCode(0, cmd));
    return cmd;
}

InstructionDouble
AssemblerTester::AssembleBranch(const std::string& string, const std::string& labelName,
    i64 ip, i64 offset, u32 sectionId)
{
    Setup();

    std::istringstream input(string);
    Tokenizer::Init();
    TokenizeInput(input);

    init_pass(EAssemblerPass::link);

    BOOST_TEST(IsNextRowExist());

    const ParserToken token = AddIdentifier(labelName);
    AssemblerIdentifier& id = GetIdentifierInfo(token.GetData());

    id.SetSectionInfo(sectionId, offset);

    InstructionDouble cmd(0, 0);
    BOOST_CHECK_NO_THROW(CompileCode(ip, cmd));
    return cmd;
}

DEFINE_TEST_HANDLER(NoArgs)
{
    // no cases, just mnem without args
    std::ostringstream ss;

    ss << name
       << "\n";

    const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetFrameSize() == test.rb);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(alloc_sp)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetFrameSize() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm);
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(r2s1)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_shamt_c() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId, name);
    }
}

DEFINE_TEST_HANDLER(r4)
{
    // add.add ra, rb, rc, rd
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.rd() == test.rd);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(r3s1)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.get_shamt_d() == test.shamt);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(r2s2)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_shamt_c() == test.sc);
        BOOST_TEST(insn.get_shamt_d() == test.sd);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(r3s2)
{
    // dep ra, rb, rc, sd, se
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.get_shamt_d() == test.sd);
        BOOST_TEST(insn.get_shamt_e() == test.se);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(bin_simm)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(bin_uimm)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_uimm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(mid_bin_simm)
{
    // addi ra, rb, simm14
    static const struct {
      unsigned ra, rb;
      int imm;
    } test_cases[] = {
        { 0,  1,  -8120 },
        { 10, 12, -1 },
        { 20, 13, 8120 },
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_mid_bin_simm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(nop)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.get_ld_imm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(cache_simm)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_bin_simm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(retf)
{
    // retf uimm21
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.get_bin_uimm() == test.imm);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(ld_imm)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.InstructionSingle::get_ld_imm() == test.imm);
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
        { 0,  1,  120, 0, -56 },
        { 10, 12, 33, 1, -1 },
        { 20, 13, 44, 2, 55 },
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.rc() == test.rc);
        BOOST_TEST(insn.GetIndexedScale() == test.scale);
        BOOST_TEST(insn.InstructionSingle::GetIndexedDisplacement() == test.disp);
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

        const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUpdateStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(int)
{
    // mnem rb, imm10
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetUpdateStride() == test.stride);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(nul_rr)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(nul_rs)
{
    // mnem ra, sb, dist_yes, dist_no
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_shamt_b() == test.shamt);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(nul_simm)
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm);
        BOOST_TEST(insn.get_nullify_yes() == test.mask_yes);
        BOOST_TEST(insn.get_nullify_not() == test.mask_no);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(nul_uimm)
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

        const InstructionDouble insn = Assemble(ss.str());
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

        const InstructionDouble insn = Assemble(ss.str());
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rs() == test.spr);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br_rr)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(loop)
{
    // mnem ra, rb, stride, label11
    static const struct {
      unsigned ra, rb;
      unsigned stride;
      int ip;
      int offset;
    } test_cases[] = {
        { 0,  33,  1,  2, 44 },
        { 10, 12, 11,  3, 23 },
        { 20, 13, 64, 44,  3 },
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
           << test.stride
           << ", "
           << labelName
           << "\n";

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetLoopStride() == test.stride - 1);
        BOOST_TEST(insn.GetLoopDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br_eh)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.rb() == test.rb);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br_rs)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_shamt_b() == test.shamt);
        BOOST_TEST(insn.GetBranchDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br_simm)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_simm() == test.imm);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(br_uimm)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.get_brc_uimm() == test.imm);
        BOOST_TEST(insn.ComputeBranchRelative(ip) == offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(call)
{
    // mnem ra, label
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(jmp)
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

        const int offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(mem_iprel)
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
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.ComputeForwardRelative(ip) == test.offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(write)
{
    // mnem label (forward 28 bit)
    static const struct {
      int ip;
      int offset;
      ESectionId section;
    } test_cases[] = {
        { 112, 40004000, ESectionId_rodata },
        { 223, 23000000, ESectionId_rodata },
        { 44,     30000, ESectionId_data },
    };

    const std::string labelName = "LabelName";

    for (const auto& test : test_cases) {
        std::ostringstream ss;

        ss << name
           << " "
           << labelName
           << "\n";

        const int offset = test.offset;
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ComputeForwardRelative(ip) == test.offset);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

DEFINE_TEST_HANDLER(lda_r)
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

        const i64 offset = test.offset * sizeof(Bundle);
        const i64 ip = test.ip * sizeof(Bundle);

        const InstructionDouble insn = AssembleBranch(ss.str(), labelName, ip, offset, test.section);
        BOOST_TEST(GetInstructionLength() == cmd_short);

        BOOST_TEST(insn.ra() == test.ra);
        BOOST_TEST(insn.GetJumpDisplacement() == test.offset - test.ip);
        BOOST_TEST(m_decoder.Decode(insn) == realInsnId);
    }
}

// TODO
DEFINE_TEST_HANDLER(ld_imm_f32) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(ld_imm_f64) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }

DEFINE_TEST_HANDLER(ld_iprel_f32)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(ld_iprel_f64)  { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }
DEFINE_TEST_HANDLER(ld_iprel_f128) { UNREFERENCED_PARAMETER(name); UNREFERENCED_PARAMETER(realInsnId); }

/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestAssembler)
{
    static const struct {
        void (AssemblerTester::*ptr)(const std::string& name, EInstructionID realInsnId);
        EInstructionID origInsnId;
        u64       Tmplt;
    } test_cases[] = {
#define X(name, subset, format, descr) \
    { &AssemblerTester::Test_##format, insn_##name, INSN_TMPLT(name) },
#include "arch/insn_table.hpp"
#undef X
    };

    Decoder decoder;
    for (const auto& test : test_cases) {
        AssemblerTester tester(decoder);
        EInstructionID realInsnId = Decoder::Decode(InstructionSingle(test.Tmplt));
        (tester.*test.ptr)(tester.GetDecoder().get_insn_name(test.origInsnId), realInsnId);
    }
}

} // namespace postrisc
