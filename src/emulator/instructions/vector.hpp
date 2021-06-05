
#include "core.hpp"

namespace postrisc {

template<class T, class OP, class CMD>
inline
CStatus
Core::VectorCompareRegReg(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC; 

          T * aa = a.template vector<T>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); i++) {
        aa[i] = static_cast<T>(T(0) - T(OP::func(bb[i], cc[i])));
    }

    return FinalizeInstruction();
}

// packed integer compare
DEFINE_INSN_IMPLEMENTATION(vcmpeq1) {   return VectorCompareRegReg< uint8_t,  cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpeq2) {   return VectorCompareRegReg< uint16_t, cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpeq4) {   return VectorCompareRegReg< uint32_t, cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpeq8) {   return VectorCompareRegReg< uint64_t, cmp_eq> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vcmplt1) {   return VectorCompareRegReg< int8_t,   cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmplt2) {   return VectorCompareRegReg< int16_t,  cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmplt4) {   return VectorCompareRegReg< int32_t,  cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmplt8) {   return VectorCompareRegReg< int64_t,  cmp_lt> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vcmpltu1) {  return VectorCompareRegReg< uint8_t,  cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpltu2) {  return VectorCompareRegReg< uint16_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpltu4) {  return VectorCompareRegReg< uint32_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vcmpltu8) {  return VectorCompareRegReg< uint64_t, cmp_lt> (cmd); }

template<class T, class OP, class CMD>
inline
CStatus
Core::VectorBinaryRegReg(const CMD cmd)
{
    GET_WRA;
    GET_RRB; 
    GET_RRC; 

          T * aa = a.template vector<T>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP::func(bb[i], cc[i]);
    }

    return FinalizeInstruction();
}

// packed integer minimum/maximum
DEFINE_INSN_IMPLEMENTATION(vmins1) {    return VectorBinaryRegReg< int8_t,  min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmins2) {    return VectorBinaryRegReg< int16_t, min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmins4) {    return VectorBinaryRegReg< int32_t, min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmins8) {    return VectorBinaryRegReg< int64_t, min> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vminu1) {    return VectorBinaryRegReg< uint8_t,  min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vminu2) {    return VectorBinaryRegReg< uint16_t, min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vminu4) {    return VectorBinaryRegReg< uint32_t, min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vminu8) {    return VectorBinaryRegReg< uint64_t, min> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vmaxs1) {    return VectorBinaryRegReg< int8_t,  max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxs2) {    return VectorBinaryRegReg< int16_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxs4) {    return VectorBinaryRegReg< int32_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxs8) {    return VectorBinaryRegReg< int64_t, max> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vmaxu1) {    return VectorBinaryRegReg< uint8_t,  max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxu2) {    return VectorBinaryRegReg< uint16_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxu4) {    return VectorBinaryRegReg< uint32_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmaxu8) {    return VectorBinaryRegReg< uint64_t, max> (cmd); }

// packed integer shifts
DEFINE_INSN_IMPLEMENTATION(vsll1) {     return VectorBinaryRegReg<uint8_t,  shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsll2) {     return VectorBinaryRegReg<uint16_t, shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsll4) {     return VectorBinaryRegReg<uint32_t, shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsll8) {     return VectorBinaryRegReg<uint64_t, shift_left> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsrl1) {     return VectorBinaryRegReg<uint8_t,  shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrl2) {     return VectorBinaryRegReg<uint16_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrl4) {     return VectorBinaryRegReg<uint32_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrl8) {     return VectorBinaryRegReg<uint64_t, shift_right> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsra1) {     return VectorBinaryRegReg< int8_t,  shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsra2) {     return VectorBinaryRegReg< int16_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsra4) {     return VectorBinaryRegReg< int32_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsra8) {     return VectorBinaryRegReg< int64_t, shift_right> (cmd); }

// rotates (variable amount)
DEFINE_INSN_IMPLEMENTATION(vrol1) {     return VectorBinaryRegReg<uint8_t,  rotate_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vrol2) {     return VectorBinaryRegReg<uint16_t, rotate_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vrol4) {     return VectorBinaryRegReg<uint32_t, rotate_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vrol8) {     return VectorBinaryRegReg<uint64_t, rotate_left> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vror1) {     return VectorBinaryRegReg<uint8_t,  rotate_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vror2) {     return VectorBinaryRegReg<uint16_t, rotate_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vror4) {     return VectorBinaryRegReg<uint32_t, rotate_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vror8) {     return VectorBinaryRegReg<uint64_t, rotate_right> (cmd); }

// vector integer arithmetic
DEFINE_INSN_IMPLEMENTATION(vaddu1) {    return VectorBinaryRegReg<uint8_t,  add> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddu2) {    return VectorBinaryRegReg<uint16_t, add> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddu4) {    return VectorBinaryRegReg<uint32_t, add> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddu8) {    return VectorBinaryRegReg<uint64_t, add> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsubu1) {    return VectorBinaryRegReg<uint8_t,  subtract> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubu2) {    return VectorBinaryRegReg<uint16_t, subtract> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubu4) {    return VectorBinaryRegReg<uint32_t, subtract> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubu8) {    return VectorBinaryRegReg<uint64_t, subtract> (cmd); }

// averages (a+b+1)/2
DEFINE_INSN_IMPLEMENTATION(vavgu1) {    return VectorBinaryRegReg<uint8_t,  average_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgu2) {    return VectorBinaryRegReg<uint16_t, average_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgu4) {    return VectorBinaryRegReg<uint32_t, average_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgu8) {    return VectorBinaryRegReg<uint64_t, average_unsigned> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vavgs1) {    return VectorBinaryRegReg< int8_t,  average_signed> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgs2) {    return VectorBinaryRegReg< int16_t, average_signed> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgs4) {    return VectorBinaryRegReg< int32_t, average_signed> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vavgs8) {    return VectorBinaryRegReg< int64_t, average_signed> (cmd); }

// carry/borrow/overflow
DEFINE_INSN_IMPLEMENTATION(vaddo1) {    return VectorBinaryRegReg<uint8_t,  add_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddo2) {    return VectorBinaryRegReg<uint16_t, add_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddo4) {    return VectorBinaryRegReg<uint32_t, add_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddo8) {    return VectorBinaryRegReg<uint64_t, add_overflow> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsubo1) {    return VectorBinaryRegReg<uint8_t,  subtract_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubo2) {    return VectorBinaryRegReg<uint16_t, subtract_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubo4) {    return VectorBinaryRegReg<uint32_t, subtract_overflow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubo8) {    return VectorBinaryRegReg<uint64_t, subtract_overflow> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vaddc1) {    return VectorBinaryRegReg<uint8_t,  add_carry> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddc2) {    return VectorBinaryRegReg<uint16_t, add_carry> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddc4) {    return VectorBinaryRegReg<uint32_t, add_carry> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddc8) {    return VectorBinaryRegReg<uint64_t, add_carry> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsubb1) {    return VectorBinaryRegReg<uint8_t,  subtract_borrow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubb2) {    return VectorBinaryRegReg<uint16_t, subtract_borrow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubb4) {    return VectorBinaryRegReg<uint32_t, subtract_borrow> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubb8) {    return VectorBinaryRegReg<uint64_t, subtract_borrow> (cmd); }

// saturated add/subtract
DEFINE_INSN_IMPLEMENTATION(vaddus1) {   return VectorBinaryRegReg<uint8_t,  add_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddus2) {   return VectorBinaryRegReg<uint16_t, add_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddus4) {   return VectorBinaryRegReg<uint32_t, add_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddus8) {   return VectorBinaryRegReg<uint64_t, add_unsigned_saturated> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsubus1) {   return VectorBinaryRegReg<uint8_t,  subtract_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubus2) {   return VectorBinaryRegReg<uint16_t, subtract_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubus4) {   return VectorBinaryRegReg<uint32_t, subtract_unsigned_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubus8) {   return VectorBinaryRegReg<uint64_t, subtract_unsigned_saturated> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vaddss1) {   return VectorBinaryRegReg< int8_t,  add_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddss2) {   return VectorBinaryRegReg< int16_t, add_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddss4) {   return VectorBinaryRegReg< int32_t, add_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vaddss8) {   return VectorBinaryRegReg< int64_t, add_signed_saturated> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsubss1) {   return VectorBinaryRegReg< int8_t,  subtract_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubss2) {   return VectorBinaryRegReg< int16_t, subtract_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubss4) {   return VectorBinaryRegReg< int32_t, subtract_signed_saturated> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsubss8) {   return VectorBinaryRegReg< int64_t, subtract_signed_saturated> (cmd); }


template<class T, class OP, class CMD>
inline
CStatus
Core::VectorBinaryRegShamt(const CMD cmd)
{
    GET_WRA;
    GET_RRB; 

    const T shamt = T(cmd.rc() & (8*sizeof(T) - 1));

          T * aa = a.template vector<T>();
    const T * bb = b.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP::func(bb[i], shamt);
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(vslli1) {    return VectorBinaryRegShamt< uint8_t,  shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vslli2) {    return VectorBinaryRegShamt< uint16_t, shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vslli4) {    return VectorBinaryRegShamt< uint32_t, shift_left> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vslli8) {    return VectorBinaryRegShamt< uint64_t, shift_left> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsrli1) {    return VectorBinaryRegShamt< uint8_t,  shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrli2) {    return VectorBinaryRegShamt< uint16_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrli4) {    return VectorBinaryRegShamt< uint32_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrli8) {    return VectorBinaryRegShamt< uint64_t, shift_right> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vsrai1) {    return VectorBinaryRegShamt< int8_t,  shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrai2) {    return VectorBinaryRegShamt< int16_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrai4) {    return VectorBinaryRegShamt< int32_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vsrai8) {    return VectorBinaryRegShamt< int64_t, shift_right> (cmd); }

template<class T, class CMD>
inline
CStatus
Core::SimdMergeLow(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister   fpr_tmp;
    const size_t n = CGeneralRegister::size<T>() / 2;
    T * tmp = fpr_tmp.template vector<T>();

    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();
    for (size_t i=0; i < n; ++i) {
        tmp[2*i+0] = bb[i];
        tmp[2*i+1] = cc[i];
    }
    a = fpr_tmp;
    return FinalizeInstruction();
}

// merge/unpack high/low parts
DEFINE_INSN_IMPLEMENTATION(vmrgl1) {    return SimdMergeLow<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgl2) {    return SimdMergeLow<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgl4) {    return SimdMergeLow<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgl8) {    return SimdMergeLow<uint64_t> (cmd); }

template<class T, class CMD>
inline
CStatus
Core::SimdMergeHigh(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister   fpr_tmp;
    const size_t n = CGeneralRegister::size<T>() / 2;
    T * tmp = fpr_tmp.template vector<T>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();
    for (size_t i=0; i < n; ++i) {
        tmp[2*i+0] = bb[i + n];
        tmp[2*i+1] = cc[i + n];
    }
    a = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(vmrgh1) {    return SimdMergeHigh<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgh2) {    return SimdMergeHigh<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgh4) {    return SimdMergeHigh<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vmrgh8) {    return SimdMergeHigh<uint64_t> (cmd); }


template<class T, class WT, class CMD>
inline
CStatus
Core::SimdUnpackLow(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    CGeneralRegister   fpr_tmp;
    const size_t n = gpr[0].template size<WT>();
    WT * tmp = fpr_tmp.template vector<WT>();
    const T * bb = b.template vector<T>();
    for (size_t i=0; i < n; ++i) {
        tmp[i] = bb[i];
    }
    a = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(vupkls1) {   return SimdUnpackLow< int8_t,  int16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vupkls2) {   return SimdUnpackLow< int16_t, int32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vupkls4) {   return SimdUnpackLow< int32_t, int64_t> (cmd); }


template<class T, class WT, class CMD>
inline
CStatus
Core::SimdUnpackHigh(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    CGeneralRegister   fpr_tmp;
    const size_t n = gpr[0].template size<WT>();
    WT * tmp = fpr_tmp.template vector<WT>();
    const T * bb = b.template vector<T>();
    for (size_t i=0; i < n; ++i) {
        tmp[i] = bb[i + n];
    }
    a = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(vupkhs1) {   return SimdUnpackHigh< int8_t,  int16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vupkhs2) {   return SimdUnpackHigh< int16_t, int32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vupkhs4) {   return SimdUnpackHigh< int32_t, int64_t> (cmd); }


template<class T, class WT, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorPack(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister   fpr_tmp;
    T * tmp = fpr_tmp.template vector<T>();
    const WT * bb = b.template vector<WT>();
    const WT * cc = c.template vector<WT>();

    const size_t n = CGeneralRegister::size<WT>();

    for (size_t i=0; i < n; ++i) {
        tmp[i  ] = static_cast<T>(OP::func( bb[i] ));
        tmp[i+n] = static_cast<T>(OP::func( cc[i] ));
    }
    a = fpr_tmp;
    return FinalizeInstruction();
}

// pack elements from 2 vectors into one
DEFINE_INSN_IMPLEMENTATION(vpkuum2) {   return VectorPack<uint8_t,  uint16_t, saturate_modulo> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpkuum4) {   return VectorPack<uint16_t, uint32_t, saturate_modulo> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpkuum8) {   return VectorPack<uint32_t, uint64_t, saturate_modulo> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vpkuus2) {   return VectorPack<uint8_t,  uint16_t, saturate_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpkuus4) {   return VectorPack<uint16_t, uint32_t, saturate_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpkuus8) {   return VectorPack<uint32_t, uint64_t, saturate_unsigned> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vpksss2) {   return VectorPack<int8_t,  int16_t, saturate_signed> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpksss4) {   return VectorPack<int16_t, int32_t, saturate_signed> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpksss8) {   return VectorPack<int32_t, int64_t, saturate_signed> (cmd); }

DEFINE_INSN_IMPLEMENTATION(vpksus2) {   return VectorPack<uint8_t,  int16_t, signed_saturate_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpksus4) {   return VectorPack<uint16_t, int32_t, signed_saturate_unsigned> (cmd); }
DEFINE_INSN_IMPLEMENTATION(vpksus8) {   return VectorPack<uint32_t, int64_t, signed_saturate_unsigned> (cmd); }

} // namespace postrisc

