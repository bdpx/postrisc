#pragma once

#include "tokenizer.hpp"
#include "program_section.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "disassembler/decoder.hpp"

namespace postrisc {

enum EInstructionLength {
    cmd_short,
    cmd_long
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

class CAssembler : public CTokenizer {
public:
    CAssembler(const Decoder& decoder);
    ~CAssembler(void);

    void Setup(void);
    int CompileFile(void);
    void Write(std::ostream& out);

    enum assemble_mode { assemble_mode_obj, assemble_mode_hex };
    int Assemble(std::istream& in, std::ostream& out, assemble_mode mode = assemble_mode_obj);
    const Decoder& GetDecoder(void) const { return m_decoder; }
    void RegisterDefine(const char *name, u64 value);
    void ExportDefinitions(std::ostream& out);

protected:
    unsigned GetImmediateSize(EInstructionSize size1, EInstructionFullSize size2);
    void SetInstructionLength(EInstructionLength cmdlen);
    EInstructionLength GetInstructionLength(void) const;
    void init_pass(EAssemblerPass passnum);

    void StartNewBundle(void);
    void EmitInstruction(CProgramSection & section, u64 cmd);
    void EmitInstruction(CProgramSection & section, u64 cmd, u64 ext);
    unsigned GetCurrentSlotNum(void) const { return m_CurrentSlotNum; }
    CProgramSection & GetSection(unsigned index) { return m_Sections[index]; }
    size_t GetSectionCount(void) const { return m_Sections.size(); }

private:
    i64 EvaluateInt(unsigned int nbits, eval_mode_type evalmode, i64 addval);
    i64 EvaluatePostfixInt(const std::vector<CParserToken> & postfix, eval_mode_type evalmode);

    unsigned int GetShiftAmount(void);
    unsigned int GetVectorShiftAmount(void);
    unsigned int GetNullificationDistance(unsigned offset);

    quad_type EvaluateFloat(void);
    quad_type EvaluatePostfixReal(const std::vector<CParserToken> & postfix);
    void CheckLineEnd(void);

    u32 GetRegister(const keywordmap & regs);
    u32 GetGeneralRegister(void);
    u32 GetSpecialRegister(void);

    u32 GetIdentifier(void);
    i64 GetLabel(i64 ip, ESectionKind segment);
    i64 ComputeBundledOffset(i64 offset) const;
    bool ScanLabel(CProgramSection & section);
    void ScanPredicates(void);
    void ScanPredicate(void);
    u32 GetExpectedInstructionNumber(void) const;
    void EmitLabel(CProgramSection & section, u64 cmd);
    u64 make_nop();

    template<class UNSIGNED> void EvaluateUnsigned(CProgramSection & section);
    template<class FLOAT, class UNSIGNED> void EvaluateFloat(CProgramSection & section);

    void CompileCode(i64 ip, CInsn2 & cmd);
    void CompileData(CProgramSection & section);

    void SetForwardBranch(u64 uimm, CInsn2 & cmd);

    void AssembleForCodeSection(CProgramSection & section);
    void AssembleForDataSection(CProgramSection & section);

    void FieldHandler_unknown(i64 ip, CInsn2 & cmd);
#define FIELD_HANDLER_PROTOTYPE(field, size, disp, fieldtype, inst_name, descr) \
    void FieldHandler_##field(i64 ip, CInsn2 & cmd);
    DECLARE_INSTRUCTION_FIELDS(FIELD_HANDLER_PROTOTYPE)
#undef FIELD_HANDLER_PROTOTYPE

    using CRawFieldHandlerFunction = void (CAssembler::*)(i64 ip, CInsn2 & cmd);
    using CFieldHandlerFunction = util::default_initializer<
            CAssembler::CRawFieldHandlerFunction,
            &CAssembler::FieldHandler_unknown>;

    static const std::array<CFieldHandlerFunction, cf_last> s_FieldHandlers;

    template<class FLOAT> void ReadonlyFloatForRelative(i64 ip, CInsn2 & cmd);

public:
    static const keywordmap asm_directives;

private:
    u64                                 m_Slots[slots_per_bundle];   // bundle buffer
    EBundleTemplate                     m_BundleTemplate;
    unsigned                            m_CurrentSlotNum;            // bundle position
    unsigned                            m_NopNumber;
    EInstructionLength                  m_InstructionLength;
    u32                                 m_InstructionNumber;
    EAssemblerPass                      m_PassNumber;
    std::vector<CProgramSection>        m_Sections;
    const Decoder&                      m_decoder;
    std::map<std::string, u32>          m_defines;
    std::vector<u64>                    m_define_values;

    // security hole for unittests
    friend class CAssemblerTester;
    friend class CAssemblerEvaluatorTester;
};

} // namespace postrisc
