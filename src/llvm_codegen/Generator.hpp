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
    X(Load, "") \
    X(MinMax, "") \
    X(MinMaxImm32, "") \
    X(MinMaxImm64, "") \
    X(Store, "") \
    X(StoreZero, "") \

namespace postrisc {
namespace llvm {

struct func_info {
    char name[32];
    EInstructionID insn;
    char dst_type[8];
    char src_type[8];
    char args[32];
    char fragment[64];
};

class Generator {
public:
    int Run();

private:
    void CreateLitLocalCfg(void);
    void CreateCommonHeader(void);
    void CreateGitIgnore(void);
    void StartFile(std::ostream& out, const char *params);

#define XXX(NAME, OPTIONS) static void NAME(std::ostream& out);
    DECLARE_CODEGEN_HANDLERS(XXX)
#undef XXX
};

#define CODEGEN_HANDLER(NAME) void Generator::NAME(std::ostream& out)

} // namespace llvm
} // namespace postrisc
