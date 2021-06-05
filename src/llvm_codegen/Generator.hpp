#pragma once

#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"
#include "immediates.hpp"

#define DECLARE_CODEGEN_HANDLERS(X) \
    X(AtomicFences, "") \
    X(AtomicLoads, "") \
    X(AtomicStores, "") \
    X(AtomicSwaps, "") \
    X(AtomicCompareSwaps, "") \
    X(AtomicLoadOps, "") \
    X(AtomicStoreOps, "") \
    X(BitCast, "") \
    X(BinaryRegImmI32, "") \
    X(BinaryRegImmU32, "") \
    X(BinaryRegImmI32Specifiec, "") \
    X(BinaryRegImmU32Specifiec, "") \
    X(BinaryRegImmI64, "") \
    X(BinaryRegImmU64, "") \
    X(BinaryRegImmI64Specifiec, "") \
    X(BinaryRegImmU64Specifiec, "") \
    X(BinaryRegImmI128, "") \
    X(BinaryRegImmU128, "") \
    X(BranchCompareRegReg, "") \
    X(BranchCompareImmI32, "") \
    X(BranchCompareImmI64, "") \
    X(BranchCompareImmI128, "") \
    X(BranchCompareImmU32, "") \
    X(BranchCompareImmU64, "") \
    X(BranchCompareImmU128, "") \
    X(BranchCompareZero, "") \
    X(BranchOnBit, "") \
    X(BuiltinNoArgs, "") \
    X(BuiltinUnary, "") \
    X(BuiltinBinary, "") \
    X(BuiltinTernary, "") \
    X(DividePow2, "") \
    X(FpBranchCompare, "-mllvm --enable-nullification=0") \
    X(FpBranchCompareGoto, "-mllvm --enable-nullification=0") \
    X(FpBranchCompareStrict, "-ffp-model=strict -mllvm --enable-nullification=0") \
    X(FpUnary, "-Xclang -fnative-half-type") \
    X(FpBinary, "-Xclang -fnative-half-type") \
    X(FpTernary, "-Xclang -fnative-half-type") \
    X(FpToInt, "") \
    X(FpStrict, "-ffp-model=strict") \
    X(FunctionPointer, "") \
    X(IntUnary, "") \
    X(IntBinary, "") \
    X(IntAlignUpU64, "") \
    X(IntDeposit32, "") \
    X(IntDeposit64, "") \
    X(IntTernary, "") \
    X(NullificationOnBit, "-mllvm --enable-nullification=1") \
    X(NullifyCompareRegReg, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmI32, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmI64, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmI128, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmU32, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmU64, "-mllvm --enable-nullification=1") \
    X(NullifyCompareImmU128, "-mllvm --enable-nullification=1") \
    X(NullifyCompareZero, "-mllvm --enable-nullification=1") \
    X(NullifyFpCompare, "-mllvm --enable-nullification=1") \
    X(NullifyFpCompareStrict, "-ffp-model=strict -mllvm --enable-nullification=1") \
    X(StackArray, "") \
    X(LoadBase, "") \
    X(StoreBase, "") \
    X(LoadGlobalSmall, "-mcmodel=small") \
    X(StoreGlobalSmall, "-mcmodel=small") \
    X(LoadGlobalMedium, "-mcmodel=medium") \
    X(StoreGlobalMedium, "-mcmodel=medium") \
    X(LoadGlobalLarge, "-mcmodel=large") \
    X(StoreGlobalLarge, "-mcmodel=large") \
    X(LoadStoreTlsSmallLocalExec, "-mcmodel=small -ftls-model=local-exec") \
    X(LoadStoreTlsSmallLocalDynamic, "-mcmodel=small -ftls-model=local-dynamic") \
    X(LoadStoreTlsSmallGlobalDynamic, "-mcmodel=small -ftls-model=global-dynamic") \
    X(LoadStoreTlsSmallInitialExec, "-mcmodel=small -ftls-model=initial-exec") \
    X(LoadStoreTlsSmallDefault, "-mcmodel=small") \
    X(LoadStoreTlsMediumLocalExec, "-mcmodel=medium -ftls-model=local-exec") \
    X(LoadStoreTlsMediumLocalDynamic, "-mcmodel=medium -ftls-model=local-dynamic") \
    X(LoadStoreTlsMediumGlobalDynamic, "-mcmodel=medium -ftls-model=global-dynamic") \
    X(LoadStoreTlsMediumInitialExec, "-mcmodel=medium -ftls-model=initial-exec") \
    X(LoadStoreTlsMediumDefault, "-mcmodel=medium") \
    X(LoadStoreTlsLargeLocalExec, "-mcmodel=large  -ftls-model=local-exec") \
    X(LoadStoreTlsLargeLocalDynamic, "-mcmodel=large  -ftls-model=local-dynamic") \
    X(LoadStoreTlsLargeGlobalDynamic, "-mcmodel=large  -ftls-model=global-dynamic") \
    X(LoadStoreTlsLargeInitialExec, "-mcmodel=large  -ftls-model=initial-exec") \
    X(LoadStoreTlsLargeDefault, "-mcmodel=large") \


namespace postrisc {
namespace llvm {

enum CodeModel {
    SMALL,
    MEDIUM,
    LARGE,
    DefaultModel = SMALL
};
enum ThreadModel {
    LocalExec,
    LocalDynamic,
    GlobalDynamic,
    InitialExec,
    DefaultTLS
};

struct noargs_info {
    char name[32];
    EInstructionID insn;
    char dst_type[8];
    char args[32];
    char fragment[64];
};

struct unary_info {
    char name[32];
    EInstructionID insn;
    char dst_type[8];
    char src_type[8];
    char args[32];
    char fragment[64];
};

struct binary_info {
    char name[32];
    EInstructionID insn;
    char dst_type[8];
    char src1_type[8];
    char src2_type[8];
    char args[32];
    char fragment[64];
};

struct ternary_info {
    char name[32];
    EInstructionID insn;
    char dst_type[8];
    char src1_type[8];
    char src2_type[8];
    char src3_type[8];
    char args[32];
    char fragment[64];
};

struct func_info {
    char           name[32];
    EInstructionID insn;
    char           type[8];
    char           c_fragment[32];
    int            offset;
};

class Generator {
public:
    int Run();
    void used(EInstructionID insn);

private:
    static const char CHECK_LABEL[];
    static const char CHECK[];
    static const char CHECK_NEXT[];
    static const char CHECK_EMPTY[];

    void CreateLitLocalCfg(void);
    void CreateCommonHeader(void);
    void CreateGitIgnore(void);
    void StartFile(std::ostream& out, const char *params);

    void LoadGlobal(std::ostream& out, CodeModel cm);
    void StoreGlobal(std::ostream& out, CodeModel cm);
    void LoadStoreTLS(std::ostream& out, CodeModel cm, ThreadModel tm);

    void CompareRegRegAndBranchOrNullify(std::ostream& out, bool nullify);
    void CompareRegBitAndBranchOrNullify(std::ostream& out, bool nullify);
    void CompareFpRegRegAndBranchOrNullify(std::ostream& out, bool nullify);

    template<typename TYPE>
    void CompareImmediateAndBranchOrNullify(std::ostream& out, const std::span<const func_info> tests, bool nullify);

    template<typename ITYPE, typename UTYPE>
    void BinaryRegImm(std::ostream& out, const std::span<const func_info> tests);

    template<typename ITYPE, typename UTYPE>
    void BinaryRegImmCase(std::ostream& out, const func_info& test, const imm_data<ITYPE>& imm, const std::string& name);

    void Deposit(std::ostream& out, int size);

#define XXX(NAME, OPTIONS) void NAME(std::ostream& out);
    DECLARE_CODEGEN_HANDLERS(XXX)
#undef XXX

    std::array<bool, insn_last> m_used{};

};

#define CODEGEN_HANDLER(NAME) void Generator::NAME(std::ostream& out)

class InsnCounter {
public:
    InsnCounter(Generator* generator, EInstructionID insn) : m_insn(insn) {
         generator->used(m_insn);
    }

    friend std::ostream& operator<<(std::ostream& out, const InsnCounter& r) {
        return out << insn_name(r.m_insn);
    }

private:
    EInstructionID  m_insn;
};

} // namespace llvm
} // namespace postrisc
