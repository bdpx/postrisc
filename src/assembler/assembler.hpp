#pragma once

#include "tokenizer.hpp"
#include "program_section.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "disassembler/decoder.hpp"
#include "util/mpreal.hpp"

namespace postrisc {

enum EInstructionLength {
    cmd_short,
    cmd_long
};

/***************************************************************************
*
***************************************************************************/

// functions for lo/hi semantic
inline int64_t lodisp(int64_t imm, unsigned int nbits) { return util::sext64(imm, nbits); }
inline int64_t hidisp(int64_t imm, unsigned int nbits) { return (imm - lodisp(imm, nbits)) >> nbits; }

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
    void RegisterDefine(const char *name, uint64_t value);
    void ExportDefinitions(std::ostream& out);

protected:
    unsigned GetImmediateSize(EInstructionSize size1, EInstructionFullSize size2);
    void SetInstructionLength(EInstructionLength cmdlen);
    EInstructionLength GetInstructionLength(void) const;
    void init_pass(EAssemblerPass passnum);

    void StartNewBundle(void);
    void EmitInstruction(CProgramSection & section, uint64_t cmd);
    void EmitInstruction(CProgramSection & section, uint64_t cmd, uint64_t ext);
    unsigned GetCurrentSlotNum(void) const { return m_CurrentSlotNum; }
    CProgramSection & GetSection(unsigned index) { return m_Sections[index]; }
    size_t GetSectionCount(void) const { return m_Sections.size(); }

private:
    int64_t EvaluateInt(unsigned int nbits, eval_mode_type evalmode, int64_t addval);
    int64_t EvaluatePostfixInt(const std::vector<CParserToken> & postfix, eval_mode_type evalmode);

    unsigned int GetShiftAmount(void);
    unsigned int GetVectorShiftAmount(void);
    unsigned int GetNullificationDistance(unsigned offset);

    mpfr::mpreal EvaluateFloat(void);
    mpfr::mpreal EvaluatePostfixReal(const std::vector<CParserToken> & postfix);
    void CheckLineEnd(void);

    uint32_t GetRegister(const keywordmap & regs);
    uint32_t GetGeneralRegister(void);
    uint32_t GetSpecialRegister(void);

    uint32_t GetIdentifier(void);
    int64_t GetLabel(int64_t ip, ESectionKind segment);
    int64_t ComputeBundledOffset(int64_t offset) const;
    bool ScanLabel(CProgramSection & section);
    void ScanPredicates(void);
    void ScanPredicate(void);
    uint32_t GetExpectedInstructionNumber(void) const;
    void EmitLabel(CProgramSection & section, uint64_t cmd);
    uint64_t make_nop();

    template<class UNSIGNED> void EvaluateUnsigned(CProgramSection & section);
    template<class FLOAT, class UNSIGNED> void EvaluateFloat(CProgramSection & section);

    void CompileCode(int64_t ip, CInsn2 & cmd);
    void CompileData(CProgramSection & section);

    void SetForwardBranch(uint64_t uimm, CInsn2 & cmd);

    void AssembleForCodeSection(CProgramSection & section);
    void AssembleForDataSection(CProgramSection & section);

    void FieldHandler_unknown(int64_t ip, CInsn2 & cmd);
#define FIELD_HANDLER_PROTOTYPE(field, size, disp, fieldtype, inst_name, descr) void FieldHandler_##field(int64_t ip, CInsn2 & cmd);
    DECLARE_INSTRUCTION_FIELDS(FIELD_HANDLER_PROTOTYPE)

#define FIELD_HANDLER_IMPLEMENTATION(FIELD_NAME) void CAssembler::FieldHandler_##FIELD_NAME(int64_t ip, CInsn2 & cmd)

    using CRawFieldHandlerFunction = void (CAssembler::*)(int64_t ip, CInsn2 & cmd);
    using CFieldHandlerFunction = util::default_initializer<CAssembler::CRawFieldHandlerFunction, &CAssembler::FieldHandler_unknown>;

    static const std::array<CFieldHandlerFunction, cf_last> s_FieldHandlers;

    template<class FLOAT> void ReadonlyFloatForRelative(int64_t ip, CInsn2 & cmd);

public:
    static const keywordmap asm_directives;

private:
    uint64_t                            m_Slots[slots_per_bundle];   // emitter instruction buffer
    EBundleTemplate                     m_BundleTemplate;            // emitter instruction buffer template
    unsigned                            m_CurrentSlotNum;            // emitter instruction buffer position
    unsigned                            m_NopNumber;
    EInstructionLength                  m_InstructionLength;
    uint32_t                            m_InstructionNumber;
    EAssemblerPass                      m_PassNumber;
    std::vector<CProgramSection>        m_Sections;
    const Decoder&                      m_decoder;
    std::map<std::string, uint32_t>     m_defines;
    std::vector<uint64_t>               m_define_values;

    // security hole for unittests
    friend class CAssemblerTester;
    friend class CAssemblerEvaluatorTester;
};

} // namespace postrisc
