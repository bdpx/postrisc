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

#define MMX_VECTOR(NAME, STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmp_eq_  ## NAME) { return core.VectorCompareRegReg<UTYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmp_lt_  ## NAME) { return core.VectorCompareRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmp_ltu_ ## NAME) { return core.VectorCompareRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmins_   ## NAME) { return core.VectorBinaryRegReg<STYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vminu_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxs_   ## NAME) { return core.VectorBinaryRegReg<STYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxu_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsll_    ## NAME) { return core.VectorBinaryRegReg<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrl_    ## NAME) { return core.VectorBinaryRegReg<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsra_    ## NAME) { return core.VectorBinaryRegReg<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vrol_    ## NAME) { return core.VectorBinaryRegReg<UTYPE, rotate_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vror_    ## NAME) { return core.VectorBinaryRegReg<UTYPE, rotate_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddu_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubu_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgu_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, average_unsigned>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgs_   ## NAME) { return core.VectorBinaryRegReg<STYPE, average_signed>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddo_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubo_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddc_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_carry>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubb_   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_borrow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddus_  ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubus_  ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddss_  ## NAME) { return core.VectorBinaryRegReg<STYPE, add_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubss_  ## NAME) { return core.VectorBinaryRegReg<STYPE, subtract_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vslli_   ## NAME) { return core.VectorBinaryRegShamt<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrli_   ## NAME) { return core.VectorBinaryRegShamt<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrai_   ## NAME) { return core.VectorBinaryRegShamt<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmrgl_   ## NAME) { return core.SimdMergeLow<UTYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmrgh_   ## NAME) { return core.SimdMergeHigh<UTYPE>(); } \

MMX_VECTOR(b,  i8,  u8)
MMX_VECTOR(h, i16, u16)
MMX_VECTOR(w, i32, u32)
MMX_VECTOR(d, i64, u64)

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkls_b) { return core.SimdUnpackLow< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkls_h) { return core.SimdUnpackLow< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkls_w) { return core.SimdUnpackLow< i32, i64>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhs_b) { return core.SimdUnpackHigh< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhs_h) { return core.SimdUnpackHigh< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhs_w) { return core.SimdUnpackHigh< i32, i64>(); }


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
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuum_h) { return core.VectorPack<u8,  u16, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuum_w) { return core.VectorPack<u16, u32, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuum_d) { return core.VectorPack<u32, u64, saturate_modulo>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuus_h) { return core.VectorPack<u8,  u16, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuus_w) { return core.VectorPack<u16, u32, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuus_d) { return core.VectorPack<u32, u64, saturate_unsigned>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksss_h) { return core.VectorPack<i8,  i16, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksss_w) { return core.VectorPack<i16, i32, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksss_d) { return core.VectorPack<i32, i64, saturate_signed>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksus_h) { return core.VectorPack<u8,  i16, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksus_w) { return core.VectorPack<u16, i32, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksus_d) { return core.VectorPack<u32, i64, signed_saturate_unsigned>(); }

} // namespace postrisc
