#include "util/common.hpp"
#include "config/emulator_configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"

namespace postrisc {

template<class T, class OP> ALWAYS_INLINE CStatus Core::VectorCompareRegReg(void)
{
    CGeneralRegister& a = ra();
    CGeneralRegister& b = rb();
    CGeneralRegister& c = rc();

    for (size_t i=0; i < CGeneralRegister::size<T>(); i++) {
        T const res = OP::func(b.template vector<T>(i), c.template vector<T>(i));
        a.template vector<T>(i) = static_cast<T>(T(0) - res);
    }

    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::VectorBinaryRegReg(void)
{
    CGeneralRegister& a = ra();
    CGeneralRegister& b = rb();
    CGeneralRegister& c = rc();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i),
                                           c.template vector<T>(i));
    }

    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::VectorBinaryRegShamt(void)
{
    CGeneralRegister& a = ra();
    CGeneralRegister& b = rb();

    const T shamt = T(cmd.rc & (8*sizeof(T) - 1));

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<T>(i) = OP::func(b.template vector<T>(i), shamt);
    }

    return FinalizeInstruction();
}

template<class T> ALWAYS_INLINE CStatus Core::SimdMergeLow(void)
{
    CGeneralRegister& b = rb();
    CGeneralRegister& c = rc();

    CGeneralRegister fpr_tmp;
    const size_t n = CGeneralRegister::size<T>() / 2;
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<T>(2*i+0) = b.template vector<T>(i);
        fpr_tmp.template vector<T>(2*i+1) = c.template vector<T>(i);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

template<class T> ALWAYS_INLINE CStatus Core::SimdMergeHigh(void)
{
    CGeneralRegister& b = rb();
    CGeneralRegister& c = rc();

    CGeneralRegister   fpr_tmp;
    const size_t n = CGeneralRegister::size<T>() / 2;
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<T>(2*i+0) = b.template vector<T>(i + n);
        fpr_tmp.template vector<T>(2*i+1) = c.template vector<T>(i + n);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

#define MMX_VECTOR(NAME, STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpeq  ## NAME) { return core.VectorCompareRegReg<UTYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmplt  ## NAME) { return core.VectorCompareRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vcmpltu ## NAME) { return core.VectorCompareRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmins   ## NAME) { return core.VectorBinaryRegReg<STYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vminu   ## NAME) { return core.VectorBinaryRegReg<UTYPE, min>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxs   ## NAME) { return core.VectorBinaryRegReg<STYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmaxu   ## NAME) { return core.VectorBinaryRegReg<UTYPE, max>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsll    ## NAME) { return core.VectorBinaryRegReg<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrl    ## NAME) { return core.VectorBinaryRegReg<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsra    ## NAME) { return core.VectorBinaryRegReg<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vrol    ## NAME) { return core.VectorBinaryRegReg<UTYPE, rotate_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vror    ## NAME) { return core.VectorBinaryRegReg<UTYPE, rotate_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddu   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubu   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgu   ## NAME) { return core.VectorBinaryRegReg<UTYPE, average_unsigned>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vavgs   ## NAME) { return core.VectorBinaryRegReg<STYPE, average_signed>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddo   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubo   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_overflow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddc   ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_carry>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubb   ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_borrow>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddus  ## NAME) { return core.VectorBinaryRegReg<UTYPE, add_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubus  ## NAME) { return core.VectorBinaryRegReg<UTYPE, subtract_unsigned_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vaddss  ## NAME) { return core.VectorBinaryRegReg<STYPE, add_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsubss  ## NAME) { return core.VectorBinaryRegReg<STYPE, subtract_signed_saturated>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vslli   ## NAME) { return core.VectorBinaryRegShamt<UTYPE, shift_left>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrli   ## NAME) { return core.VectorBinaryRegShamt<UTYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vsrai   ## NAME) { return core.VectorBinaryRegShamt<STYPE, shift_right>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmrgl   ## NAME) { return core.SimdMergeLow<UTYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(vmrgh   ## NAME) { return core.SimdMergeHigh<UTYPE>(); } \

MMX_VECTOR(b,  i8,  u8)
MMX_VECTOR(h, i16, u16)
MMX_VECTOR(w, i32, u32)
MMX_VECTOR(d, i64, u64)

template<class T, class WT> ALWAYS_INLINE CStatus Core::SimdUnpackLow(void)
{
    CGeneralRegister& b = rb();

    CGeneralRegister fpr_tmp;
    const size_t n = b.template size<WT>();
    for (size_t i=0; i < n; ++i) {
        fpr_tmp.template vector<WT>(i) = b.template vector<T>(i);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

template<class T, class WT> ALWAYS_INLINE CStatus Core::SimdUnpackHigh(void)
{
    CGeneralRegister& b = rb();

    CGeneralRegister fpr_tmp;
    const size_t n = fpr_tmp.template size<WT>();
    for (size_t i = 0; i < n; ++i) {
        fpr_tmp.template vector<WT>(i) = b.template vector<T>(i + n);
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsb) { return core.SimdUnpackLow< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsh) { return core.SimdUnpackLow< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupklsw) { return core.SimdUnpackLow< i32, i64>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsb) { return core.SimdUnpackHigh< i8,  i16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsh) { return core.SimdUnpackHigh< i16, i32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vupkhsw) { return core.SimdUnpackHigh< i32, i64>(); }


template<class T, class WT, class OP> ALWAYS_INLINE CStatus Core::VectorPack(void)
{
    CGeneralRegister& b = rb();
    CGeneralRegister& c = rc();

    CGeneralRegister fpr_tmp;
    const size_t n = CGeneralRegister::size<WT>();

    for (size_t i = 0; i < n; ++i) {
        fpr_tmp.template vector<T>(i  ) = static_cast<T>(OP::func( b.template vector<WT>(i) ));
        fpr_tmp.template vector<T>(i+n) = static_cast<T>(OP::func( c.template vector<WT>(i) ));
    }
    ra() = fpr_tmp;
    return FinalizeInstruction();
}

// pack elements from 2 vectors into one
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuumh) { return core.VectorPack<u8,  u16, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuumw) { return core.VectorPack<u16, u32, saturate_modulo>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuumd) { return core.VectorPack<u32, u64, saturate_modulo>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuush) { return core.VectorPack<u8,  u16, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuusw) { return core.VectorPack<u16, u32, saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpkuusd) { return core.VectorPack<u32, u64, saturate_unsigned>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksssh) { return core.VectorPack<i8,  i16, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksssw) { return core.VectorPack<i16, i32, saturate_signed>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksssd) { return core.VectorPack<i32, i64, saturate_signed>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksush) { return core.VectorPack<u8,  i16, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksusw) { return core.VectorPack<u16, i32, signed_saturate_unsigned>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vpksusd) { return core.VectorPack<u32, i64, signed_saturate_unsigned>(); }

} // namespace postrisc
