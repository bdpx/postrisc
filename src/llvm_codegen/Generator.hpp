#pragma once

#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"
#include "immediates.hpp"

enum SwapMode { NONE, SWAP };

#define DECLARE_CODEGEN_HANDLERS(X) \
    X(AtomicFences, "") \
    X(AtomicLoads, "") \
    X(AtomicStores, "") \
    X(AtomicSwaps, "") \
    X(AtomicCompareSwaps, "") \
    X(AtomicLoadOps, "") \
    X(BitCast, "") \
    X(BitwiseRegReg, "") \
    X(BitwiseRegImm32, "") \
    X(BitwiseRegImm64, "") \
    X(BitwiseRegImm128, "") \
    X(BranchCompare, "") \
    X(BranchCompareImm32, "") \
    X(BranchCompareImm64, "") \
    X(BranchCompareImm128, "") \
    X(BranchCompareZero, "") \
    X(BranchOnBit, "") \
    X(BuiltinNoArgs, "") \
    X(BuiltinUnary, "") \
    X(BuiltinBinary, "") \
    X(BuiltinTernary, "") \
    X(ClMul, "") \
    X(CompareRegReg, "") \
    X(CompareImm32, "") \
    X(CompareImm64, "") \
    X(CompareImm128, "") \
    X(DividePow2, "") \
    X(FpBranchCompare, "-mllvm --enable-nullification=0") \
    X(FpMinMax, "") \
    X(FpUnary, "") \
    X(FpBinary, "") \
    X(FpTernary, "-Xclang -fnative-half-type") \
    X(FpToInt, "") \
    X(IntUnary, "") \
    X(IntBinary, "") \
    X(IntTernary, "") \
    X(IntRegReg, "") \
    X(IntRegImm32, "") \
    X(IntRegImm64, "") \
    X(IntRegImm128, "") \
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
    X(MinMax, "") \
    X(MinMaxImm32, "") \
    X(MinMaxImm64, "") \


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

class Generator {
public:
    int Run();

private:
    static const char CHECK_LABEL[];
    static const char CHECK[];
    static const char CHECK_NEXT[];
    static const char CHECK_EMPTY[];

    void CreateLitLocalCfg(void);
    void CreateCommonHeader(void);
    void CreateGitIgnore(void);
    void StartFile(std::ostream& out, const char *params);

    static void LoadGlobal(std::ostream& out, CodeModel cm);
    static void StoreGlobal(std::ostream& out, CodeModel cm);
    static void LoadStoreTLS(std::ostream& out, CodeModel cm, ThreadModel tm);

    template<typename ITYPE, typename UTYPE>
    static void IntRegImm(std::ostream& out, int size);

    template<typename ITYPE, typename UTYPE>
    static void BranchCompareImm(std::ostream& out, int size);

    template<typename ITYPE, typename UTYPE>
    static void MinMaxRegImm(std::ostream& out, int size);

    template<typename T, typename imm_op>
    static void MinMaxImm(std::ostream& out, const std::string& type, const imm_op& op, const imm_data<T>& imm, bool cmp_reg_first, bool choose_reg_first);

#define XXX(NAME, OPTIONS) static void NAME(std::ostream& out);
    DECLARE_CODEGEN_HANDLERS(XXX)
#undef XXX
};

#define CODEGEN_HANDLER(NAME) void Generator::NAME(std::ostream& out)

} // namespace llvm
} // namespace postrisc
