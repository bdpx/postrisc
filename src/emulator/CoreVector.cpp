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

#define MMX_VECTOR(SNAME, UNAME, STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_eq_    ## SNAME) { return core.VectorCompareRegReg<UTYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_    ## SNAME) { return core.VectorCompareRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_    ## UNAME) { return core.VectorCompareRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_       ## SNAME) { return core.VectorBinaryRegReg<STYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_       ## SNAME) { return core.VectorBinaryRegReg<STYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sll_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(srl_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sra_       ## SNAME) { return core.VectorBinaryRegReg<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rol_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, rotate_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(ror_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, rotate_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, add>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, subtract>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(avg_       ## UNAME) { return core.VectorBinaryRegReg<UTYPE, average_unsigned>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(avg_       ## SNAME) { return core.VectorBinaryRegReg<STYPE, average_signed>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(addo_      ## SNAME) { return core.VectorBinaryRegReg<UTYPE, add_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(subo_      ## SNAME) { return core.VectorBinaryRegReg<UTYPE, subtract_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(addc_      ## UNAME) { return core.VectorBinaryRegReg<UTYPE, add_carry>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(subb_      ## UNAME) { return core.VectorBinaryRegReg<UTYPE, subtract_borrow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_sat_   ## UNAME) { return core.VectorBinaryRegReg<UTYPE, add_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_sat_   ## UNAME) { return core.VectorBinaryRegReg<UTYPE, subtract_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_sat_   ## SNAME) { return core.VectorBinaryRegReg<STYPE, add_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_sat_   ## SNAME) { return core.VectorBinaryRegReg<STYPE, subtract_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sll_imm_   ## UNAME) { return core.VectorBinaryRegShamt<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(srl_imm_   ## UNAME) { return core.VectorBinaryRegShamt<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sra_imm_   ## SNAME) { return core.VectorBinaryRegShamt<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_low_ ## UNAME) { return core.SimdMergeLow<UTYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_high_## UNAME) { return core.SimdMergeHigh<UTYPE>(); } \

MMX_VECTOR( vi8,  vu8,  i8,  u8)
MMX_VECTOR(vi16, vu16, i16, u16)
MMX_VECTOR(vi32, vu32, i32, u32)
MMX_VECTOR(vi64, vu64, i64, u64)

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vi8)   { return core.SimdUnpackLow< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vu8)   { return core.SimdUnpackLow< u8,  u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vi16)  { return core.SimdUnpackLow< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vu16)  { return core.SimdUnpackLow< u16, u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vi32)  { return core.SimdUnpackLow< i32, i64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_vu32)  { return core.SimdUnpackLow< u32, u64>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vi8)  { return core.SimdUnpackHigh< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vu8)  { return core.SimdUnpackHigh< u8,  u16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vi16) { return core.SimdUnpackHigh< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vu16) { return core.SimdUnpackHigh< u16, u32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vi32) { return core.SimdUnpackHigh< i32, i64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_vu32) { return core.SimdUnpackHigh< u32, u64>(); }

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
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_mod_vu16) { return core.VectorPack<u8,  u16, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_mod_vu32) { return core.VectorPack<u16, u32, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_mod_vu64) { return core.VectorPack<u32, u64, saturate_modulo>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_usat_vi16) { return core.VectorPack<u8,  u16, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_usat_vi32) { return core.VectorPack<u16, u32, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_usat_vi64) { return core.VectorPack<u32, u64, saturate_unsigned>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vi16) { return core.VectorPack<i8,  i16, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vi32) { return core.VectorPack<i16, i32, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vi64) { return core.VectorPack<i32, i64, saturate_signed>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vu16) { return core.VectorPack<u8,  i16, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vu32) { return core.VectorPack<u16, i32, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_sat_vu64) { return core.VectorPack<u32, i64, signed_saturate_unsigned>(); }

} // namespace postrisc
