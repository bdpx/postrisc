#include "arch/isa.hpp"
#define SPECIALIZE_IMMEDIATES 1
#include "immediates.hpp"

namespace postrisc {
namespace llvm {

//====-------------------------------------------------------------=======//
// load immediates (28-64 bits)
//====-------------------------------------------------------------=======//
template<>
const std::vector<imm_data<u32>> immediates<u32>::load_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_call) - 1 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_call) },
        { LONG,  MaxUnsignedImm<u32>(bits_per_call) + 1 },
        { LONG,  MaxUnsignedImm<u32>(28) },
        // FIXME: disassembled as signed
        //{ LONG,  std::numeric_limits<u32>::max() / 2 + 1000 },
        //{ LONG,  std::numeric_limits<u32>::max() - 6 },
        //{ LONG,  std::numeric_limits<u32>::max() - 5 },
};

template<>
const std::vector<imm_data<i32>> immediates<i32>::load_immediates =
{
        { LONG,  std::numeric_limits<i32>::min() + 2 },
        { LONG,  std::numeric_limits<i32>::min() + 3 },
        { LONG,  std::numeric_limits<i32>::min() / 2 - 1000 },
        { LONG,  MinSignedImm<i32>(28) },
        { LONG,  MinSignedImm<i32>(bits_per_call) - 1 },
        { SHORT, MinSignedImm<i32>(bits_per_call) },
        { SHORT, MinSignedImm<i32>(bits_per_call) + 1 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i32>(bits_per_call) - 1 },
        { SHORT, MaxSignedImm<i32>(bits_per_call) },
        { LONG,  MaxSignedImm<i32>(bits_per_call) + 1 },
        { LONG,  MaxSignedImm<i32>(28) },
        { LONG,  std::numeric_limits<i32>::max() / 2 + 1000 },
        { LONG,  std::numeric_limits<i32>::max() - 3 },
        { LONG,  std::numeric_limits<i32>::max() - 2 },
};

template<>
const std::vector<imm_data<u64>> immediates<u64>::load_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_call) - 1 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_call) },
        { LONG,  MaxUnsignedImm<u64>(bits_per_call) + 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_call_long) - 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_call_long) },
};

template<>
const std::vector<imm_data<i64>> immediates<i64>::load_immediates =
{
        { LONG,  MinSignedImm<i64>(bits_per_call_long) },
        { LONG,  MinSignedImm<i64>(bits_per_call_long) + 1 },
        { LONG,  MinSignedImm<i64>(bits_per_call) - 1 },
        { SHORT, MinSignedImm<i64>(bits_per_call) },
        { SHORT, MinSignedImm<i64>(bits_per_call) + 1 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i64>(bits_per_call) - 1 },
        { SHORT, MaxSignedImm<i64>(bits_per_call) },
        { LONG,  MaxSignedImm<i64>(bits_per_call) + 1 },
        { LONG,  MaxSignedImm<i64>(bits_per_call_long) - 1 },
        { LONG,  MaxSignedImm<i64>(bits_per_call_long) },
};

//====-------------------------------------------------------------=======//
// binary immediates in reg-imm ops (21,63 bits)
//====-------------------------------------------------------------=======//
template<>
const std::vector<imm_data<u32>> immediates<u32>::binimm_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_binimm) - 1 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_binimm) },
        { LONG,  MaxUnsignedImm<u32>(bits_per_binimm) + 1 },
        { LONG,  MaxUnsignedImm<u32>(28) },
        // FIXME: disassembled as signed
        //{ LONG,  std::numeric_limits<u32>::max() / 2 + 1000 },
        //{ LONG,  std::numeric_limits<u32>::max() - 6 },
        //{ LONG,  std::numeric_limits<u32>::max() - 5 },
};

template<>
const std::vector<imm_data<i32>> immediates<i32>::binimm_immediates =
{
        { LONG,  std::numeric_limits<i32>::min() + 2 },
        { LONG,  std::numeric_limits<i32>::min() + 3 },
        { LONG,  std::numeric_limits<i32>::min() / 2 - 1000 },
        { LONG,  MinSignedImm<i32>(28) },
        { LONG,  MinSignedImm<i32>(bits_per_binimm) - 3 },
        { SHORT, MinSignedImm<i32>(bits_per_binimm) },
        { SHORT, MinSignedImm<i32>(bits_per_binimm) + 3 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i32>(bits_per_binimm) - 3 },
        { SHORT, MaxSignedImm<i32>(bits_per_binimm) },
        { LONG,  MaxSignedImm<i32>(bits_per_binimm) + 3 },
        { LONG,  MaxSignedImm<i32>(28) },
        { LONG,  std::numeric_limits<i32>::max() / 2 + 1000 },
        { LONG,  std::numeric_limits<i32>::max() - 3 },
        { LONG,  std::numeric_limits<i32>::max() - 2 },
};

template<>
const std::vector<imm_data<u64>> immediates<u64>::binimm_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_binimm) - 1 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_binimm) },
        { LONG,  MaxUnsignedImm<u64>(bits_per_binimm) + 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_binimm_long) - 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_binimm_long) },
};

template<>
const std::vector<imm_data<i64>> immediates<i64>::binimm_immediates =
{
        { LONG,  MinSignedImm<i64>(bits_per_binimm_long) },
        { LONG,  MinSignedImm<i64>(bits_per_binimm_long) + 3 },
        { LONG,  MinSignedImm<i64>(bits_per_binimm) - 3 },
        { SHORT, MinSignedImm<i64>(bits_per_binimm) },
        { SHORT, MinSignedImm<i64>(bits_per_binimm) + 3 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i64>(bits_per_binimm) - 3 },
        { SHORT, MaxSignedImm<i64>(bits_per_binimm) },
        { LONG,  MaxSignedImm<i64>(bits_per_binimm) + 3 },
        { LONG,  MaxSignedImm<i64>(bits_per_binimm_long) - 3 },
        { LONG,  MaxSignedImm<i64>(bits_per_binimm_long) },
};

//====-------------------------------------------------------------=======//
// branch immediates in compare-imm-and-branch (11,40 bits)
//====-------------------------------------------------------------=======//
template<>
const std::vector<imm_data<u32>> immediates<u32>::branch_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_brci_imm) - 1 },
        { SHORT, MaxUnsignedImm<u32>(bits_per_brci_imm) },
        { LONG,  MaxUnsignedImm<u32>(bits_per_brci_imm) + 1 },
        { LONG,  MaxUnsignedImm<u32>(28) },
        // FIXME: disassembled as signed
        //{ LONG,  std::numeric_limits<u32>::max() / 2 + 1000 },
        //{ LONG,  std::numeric_limits<u32>::max() - 6 },
        //{ LONG,  std::numeric_limits<u32>::max() - 5 },
};

template<>
const std::vector<imm_data<i32>> immediates<i32>::branch_immediates =
{
        { LONG,  std::numeric_limits<i32>::min() + 2 },
        { LONG,  std::numeric_limits<i32>::min() + 3 },
        { LONG,  std::numeric_limits<i32>::min() / 2 - 1000 },
        { LONG,  MinSignedImm<i32>(28) },
        { LONG,  MinSignedImm<i32>(bits_per_brci_imm) - 1 },
        { SHORT, MinSignedImm<i32>(bits_per_brci_imm) },
        { SHORT, MinSignedImm<i32>(bits_per_brci_imm) + 1 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i32>(bits_per_brci_imm) - 1 },
        { SHORT, MaxSignedImm<i32>(bits_per_brci_imm) },
        { LONG,  MaxSignedImm<i32>(bits_per_brci_imm) + 1 },
        { LONG,  MaxSignedImm<i32>(28) },
        { LONG,  std::numeric_limits<i32>::max() / 2 + 1000 },
        { LONG,  std::numeric_limits<i32>::max() - 3 },
        { LONG,  std::numeric_limits<i32>::max() - 2 },
};

template<>
const std::vector<imm_data<u64>> immediates<u64>::branch_immediates =
{
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_brci_imm) - 1 },
        { SHORT, MaxUnsignedImm<u64>(bits_per_brci_imm) },
        { LONG,  MaxUnsignedImm<u64>(bits_per_brci_imm) + 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_brci_imm_long) - 1 },
        { LONG,  MaxUnsignedImm<u64>(bits_per_brci_imm_long) },
};

template<>
const std::vector<imm_data<i64>> immediates<i64>::branch_immediates =
{
        { LONG,  MinSignedImm<i64>(bits_per_brci_imm_long) },
        { LONG,  MinSignedImm<i64>(bits_per_brci_imm_long) + 1 },
        { LONG,  MinSignedImm<i64>(bits_per_brci_imm) - 1 },
        { SHORT, MinSignedImm<i64>(bits_per_brci_imm) },
        { SHORT, MinSignedImm<i64>(bits_per_brci_imm) + 1 },
        { SHORT, -1000 },
        { SHORT, -10 },
        { SHORT, 10 },
        { SHORT, 1000 },
        { SHORT, MaxSignedImm<i64>(bits_per_brci_imm) - 1 },
        { SHORT, MaxSignedImm<i64>(bits_per_brci_imm) },
        { LONG,  MaxSignedImm<i64>(bits_per_brci_imm) + 1 },
        { LONG,  MaxSignedImm<i64>(bits_per_brci_imm_long) - 1 },
        { LONG,  MaxSignedImm<i64>(bits_per_brci_imm_long) },
};

} // namespace llvm
} // namespace postrisc
