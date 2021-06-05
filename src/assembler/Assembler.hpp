#pragma once

#include "Tokenizer.hpp"
#include "ProgramSection.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

namespace postrisc {

enum EInstructionLength {
    cmd_short,
    cmd_long
};

/****************************************************************************
* asembler directives
****************************************************************************/
#define DECLARE_ASM_DIRECTIVE(X) \
    X(data,    "open data section") \
    X(def,     "define symbol") \
    X(end,     "end program") \
    X(file,    "set source file name") \
    X(global,  "export global name") \
    X(import,  "import global name") \
    X(rodata,  "open rodata section") \
    X(text,    "open text section") \

enum asm_directive : u8 {
#define DIRECTIVE_ENUM_X(name, descr) asm_dir_##name,
    DECLARE_ASM_DIRECTIVE(DIRECTIVE_ENUM_X)
};

/****************************************************************************
* assembler data mnemonics
****************************************************************************/
#define DECLARE_DATA_MNEM(X) \
    X(align) X(ascii) \
    X(d1) X(d2) X(d4) X(d8) \
    X(double) \
    X(float) \
    X(i1) X(i2) X(i4) X(i8) \
    X(quad) \
    X(space) \

enum asm_data_keyword : u8 {
#define DATA_ENUM_X(name) asm_##name,
    DECLARE_DATA_MNEM(DATA_ENUM_X)
};

/***************************************************************************
*
***************************************************************************/
// functions for lo/hi semantic
inline i64 lodisp(i64 imm, unsigned int nbits)
{
    return util::sext64(imm, nbits);
}

inline i64 hidisp(i64 imm, unsigned int nbits)
{
    return (imm - lodisp(imm, nbits)) >> nbits;
}

enum eval_mode_type {
    eval_signed,
    eval_unsigned,
    eval_critical  // must be resolved immediate at first compiler pass
};

// compiler passes
enum class EAssemblerPass {
    compile,  // collect labels, references, compute initial positions
    link      // recompute positions, skip unused labels
};

class Assembler : public Tokenizer {
public:
    explicit Assembler(const Decoder& decoder);
    ~Assembler(void);

    void Setup(void);
    int CompileFile(void);
    void Write(std::ostream& out);

    enum assemble_mode { assemble_mode_obj, assemble_mode_hex };
    int Assemble(std::istream& in, std::ostream& out, assemble_mode mode = assemble_mode_obj);
    const Decoder& GetDecoder(void) const { return m_decoder; }
    void RegisterDefine(const char *name, u64 value);
    void ExportDefinitions(std::ostream& out);

    void CompileCode(i64 ip, InstructionDouble& cmd);
    i64 EvaluateInt(unsigned int nbits, eval_mode_type evalmode, i64 addval);

protected:
    unsigned GetImmediateSize(EInstructionSize size1, EInstructionFullSize size2);
    void SetInstructionLength(EInstructionLength cmdlen);
    EInstructionLength GetInstructionLength(void) const;
    void init_pass(EAssemblerPass passnum);

    void StartNewBundle(void);
    void EmitInstruction(ProgramSection& section, u64 cmd);
    void EmitInstruction(ProgramSection& section, u64 cmd, u64 ext);
    unsigned GetCurrentSlotNum(void) const { return m_CurrentSlotNum; }
    ProgramSection& GetSection(unsigned index) { return m_Sections[index]; }
    size_t GetSectionCount(void) const { return m_Sections.size(); }

private:
    i64 EvaluatePostfixInt(const std::vector<ParserToken>& postfix, eval_mode_type evalmode) const;

    unsigned int GetShiftAmount(void);
    unsigned int GetNullificationDistance(unsigned offset);

    quad_type EvaluateFloat(void);
    quad_type EvaluatePostfixReal(const std::vector<ParserToken>& postfix);
    void CheckLineEnd(void);

    template<class RegisterType> u32 GetRegister(void);
    u32 GetGeneralRegister(void);
    u32 GetSpecialRegister(void);

    u32 GetIdentifier(void);
    i64 GetLabel(i64 ip, ESectionKind segment);
    i64 ComputeBundledOffset(i64 offset) const;
    bool ScanLabel(ProgramSection& section);
    void ScanPredicates(void);
    void ScanPredicate(void);
    u32 GetExpectedInstructionNumber(void) const;
    void EmitLabel(ProgramSection& section, u64 cmd);
    u64 make_nop();

    template<typename SIGNED> void EvaluateSigned(ProgramSection& section);
    template<typename UNSIGNED> void EvaluateUnsigned(ProgramSection& section);
    template<typename FLOAT, typename UNSIGNED> void EvaluateFloat(ProgramSection& section);

    void CompileData(ProgramSection& section);
    void CompileMemoryOrdering(InstructionDouble& cmd);

    void SetForwardBranch(u64 uimm, InstructionDouble& cmd);

    void AssembleForCodeSection(ProgramSection& section);
    void AssembleForDataSection(ProgramSection& section);

    void FieldHandler_unknown(i64 ip, InstructionDouble& cmd);
#define FIELD_HANDLER_PROTOTYPE(field, size, disp, fieldtype, inst_name, descr) \
    void FieldHandler_##field(i64 ip, InstructionDouble& cmd);
    DECLARE_INSTRUCTION_FIELDS(FIELD_HANDLER_PROTOTYPE)
#undef FIELD_HANDLER_PROTOTYPE

    template<typename FLOAT> void ReadonlyFloatForRelative(i64 ip, InstructionDouble& cmd);

private:
    u64                                 m_Slots[slots_per_bundle];   // bundle buffer
    EBundleTemplate                     m_BundleTemplate = EBundleTemplate::bundle_sss;
    unsigned                            m_CurrentSlotNum = 0;            // bundle position
    unsigned                            m_NopNumber = 0;
    EInstructionLength                  m_InstructionLength = cmd_short;
    u32                                 m_InstructionNumber = 0;
    EAssemblerPass                      m_PassNumber = EAssemblerPass::compile;
    std::vector<ProgramSection>         m_Sections;
    const Decoder&                      m_decoder;
    std::map<std::string, u32>          m_defines;
    std::vector<u64>                    m_define_values;
};

} // namespace postrisc
