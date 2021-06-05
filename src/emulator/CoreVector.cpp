#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

template<typename T, typename OP> ALWAYS_INLINE Result Core::VectorCompareRegReg(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    for (size_t i=0; i < GeneralRegister::size<T>(); i++) {
        T const res = OP::func(b.template vector<T>(i), c.template vector<T>(i));
        a.template vector<T>(i) = static_cast<T>(T(0) - res);
    }

    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::VectorBinaryRegReg(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i),
                                           c.template vector<T>(i));
    }

    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::VectorBinaryRegShamt(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();

    const T shamt = T(cmd.rc & (8*sizeof(T) - 1));

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i), shamt);
    }

    return FinalizeInstruction();
}

template<typename T> ALWAYS_INLINE Result Core::VectorSplat(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    const size_t index = c.scalar<u32>() % (sizeof(GeneralRegister) / sizeof(T));
    const T value = b.template vector<T>(index);

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = value;
    }

    return FinalizeInstruction();
}

template<typename T> ALWAYS_INLINE Result Core::VectorSplatImmediate(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();

    const size_t index = cmd.rc % (sizeof(GeneralRegister) / sizeof(T));
    const T value = b.template vector<T>(index);

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = value;
    }

    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::VectorTernaryRegShamt(void)
{
    GeneralRegister& a = ra();
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    const T shamt = T(cmd.rc & (8*sizeof(T) - 1));

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i), c.template vector<T>(i), shamt);
    }

    return FinalizeInstruction();
}

template<typename T> ALWAYS_INLINE Result Core::SimdMergeLow(void)
{
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    GeneralRegister fpr_tmp;
    const size_t n = GeneralRegister::size<T>() / 2;
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<T>(2*i+0) = b.template vector<T>(i);
        fpr_tmp.template vector<T>(2*i+1) = c.template vector<T>(i);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

template<typename T> ALWAYS_INLINE Result Core::SimdMergeHigh(void)
{
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    GeneralRegister   fpr_tmp;
    const size_t n = GeneralRegister::size<T>() / 2;
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<T>(2*i+0) = b.template vector<T>(i + n);
        fpr_tmp.template vector<T>(2*i+1) = c.template vector<T>(i + n);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpeqb) { return core.VectorCompareRegReg<u8, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltsb) { return core.VectorCompareRegReg<i8, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltub) { return core.VectorCompareRegReg<u8, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminsb) { return core.VectorBinaryRegReg<i8, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminub) { return core.VectorBinaryRegReg<u8, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxsb) { return core.VectorBinaryRegReg<i8, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxub) { return core.VectorBinaryRegReg<u8, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllb) { return core.VectorBinaryRegReg<u8, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlb) { return core.VectorBinaryRegReg<u8, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrab) { return core.VectorBinaryRegReg<i8, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddb) { return core.VectorBinaryRegReg<u8, add>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubb) { return core.VectorBinaryRegReg<u8, subtract>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgub) { return core.VectorBinaryRegReg<u8, average_unsigned_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgsb) { return core.VectorBinaryRegReg<i8, average_signed_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddob) { return core.VectorBinaryRegReg<u8, add_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubob) { return core.VectorBinaryRegReg<u8, subtract_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddcb) { return core.VectorBinaryRegReg<u8, add_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubcb) { return core.VectorBinaryRegReg<u8, subtract_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddusatb) { return core.VectorBinaryRegReg<u8, add_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubusatb) { return core.VectorBinaryRegReg<u8, subtract_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddssatb) { return core.VectorBinaryRegReg<i8, add_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubssatb) { return core.VectorBinaryRegReg<i8, subtract_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllib) { return core.VectorBinaryRegShamt<u8, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlib) { return core.VectorBinaryRegShamt<u8, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsraib) { return core.VectorBinaryRegShamt<i8, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpib) { return core.VectorTernaryRegShamt<u8, shift_right_pair_imm>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergelb) { return core.SimdMergeLow<u8>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergehb) { return core.SimdMergeHigh<u8>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatb) { return core.VectorSplat<u8>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatib) { return core.VectorSplatImmediate<u8>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpeqh) { return core.VectorCompareRegReg<u16, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltsh) { return core.VectorCompareRegReg<i16, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltuh) { return core.VectorCompareRegReg<u16, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminsh) { return core.VectorBinaryRegReg<i16, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminuh) { return core.VectorBinaryRegReg<u16, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxsh) { return core.VectorBinaryRegReg<i16, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxuh) { return core.VectorBinaryRegReg<u16, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllh) { return core.VectorBinaryRegReg<u16, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlh) { return core.VectorBinaryRegReg<u16, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrah) { return core.VectorBinaryRegReg<i16, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddh) { return core.VectorBinaryRegReg<u16, add>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubh) { return core.VectorBinaryRegReg<u16, subtract>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavguh) { return core.VectorBinaryRegReg<u16, average_unsigned_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgsh) { return core.VectorBinaryRegReg<i16, average_signed_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddoh) { return core.VectorBinaryRegReg<u16, add_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsuboh) { return core.VectorBinaryRegReg<u16, subtract_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddch) { return core.VectorBinaryRegReg<u16, add_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubch) { return core.VectorBinaryRegReg<u16, subtract_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddusath) { return core.VectorBinaryRegReg<u16, add_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubusath) { return core.VectorBinaryRegReg<u16, subtract_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddssath) { return core.VectorBinaryRegReg<i16, add_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubssath) { return core.VectorBinaryRegReg<i16, subtract_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllih) { return core.VectorBinaryRegShamt<u16, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlih) { return core.VectorBinaryRegShamt<u16, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsraih) { return core.VectorBinaryRegShamt<i16, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpih) { return core.VectorTernaryRegShamt<u16, shift_right_pair_imm>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergelh) { return core.SimdMergeLow<u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergehh) { return core.SimdMergeHigh<u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplath) { return core.VectorSplat<u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatih) { return core.VectorSplatImmediate<u16>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpeqw) { return core.VectorCompareRegReg<u32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltsw) { return core.VectorCompareRegReg<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltuw) { return core.VectorCompareRegReg<u32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminsw) { return core.VectorBinaryRegReg<i32, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminuw) { return core.VectorBinaryRegReg<u32, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxsw) { return core.VectorBinaryRegReg<i32, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxuw) { return core.VectorBinaryRegReg<u32, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllw) { return core.VectorBinaryRegReg<u32, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlw) { return core.VectorBinaryRegReg<u32, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsraw) { return core.VectorBinaryRegReg<i32, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddw) { return core.VectorBinaryRegReg<u32, add>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubw) { return core.VectorBinaryRegReg<u32, subtract>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavguw) { return core.VectorBinaryRegReg<u32, average_unsigned_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgsw) { return core.VectorBinaryRegReg<i32, average_signed_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddow) { return core.VectorBinaryRegReg<u32, add_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubow) { return core.VectorBinaryRegReg<u32, subtract_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddcw) { return core.VectorBinaryRegReg<u32, add_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubcw) { return core.VectorBinaryRegReg<u32, subtract_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddusatw) { return core.VectorBinaryRegReg<u32, add_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubusatw) { return core.VectorBinaryRegReg<u32, subtract_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddssatw) { return core.VectorBinaryRegReg<i32, add_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubssatw) { return core.VectorBinaryRegReg<i32, subtract_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vslliw) { return core.VectorBinaryRegShamt<u32, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrliw) { return core.VectorBinaryRegShamt<u32, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsraiw) { return core.VectorBinaryRegShamt<i32, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpiw) { return core.VectorTernaryRegShamt<u32, shift_right_pair_imm>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergelw) { return core.SimdMergeLow<u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergehw) { return core.SimdMergeHigh<u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatw) { return core.VectorSplat<u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatiw) { return core.VectorSplatImmediate<u32>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpeqd) { return core.VectorCompareRegReg<u64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltsd) { return core.VectorCompareRegReg<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltud) { return core.VectorCompareRegReg<u64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminsd) { return core.VectorBinaryRegReg<i64, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vminud) { return core.VectorBinaryRegReg<u64, min>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxsd) { return core.VectorBinaryRegReg<i64, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxud) { return core.VectorBinaryRegReg<u64, max>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vslld) { return core.VectorBinaryRegReg<u64, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrld) { return core.VectorBinaryRegReg<u64, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrad) { return core.VectorBinaryRegReg<i64, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddd) { return core.VectorBinaryRegReg<u64, add>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubd) { return core.VectorBinaryRegReg<u64, subtract>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgud) { return core.VectorBinaryRegReg<u64, average_unsigned_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgsd) { return core.VectorBinaryRegReg<i64, average_signed_ceil>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddod) { return core.VectorBinaryRegReg<u64, add_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubod) { return core.VectorBinaryRegReg<u64, subtract_overflow>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddcd) { return core.VectorBinaryRegReg<u64, add_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubcd) { return core.VectorBinaryRegReg<u64, subtract_carry>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddusatd) { return core.VectorBinaryRegReg<u64, add_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubusatd) { return core.VectorBinaryRegReg<u64, subtract_unsigned_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddssatd) { return core.VectorBinaryRegReg<i64, add_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubssatd) { return core.VectorBinaryRegReg<i64, subtract_signed_saturated>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsllid) { return core.VectorBinaryRegShamt<u64, shift_left>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrlid) { return core.VectorBinaryRegShamt<u64, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsraid) { return core.VectorBinaryRegShamt<i64, shift_right>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpid) { return core.VectorTernaryRegShamt<u64, shift_right_pair_imm>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergeld) { return core.SimdMergeLow<u64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vmergehd) { return core.SimdMergeHigh<u64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatd) { return core.VectorSplat<u64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsplatid) { return core.VectorSplatImmediate<u64>(); }

template<typename T, typename OP> ALWAYS_INLINE Result Core::VectorTernaryRegRegReg(void)
{
          GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    const GeneralRegister& d = rd();

    for (size_t i=0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i), c.template vector<T>(i),  d.template vector<T>(i));
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(vslpb) { return core.VectorTernaryRegRegReg< u8, shift_left_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vslph) { return core.VectorTernaryRegRegReg<u16, shift_left_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vslpw) { return core.VectorTernaryRegRegReg<u32, shift_left_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vslpd) { return core.VectorTernaryRegRegReg<u64, shift_left_pair>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpb) { return core.VectorTernaryRegRegReg< u8, shift_right_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrph) { return core.VectorTernaryRegRegReg<u16, shift_right_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpw) { return core.VectorTernaryRegRegReg<u32, shift_right_pair>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrpd) { return core.VectorTernaryRegRegReg<u64, shift_right_pair>(); }

template<typename T, typename WT> ALWAYS_INLINE Result Core::SimdUnpackLow(void)
{
    GeneralRegister& b = rb();

    GeneralRegister fpr_tmp;
    const size_t n = b.template size<WT>();
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<WT>(i) = b.template vector<T>(i);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

template<typename T, typename WT> ALWAYS_INLINE Result Core::SimdUnpackHigh(void)
{
    GeneralRegister& b = rb();

    GeneralRegister fpr_tmp;
    const size_t n = fpr_tmp.template size<WT>();
    for (size_t i = 0; i < n; ++i) {
        fpr_tmp.template vector<WT>(i) = b.template vector<T>(i + n);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsb) { return core.SimdUnpackLow< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklub) { return core.SimdUnpackLow< u8,  u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsh) { return core.SimdUnpackLow< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkluh) { return core.SimdUnpackLow< u16, u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsw) { return core.SimdUnpackLow< i32, i64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkluw) { return core.SimdUnpackLow< u32, u64>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsb) { return core.SimdUnpackHigh< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhub) { return core.SimdUnpackHigh< u8,  u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsh) { return core.SimdUnpackHigh< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhuh) { return core.SimdUnpackHigh< u16, u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsw) { return core.SimdUnpackHigh< i32, i64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhuw) { return core.SimdUnpackHigh< u32, u64>(); }

template<typename T, typename WT, typename OP> ALWAYS_INLINE Result Core::VectorPack(void)
{
    GeneralRegister& b = rb();
    GeneralRegister& c = rc();

    GeneralRegister fpr_tmp;
    const size_t n = GeneralRegister::size<WT>();

    for (size_t i = 0; i < n; ++i) {
        fpr_tmp.template vector<T>(i  ) = static_cast<T>(OP::func( b.template vector<WT>(i) ));
        fpr_tmp.template vector<T>(i+n) = static_cast<T>(OP::func( c.template vector<WT>(i) ));
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

// pack elements from 2 vectors into one
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkumodh) { return core.VectorPack<u8,  u16, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkumodw) { return core.VectorPack<u16, u32, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkumodd) { return core.VectorPack<u32, u64, saturate_modulo>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusatsh) { return core.VectorPack<u8,  u16, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusatsw) { return core.VectorPack<u16, u32, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusatsd) { return core.VectorPack<u32, u64, saturate_unsigned>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkssath) { return core.VectorPack<i8,  i16, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkssatw) { return core.VectorPack<i16, i32, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkssatd) { return core.VectorPack<i32, i64, saturate_signed>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusath) { return core.VectorPack<u8,  i16, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusatw) { return core.VectorPack<u16, i32, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkusatd) { return core.VectorPack<u32, i64, signed_saturate_unsigned>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpermb)
{
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();
    const GeneralRegister& d = core.rd();

    GeneralRegister tmp;

    for (size_t i = 0; i < b.size<u8>(); i++) {
        const unsigned n = d.template vector<u8>(i);
        const unsigned shift = n & (sizeof(tmp) - 1);
        if( (n & sizeof(tmp)) == 0)
            tmp.template vector<u8>(i) = b.template vector<u8>(shift);
        else
            tmp.template vector<u8>(i) = c.template vector<u8>(shift);
    }
    core.ra() = tmp;
    return core.FinalizeInstruction();
}

} // namespace postrisc
