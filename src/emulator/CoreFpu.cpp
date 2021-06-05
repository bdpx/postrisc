#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

class fmin   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.min(a, b);               }};
class fmax   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.max(a, b);               }};
class fminnum{ public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.min(a, b);      }};
class fmaxnum{ public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.max(a, b);      }};
class famin  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.min(fpu.abs(a), fpu.abs(b));   }};
class famax  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.max(fpu.abs(a), fpu.abs(b));   }};
class fadd   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.add(a, b);                     }};
class fsub   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.sub(a, b);                     }};
class fnadd  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.chs(fpu.add(a, b));            }};
class fmul   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.mul(a, b);                     }};
class fdiv   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.div(a, b);                     }};
class fnmul  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.chs(fpu.mul(a, b));            }};
class fabs_diff  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.abs(fpu.sub(a, b));            }};
class fnabs_diff { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.nabs(fpu.sub(a, b));           }};

class fcmp_o   { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered     (a, b); }};
class fcmp_oeq { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_eq  (a, b); }};
class fcmp_one { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_ne  (a, b); }};
class fcmp_olt { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_lt  (a, b); }};
class fcmp_oge { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_ge  (a, b); }};
class fcmp_u   { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered   (a, b); }};
class fcmp_ueq { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_eq(a, b); }};
class fcmp_une { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_ne(a, b); }};
class fcmp_ult { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_lt(a, b); }};
class fcmp_uge { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_ge(a, b); }};

class fneg   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a) { return fpu.chs(a);   }};
class fabs   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a) { return fpu.abs(a);   }};
class fnabs  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a) { return fpu.nabs(a);  }};
class fsqrt  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a) { return fpu.sqrt(a);  }};
class frsqrt { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a) { return fpu.rsqrt(a); }};

class frnd    { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.round_to_int<T>(a, rm); }};
class extend  { public: template<typename D, typename T> static D func(fpu::SoftFPU& fpu, T a) { return fpu.extend<D,T>(a); }};
class cvt_f_f { public: template<typename D, typename T> static D func(fpu::SoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_f_f<D,T>(a, rm); }};
class cvt_f_i { public: template<typename D, typename T> static D func(fpu::SoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_f_i<D,T>(a, rm); }};
class cvt_i_f { public: template<typename D, typename T> static D func(fpu::SoftFPU& fpu, T a, fpu::RoundingModeEnum rm) { return fpu.cvt_i_f<D,T>(a, rm); }};

class fmerge { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) { return fpu.merge(a, b, c); }};
class fmadd  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_NONE);    }};
class fmsub  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_ADDEND);  }};
class fnmadd { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_PRODUCT); }};
class fnmsub { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_RESULT);  }};

class fadd_alt { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return { fpu.add(a.re, b.re), fpu.sub(a.im, b.im) };  }};
class fsub_alt { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return { fpu.sub(a.re, b.re), fpu.add(a.im, b.im) };  }};

class fmadd_alt { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) {
 return { fpu.muladd(a.re, b.re, c.re, fpu::FLOAT_MULADD_NEGATE_NONE),
          fpu.muladd(a.im, b.im, c.im, fpu::FLOAT_MULADD_NEGATE_ADDEND) };   }};

class fmsub_alt { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b, T c) {
 return { fpu.muladd(a.re, b.re, c.re, fpu::FLOAT_MULADD_NEGATE_ADDEND),
          fpu.muladd(a.im, b.im, c.im, fpu::FLOAT_MULADD_NEGATE_NONE) };   }};

/***************************************************************************
* templates
***************************************************************************/
template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp2(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp3(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp4(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>(),
                                     rd().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::FpScalarCompare(void)
{
    bool const condition = OP::template func<T>(fpu, rb().template scalar<T>(),
                                                     rc().template scalar<T>());
    ra() = condition;
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp2RoundingMode(fpu::RoundingModeEnum rm)
{
    ra() = OP:: template func<D>(fpu, rb().template scalar<T>(), rm);
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp2(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D> ( fpu, b.template vector<T>(i) );
    }
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp2RoundingMode(fpu::RoundingModeEnum rm)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D>(fpu, b.template vector<T>(i), rm);
    }
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp3(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP::template func<D>(fpu, b.template vector<T>(i), c.template vector<T>(i));
    }
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp3Horizontal(void)
{
    static_assert(sizeof(T) == sizeof(D));
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();

    GeneralRegister tmp;

    for (size_t i = 0; i < GeneralRegister::size<T>() / 2; ++i) {
        tmp.template vector<D>(i) =
            OP::template func<D>(fpu, b.template vector<T>(2*i+0),
                                      b.template vector<T>(2*i+1));
    }
    for (size_t i = 0; i < GeneralRegister::size<T>() / 2; ++i) {
        tmp.template vector<D>(i + GeneralRegister::size<T>() / 2) =
            OP::template func<D>(fpu, c.template vector<T>(2*i+0),
                                      c.template vector<T>(2*i+1));
    }

    ra() = tmp;
    return FpuFinalizeInstruction();
}

template<typename D, typename T> ALWAYS_INLINE Result Core::VectorOp3DotProduct(void)
{
    static_assert(sizeof(T) == sizeof(D));
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();

    D tmp = fpu.mul(b.template vector<T>(0),
                    c.template vector<T>(0));
    for (size_t i = 1; i < GeneralRegister::size<T>(); ++i) {
        tmp = fpu.add(tmp, fpu.mul(b.template vector<T>(i),
                                   c.template vector<T>(i)));
    }

    ra() = tmp;
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp4(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    const GeneralRegister& d = rd();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP:: template func<D> (fpu, b.template vector<T>(i), c.template vector<T>(i), d.template vector<T>(i));
    }
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::FpVectorCompare(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        bool const condition = OP::template func<T>(fpu, b.template vector<T>(i), c.template vector<T>(i));
        a.template vector<D>(i) = condition;
    }
    return FpuFinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::FpBranchOnCompare(AddressSpace& as)
{
    bool const condition = OP::template func<T>(fpu, ra().template scalar<T>(),
                                                     rb().template scalar<T>());
    if (condition) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::FpNullifyOnCompare(void)
{
    bool const condition = OP::template func<T>(fpu, ra().template scalar<T>(),
                                                     rb().template scalar<T>());
    return NullifyConditionally(condition);
}

template<typename T> ALWAYS_INLINE Result Core::FpClassify(void)
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
    DEFINE_INSN_IMPLEMENTATION_NOMEM(neg_           ## NAME) { return core.ScalarOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_           ## NAME) { return core.ScalarOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nabs_          ## NAME) { return core.ScalarOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sqrt_          ## NAME) { return core.ScalarOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rsqrt_         ## NAME) { return core.ScalarOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nadd_          ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmul_          ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(div_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_diff_      ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fabs_diff > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nabs_diff_     ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fnabs_diff> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_           ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(minnum_        ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fminnum> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(maxnum_        ## NAME) { return core.ScalarOp3 < TYPE, TYPE, fmaxnum> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_min_       ## NAME) { return core.ScalarOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_max_       ## NAME) { return core.ScalarOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_         ## NAME) { return core.ScalarOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(madd_          ## NAME) { return core.ScalarOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(msub_          ## NAME) { return core.ScalarOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmadd_         ## NAME) { return core.ScalarOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmsub_         ## NAME) { return core.ScalarOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(floor_         ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RDN); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(ceil_          ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RUP); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_         ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(roundeven_     ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RNE); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(round_         ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RMM); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nearbyint_     ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rint_          ## NAME) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_oeq_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_one_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_olt_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_oge_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_oge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ueq_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_une_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ult_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_uge_       ## NAME) { return core.FpScalarCompare<TYPE, fcmp_uge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_o_         ## NAME) { return core.FpScalarCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_u_         ## NAME) { return core.FpScalarCompare<TYPE, fcmp_u  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(class_         ## NAME) { return core.FpClassify<TYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_i32_       ## NAME) { return core.ScalarOp2RoundingMode < TYPE,  i32, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_u32_       ## NAME) { return core.ScalarOp2RoundingMode < TYPE,  u32, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_i64_       ## NAME) { return core.ScalarOp2RoundingMode < TYPE,  i64, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_u64_       ## NAME) { return core.ScalarOp2RoundingMode < TYPE,  u64, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_i128_      ## NAME) { return core.ScalarOp2RoundingMode < TYPE, i128, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_u128_      ## NAME) { return core.ScalarOp2RoundingMode < TYPE, u128, cvt_i_f> (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _i32 ) { return core.ScalarOp2RoundingMode <  i32, TYPE, cvt_f_i> (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _u32 ) { return core.ScalarOp2RoundingMode <  u32, TYPE, cvt_f_i> (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _i64 ) { return core.ScalarOp2RoundingMode <  i64, TYPE, cvt_f_i> (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _u64 ) { return core.ScalarOp2RoundingMode <  u64, TYPE, cvt_f_i> (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _i128) { return core.ScalarOp2RoundingMode < i128, TYPE, cvt_f_i> (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME ## _u128) { return core.ScalarOp2RoundingMode < u128, TYPE, cvt_f_i> (fpu::RM_RTZ); } \

FP_BASE_SCALAR(f16, fpu::f16)
FP_BASE_SCALAR(f32, fpu::f32)
FP_BASE_SCALAR(f64, fpu::f64)
FP_BASE_SCALAR(f128, fpu::f128)

#define FP_BASE_BRANCH_SCALAR(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION      (br_oeq_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_oeq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_one_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_one>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_olt_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_olt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_oge_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_oge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_o_       ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_o  >(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ueq_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ueq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_une_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_une>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ult_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ult>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_uge_     ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_uge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_u_       ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_u  >(as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_oeq_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_one_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_olt_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_oge_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_oge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_o_      ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ueq_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_une_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ult_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_uge_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_uge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_u_      ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_u  >(); } \

FP_BASE_BRANCH_SCALAR(f32, fpu::f32)
FP_BASE_BRANCH_SCALAR(f64, fpu::f64)
FP_BASE_BRANCH_SCALAR(f128, fpu::f128)

#define FP_BASE_VECTOR(NAME, INAME, TYPE, INTTYPE, UINTTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(neg_           ## NAME) { return core.VectorOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_           ## NAME) { return core.VectorOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nabs_          ## NAME) { return core.VectorOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sqrt_          ## NAME) { return core.VectorOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rsqrt_         ## NAME) { return core.VectorOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nadd_          ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(div_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmul_          ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_diff_      ## NAME) { return core.VectorOp3 < TYPE, TYPE, fabs_diff > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nabs_diff_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnabs_diff> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_           ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(minnum_        ## NAME) { return core.VectorOp3 < TYPE, TYPE, fminnum> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(maxnum_        ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmaxnum> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_min_       ## NAME) { return core.VectorOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_max_       ## NAME) { return core.VectorOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_         ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(madd_          ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(msub_          ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmadd_         ## NAME) { return core.VectorOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nmsub_         ## NAME) { return core.VectorOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(madd_alt_      ## NAME) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(msub_alt_      ## NAME) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_alt_       ## NAME) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_alt_       ## NAME) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_horiz_     ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_horiz_     ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_horiz_     ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fmul > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(dot_           ## NAME) { return core.VectorOp3DotProduct < TYPE, TYPE> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_oeq_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_one_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_olt_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_oge_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_oge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_o_         ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ueq_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_une_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ult_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_uge_       ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_uge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_u_         ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_u  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_low_     ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(merge_high_    ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_high_   ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(unpack_low_    ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(pack_          ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(floor_         ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RDN); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(ceil_          ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RUP); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_         ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(roundeven_     ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RNE); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(round_         ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (fpu::RM_RMM); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nearbyint_     ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rint_          ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME  ## _vu ## INAME) { return core.VectorOp2RoundingMode<UINTTYPE, TYPE, cvt_f_i>(fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(trunc_ ## NAME  ## _vi ## INAME) { return core.VectorOp2RoundingMode< INTTYPE, TYPE, cvt_f_i>(fpu::RM_RTZ); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_vi ## INAME ## _   ## NAME ) { return core.VectorOp2RoundingMode<TYPE,  INTTYPE, cvt_i_f>(core.fpu.get_rounding_mode()); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_vu ## INAME ## _   ## NAME ) { return core.VectorOp2RoundingMode<TYPE, UINTTYPE, cvt_i_f>(core.fpu.get_rounding_mode()); } \

FP_BASE_VECTOR(vf16, 16, fpu::f16, i16, u16)
FP_BASE_VECTOR(vf32, 32, fpu::f32, i32, u32)
FP_BASE_VECTOR(vf64, 64, fpu::f64, i64, u64)

/***************************************************************************
* f128 custom
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(scale_f128)
{
    const int scale = static_cast<int>(core.cmd.rc);
    core.ra() = core.fpu.scalbn(core.rb().sfq(), scale);
    return core.FpuFinalizeInstruction();
}

/***************************************************************************
* all-to-all
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f32_f16)  { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f32,  cvt_f_f>(core.fpu.get_rounding_mode()); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f64_f16)  { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f64,  cvt_f_f>(core.fpu.get_rounding_mode()); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f128_f16) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f128, cvt_f_f>(core.fpu.get_rounding_mode()); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f64_f32)  { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f64,  cvt_f_f>(core.fpu.get_rounding_mode()); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f128_f32) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f128, cvt_f_f>(core.fpu.get_rounding_mode()); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cvt_f128_f64) { return core.ScalarOp2RoundingMode<fpu::f64, fpu::f128, cvt_f_f>(core.fpu.get_rounding_mode()); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f16_f32)  { return core.ScalarOp2<fpu::f64,  fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f16_f64)  { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f16_f128) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f32_f64)  { return core.ScalarOp2<fpu::f64,  fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f32_f128) { return core.ScalarOp2<fpu::f128, fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(extend_f64_f128) { return core.ScalarOp2<fpu::f128, fpu::f64, extend>(); }

} //namespace postrisc
