#include "util/common.hpp"
#include "config/emulator_configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"

namespace postrisc {

class fmin   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.min(a, b);               }};
class fmax   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.max(a, b);               }};
class fminnm { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.min(a, b);      }};
class fmaxnm { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.max(a, b);      }};
class famin  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.min(fpu.abs(a), fpu.abs(b));   }};
class famax  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.max(fpu.abs(a), fpu.abs(b));   }};
class fadd   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.add(a, b);                     }};
class fsub   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.sub(a, b);                     }};
class fnadd  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.chs(fpu.add(a, b));            }};
class fmul   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.mul(a, b);                     }};
class fdiv   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.div(a, b);                     }};
class fnmul  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.chs(fpu.mul(a, b));            }};
class fabsd  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.abs(fpu.sub(a, b));            }};
class fnabsd { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.nabs(fpu.sub(a, b));           }};

class fcmp_o   { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_ordered     (a, b); }};
class fcmp_oeq { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_ordered_eq  (a, b); }};
class fcmp_one { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_ordered_ne  (a, b); }};
class fcmp_olt { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_ordered_lt  (a, b); }};
class fcmp_ole { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_ordered_le  (a, b); }};
class fcmp_uo  { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_unordered   (a, b); }};
class fcmp_ueq { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_unordered_eq(a, b); }};
class fcmp_une { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_unordered_ne(a, b); }};
class fcmp_ult { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_unordered_lt(a, b); }};
class fcmp_ule { public: template<class T> static bool func(fpu::CSoftFPU& fpu, T a, T b) { return fpu.compare_unordered_le(a, b); }};

class fneg   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a) { return fpu.chs(a);   }};
class fabs   { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a) { return fpu.abs(a);   }};
class fnabs  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a) { return fpu.nabs(a);  }};
class fsqrt  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a) { return fpu.sqrt(a);  }};
class frsqrt { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a) { return fpu.rsqrt(a); }};

class frnd    { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.round_to_int<T>(a, rm); }};
class extend  { public: template<class D, class T> static D func(fpu::CSoftFPU& fpu, T a) { return fpu.extend<D,T>(a); }};
class cvt_f_f { public: template<class D, class T> static D func(fpu::CSoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_f_f<D,T>(a, rm); }};
class cvt_f_i { public: template<class D, class T> static D func(fpu::CSoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_f_i<D,T>(a, rm); }};
class cvt_i_f { public: template<class D, class T> static D func(fpu::CSoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_i_f<D,T>(a, rm); }};

class fmerge { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) { return fpu.merge(a, b, c); }};
class fmadd  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_NONE);    }};
class fmsub  { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_ADDEND);  }};
class fnmadd { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_PRODUCT); }};
class fnmsub { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_RESULT);  }};

class fadd_alt { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return { fpu.add(a.re, b.re), fpu.sub(a.im, b.im) };  }};
class fsub_alt { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b) { return { fpu.sub(a.re, b.re), fpu.add(a.im, b.im) };  }};

class fmadd_alt { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) {
 return { fpu.muladd(a.re, b.re, c.re, fpu::FLOAT_MULADD_NEGATE_NONE),
          fpu.muladd(a.im, b.im, c.im, fpu::FLOAT_MULADD_NEGATE_ADDEND) };   }};

class fmsub_alt { public: template<class T> static T func(fpu::CSoftFPU& fpu, T a, T b, T c) {
 return { fpu.muladd(a.re, b.re, c.re, fpu::FLOAT_MULADD_NEGATE_ADDEND),
          fpu.muladd(a.im, b.im, c.im, fpu::FLOAT_MULADD_NEGATE_NONE) };   }};

CStatus
Core::GetRoundingMode(unsigned roundmode, fpu::RoundingModeEnum& rm)
{
    switch (roundmode) {
    case fpu::RM_RNE:
    case fpu::RM_RTZ:
    case fpu::RM_RDN:
    case fpu::RM_RUP:
    case fpu::RM_RMM:
        rm = static_cast<fpu::RoundingModeEnum>(roundmode);
        return CStatus::continue_execution;
    case fpu::RM_DYN:
        rm = fpu.get_rounding_mode();
        return CStatus::continue_execution;
    }

    LOG_ERROR(LOG_EXCEPTION) << "Invalid rounding mode: " << roundmode;
    return CStatus::illegal_instruction;
}

/***************************************************************************
* templates
***************************************************************************/
template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::ScalarOp2(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::ScalarOp3(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::ScalarOp4(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>(),
                                     rd().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::FpScalarCompare(void)
{
    bool const condition = OP::template func<T>(fpu, rb().template scalar<T>(),
                                                     rc().template scalar<T>());
    ra() = condition;
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::ScalarOp2RoundingMode(void)
{
    fpu::RoundingModeEnum rm = fpu::RM_RNE;
    CHECK_STATUS(GetRoundingMode(cmd.rounding_mode, rm));
    ra() = OP:: template func<D>(fpu, rb().template scalar<T>(), rm);
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::VectorOp2(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister& a = ra();
    CGeneralRegister const & b = rb();
    for (size_t i = 0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D> ( fpu, b.template vector<T>(i) );
    }
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::VectorOp2RoundingMode(void)
{
    static_assert(sizeof(T) == sizeof(D));
    fpu::RoundingModeEnum rm = fpu::RM_RNE;
    CHECK_STATUS(GetRoundingMode(cmd.rounding_mode, rm));
    CGeneralRegister& a = ra();
    CGeneralRegister const & b = rb();
    for (size_t i = 0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D> ( fpu, b.template vector<T>(i), rm );
    }
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::VectorOp3(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister& a = ra();
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();
    for (size_t i = 0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP::template func<D>(fpu, b.template vector<T>(i), c.template vector<T>(i));
    }
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::VectorOp3Horizontal(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();

    CGeneralRegister tmp;

    for (size_t i = 0; i < CGeneralRegister::size<T>() / 2; ++i) {
        tmp.template vector<D>(i) =
            OP::template func<D>(fpu, b.template vector<T>(2*i+0),
                                      b.template vector<T>(2*i+1));
    }
    for (size_t i = 0; i < CGeneralRegister::size<T>() / 2; ++i) {
        tmp.template vector<D>(i + CGeneralRegister::size<T>() / 2) =
            OP::template func<D>(fpu, c.template vector<T>(2*i+0),
                                      c.template vector<T>(2*i+1));
    }

    ra() = tmp;
    return FpuFinalizeInstruction();
}

template<class D, class T> ALWAYS_INLINE CStatus Core::VectorOp3DotProduct(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();

    D tmp = fpu.mul(b.template vector<T>(0),
                    c.template vector<T>(0));
    for (size_t i = 1; i < CGeneralRegister::size<T>(); ++i) {
        tmp = fpu.add(tmp, fpu.mul(b.template vector<T>(i),
                                   c.template vector<T>(i)));
    }

    ra() = tmp;
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::VectorOp4(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister& a = ra();
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();
    CGeneralRegister const & d = rd();
    for (size_t i = 0; i < CGeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP:: template func<D> (fpu, b.template vector<T>(i), c.template vector<T>(i), d.template vector<T>(i));
    }
    return FpuFinalizeInstruction();
}

template<class D, class T, class OP> ALWAYS_INLINE CStatus Core::FpVectorCompare(void)
{
    static_assert(sizeof(T) == sizeof(D));
    CGeneralRegister& a = ra();
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();
    for (size_t i = 0; i < CGeneralRegister::size<T>(); ++i) {
        bool const condition = OP::template func<T>(fpu, b.template vector<T>(i), c.template vector<T>(i));
        a.template vector<D>(i) = condition;
    }
    return FpuFinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::FpBranchOnCompare(IAddressSpace& as)
{
    bool const condition = OP::template func<T>(fpu, ra().template scalar<T>(),
                                                     rb().template scalar<T>());
    if (condition) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::FpNullifyOnCompare(void)
{
    bool const condition = OP::template func<T>(fpu, ra().template scalar<T>(),
                                                     rb().template scalar<T>());
    return NullifyConditionally(condition);
}

template<class T> ALWAYS_INLINE CStatus Core::FpBranchOnClassify(IAddressSpace& as)
{
    unsigned const classify = cmd.rb;
    bool const condition = (fpu.classify(ra().template scalar<T>()) & classify) != 0;
    if (condition) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<class T> ALWAYS_INLINE CStatus Core::FpNullifyOnClassify(void)
{
    unsigned const classify = cmd.rb;
    bool const condition = (fpu.classify(ra().template scalar<T>()) & classify) != 0;
    return NullifyConditionally(condition);
}

template<class T> ALWAYS_INLINE CStatus Core::FpClassify(void)
{
    unsigned const classify_mask = cmd.rc;
    bool const condition = (fpu.classify(rb().template scalar<T>()) & classify_mask) != 0;
    ra() = condition;
    return FpuFinalizeInstruction();
}

/***************************************************************************
* unified f16/f32/f64/f128
***************************************************************************/

#define FP_BASE_SCALAR(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fneg    ## NAME         ) { return core.ScalarOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabs    ## NAME         ) { return core.ScalarOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabs   ## NAME         ) { return core.ScalarOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrt   ## NAME         ) { return core.ScalarOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrt  ## NAME         ) { return core.ScalarOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fadd    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsub    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnadd   ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmul    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmul   ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdiv    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsd   ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fabsd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsd  ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fnabsd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmin    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmax    ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnm  ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fminnm > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnm  ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, fmaxnm > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famin   ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famax   ## NAME         ) { return core.ScalarOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerge  ## NAME         ) { return core.ScalarOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadd   ## NAME         ) { return core.ScalarOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsub   ## NAME         ) { return core.ScalarOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmadd  ## NAME         ) { return core.ScalarOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsub  ## NAME         ) { return core.ScalarOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frnd    ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## oeq  ) { return core.FpScalarCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## one  ) { return core.FpScalarCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## olt  ) { return core.FpScalarCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ole  ) { return core.FpScalarCompare<TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ueq  ) { return core.FpScalarCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## une  ) { return core.FpScalarCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ult  ) { return core.FpScalarCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ule  ) { return core.FpScalarCompare<TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## o    ) { return core.FpScalarCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## uo   ) { return core.FpScalarCompare<TYPE, fcmp_uo >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcl     ## NAME         ) { return core.FpClassify<TYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtiw2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE,  i32, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtuw2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE,  u32, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtid2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE,  i64, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtud2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE,  u64, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtiq2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE, i128, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtuq2 ## NAME         ) { return core.ScalarOp2RoundingMode < TYPE, u128, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2iw  ) { return core.ScalarOp2RoundingMode <  i32, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2uw  ) { return core.ScalarOp2RoundingMode <  u32, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2id  ) { return core.ScalarOp2RoundingMode <  i64, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2ud  ) { return core.ScalarOp2RoundingMode <  u64, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2iq  ) { return core.ScalarOp2RoundingMode < i128, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2uq  ) { return core.ScalarOp2RoundingMode < u128, TYPE, cvt_f_i> (); } \

FP_BASE_SCALAR(sh, fpu::f16)
FP_BASE_SCALAR(ss, fpu::f32)
FP_BASE_SCALAR(sd, fpu::f64)
FP_BASE_SCALAR(sq, fpu::f128)

#define FP_BASE_BRANCH_SCALAR(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## oeq  ) { return core.FpBranchOnCompare<TYPE, fcmp_oeq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## one  ) { return core.FpBranchOnCompare<TYPE, fcmp_one>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## olt  ) { return core.FpBranchOnCompare<TYPE, fcmp_olt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## ole  ) { return core.FpBranchOnCompare<TYPE, fcmp_ole>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## ueq  ) { return core.FpBranchOnCompare<TYPE, fcmp_ueq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## une  ) { return core.FpBranchOnCompare<TYPE, fcmp_une>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## ult  ) { return core.FpBranchOnCompare<TYPE, fcmp_ult>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## ule  ) { return core.FpBranchOnCompare<TYPE, fcmp_ule>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## o    ) { return core.FpBranchOnCompare<TYPE, fcmp_o  >(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## uo   ) { return core.FpBranchOnCompare<TYPE, fcmp_uo >(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf      ## NAME ## class) { return core.FpBranchOnClassify<TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## class) { return core.FpNullifyOnClassify<TYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## oeq  ) { return core.FpNullifyOnCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## one  ) { return core.FpNullifyOnCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## olt  ) { return core.FpNullifyOnCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## ole  ) { return core.FpNullifyOnCompare<TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## ueq  ) { return core.FpNullifyOnCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## une  ) { return core.FpNullifyOnCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## ult  ) { return core.FpNullifyOnCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## ule  ) { return core.FpNullifyOnCompare<TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## o    ) { return core.FpNullifyOnCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf    ## NAME ## uo   ) { return core.FpNullifyOnCompare<TYPE, fcmp_uo >(); } \

FP_BASE_BRANCH_SCALAR(ss, fpu::f32)
FP_BASE_BRANCH_SCALAR(sd, fpu::f64)
FP_BASE_BRANCH_SCALAR(sq, fpu::f128)

#define FP_BASE_VECTOR(NAME, INAME, TYPE, INTTYPE, UINTTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fneg    ## NAME               ) { return core.VectorOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabs    ## NAME               ) { return core.VectorOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabs   ## NAME               ) { return core.VectorOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrt   ## NAME               ) { return core.VectorOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrt  ## NAME               ) { return core.VectorOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fadd    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsub    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnadd   ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmul    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdiv    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmul   ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsd   ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fabsd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsd  ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fnabsd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmin    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmax    ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famin   ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famax   ## NAME               ) { return core.VectorOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerge  ## NAME               ) { return core.VectorOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadd   ## NAME               ) { return core.VectorOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsub   ## NAME               ) { return core.VectorOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmadd  ## NAME               ) { return core.VectorOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsub  ## NAME               ) { return core.VectorOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadda  ## NAME               ) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsuba  ## NAME               ) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(faddc   ## NAME               ) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubc   ## NAME               ) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(faddh   ## NAME               ) { return core.VectorOp3Horizontal < TYPE, TYPE, fadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubh   ## NAME               ) { return core.VectorOp3Horizontal < TYPE, TYPE, fsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmulh   ## NAME               ) { return core.VectorOp3Horizontal < TYPE, TYPE, fmul > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdot    ## NAME               ) { return core.VectorOp3DotProduct < TYPE, TYPE> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## oeq        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## one        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## olt        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ole        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ueq        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## une        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ult        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## ule        ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## o          ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmp    ## NAME ## uo         ) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_uo >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmrgl   ## NAME               ) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmrgh   ## NAME               ) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(funpkh  ## NAME               ) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(funpkl  ## NAME               ) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fpk     ## NAME               ) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frnd    ## NAME               ) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2u ## INAME) { return core.VectorOp2RoundingMode < UINTTYPE, TYPE, cvt_f_i > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt    ## NAME ## 2i ## INAME) { return core.VectorOp2RoundingMode <  INTTYPE, TYPE, cvt_f_i > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvti   ## INAME ## 2 ## NAME ) { return core.VectorOp2RoundingMode < TYPE,  INTTYPE, cvt_i_f > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtu   ## INAME ## 2 ## NAME ) { return core.VectorOp2RoundingMode < TYPE, UINTTYPE, cvt_i_f > (); } \

FP_BASE_VECTOR(ph, h, fpu::f16, i16, u16)
FP_BASE_VECTOR(ps, w, fpu::f32, i32, u32)
FP_BASE_VECTOR(pd, d, fpu::f64, i64, u64)

/***************************************************************************
* f128 custom
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(fscale)
{
    const int scale = static_cast<int>(core.cmd.rc);
    core.ra() = core.fpu.scalbn(core.rb().sfq(), scale);
    return core.FpuFinalizeInstruction();
}

/***************************************************************************
* all-to-all
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtss2sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f32,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtsd2sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtsq2sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtsd2ss) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtsq2ss) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvtsq2sd) { return core.ScalarOp2RoundingMode<fpu::f64, fpu::f128, cvt_f_f>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fextsh2ss) { return core.ScalarOp2<fpu::f64,  fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fextsh2sd) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fextsh2sq) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fextss2sd) { return core.ScalarOp2<fpu::f64,  fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fextss2sq) { return core.ScalarOp2<fpu::f128, fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fextsd2sq) { return core.ScalarOp2<fpu::f128, fpu::f64, extend>(); }

} //namespace postrisc
