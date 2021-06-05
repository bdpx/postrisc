#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

class fmin   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.min(a, b);               }};
class fmax   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.max(a, b);               }};
class fminnm { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.min(a, b);      }};
class fmaxnm { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return /*FIXME*/fpu.max(a, b);      }};
class famin  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.min(fpu.abs(a), fpu.abs(b));   }};
class famax  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.max(fpu.abs(a), fpu.abs(b));   }};
class fadd   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.add(a, b);                     }};
class fsub   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.sub(a, b);                     }};
class fnadd  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.chs(fpu.add(a, b));            }};
class fmul   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.mul(a, b);                     }};
class fdiv   { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.div(a, b);                     }};
class fnmul  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.chs(fpu.mul(a, b));            }};
class fabsd  { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.abs(fpu.sub(a, b));            }};
class fnabsd { public: template<typename T> static T func(fpu::SoftFPU& fpu, T a, T b) { return fpu.nabs(fpu.sub(a, b));           }};

class fcmp_o   { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered     (a, b); }};
class fcmp_oeq { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_eq  (a, b); }};
class fcmp_one { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_ne  (a, b); }};
class fcmp_olt { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_lt  (a, b); }};
class fcmp_ole { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_ordered_le  (a, b); }};
class fcmp_u   { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered   (a, b); }};
class fcmp_ueq { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_eq(a, b); }};
class fcmp_une { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_ne(a, b); }};
class fcmp_ult { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_lt(a, b); }};
class fcmp_ule { public: template<typename T> static bool func(fpu::SoftFPU& fpu, T a, T b) { return fpu.compare_unordered_le(a, b); }};

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

Result
Core::GetRoundingMode(unsigned roundmode, fpu::RoundingModeEnum& rm)
{
    switch (roundmode) {
    case fpu::RM_RNE:
    case fpu::RM_RTZ:
    case fpu::RM_RDN:
    case fpu::RM_RUP:
    case fpu::RM_RMM:
        rm = static_cast<fpu::RoundingModeEnum>(roundmode);
        return Result::continue_execution;
    case fpu::RM_DYN:
        rm = fpu.get_rounding_mode();
        return Result::continue_execution;
    }

    LOG_ERROR(LOG_EXCEPTION) << "Invalid rounding mode: " << roundmode;
    return Result::illegal_instruction;
}

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

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp2RoundingMode(void)
{
    fpu::RoundingModeEnum rm = fpu::RM_RNE;
    CHECK_RESULT(GetRoundingMode(cmd.rounding_mode, rm));
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

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp2RoundingMode(void)
{
    static_assert(sizeof(T) == sizeof(D));
    fpu::RoundingModeEnum rm = fpu::RM_RNE;
    CHECK_RESULT(GetRoundingMode(cmd.rounding_mode, rm));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D> ( fpu, b.template vector<T>(i), rm );
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

template<typename T> ALWAYS_INLINE Result Core::FpBranchOnClassify(AddressSpace& as)
{
    unsigned const classify = cmd.rb;
    bool const condition = (fpu.classify(ra().template scalar<T>()) & classify) != 0;
    if (condition) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<typename T> ALWAYS_INLINE Result Core::FpNullifyOnClassify(void)
{
    unsigned const classify = cmd.rb;
    bool const condition = (fpu.classify(ra().template scalar<T>()) & classify) != 0;
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
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fneg_     ## NAME       ) { return core.ScalarOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabs_     ## NAME       ) { return core.ScalarOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabs_    ## NAME       ) { return core.ScalarOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrt_    ## NAME       ) { return core.ScalarOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrt_   ## NAME       ) { return core.ScalarOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fadd_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsub_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnadd_    ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmul_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmul_    ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdiv_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsd_    ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fabsd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsd_   ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fnabsd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmin_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmax_     ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnm_   ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fminnm > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnm_   ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, fmaxnm > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famin_    ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famax_    ## NAME       ) { return core.ScalarOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerge_   ## NAME       ) { return core.ScalarOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadd_    ## NAME       ) { return core.ScalarOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsub_    ## NAME       ) { return core.ScalarOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmadd_   ## NAME       ) { return core.ScalarOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsub_   ## NAME       ) { return core.ScalarOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frnd_     ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE, TYPE, frnd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_eq_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_ne_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_lt_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_le_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_eq_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_ne_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_lt_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_le_ ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_    ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_    ## NAME       ) { return core.FpScalarCompare<TYPE, fcmp_u  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcl_      ## NAME       ) { return core.FpClassify<TYPE>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_iw_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE,  i32, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_uw_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE,  u32, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_id_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE,  i64, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_ud_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE,  u64, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_iq_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE, i128, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_uq_  ## NAME       ) { return core.ScalarOp2RoundingMode < TYPE, u128, cvt_i_f> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _iw) { return core.ScalarOp2RoundingMode <  i32, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _uw) { return core.ScalarOp2RoundingMode <  u32, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _id) { return core.ScalarOp2RoundingMode <  i64, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _ud) { return core.ScalarOp2RoundingMode <  u64, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _iq) { return core.ScalarOp2RoundingMode < i128, TYPE, cvt_f_i> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _uq) { return core.ScalarOp2RoundingMode < u128, TYPE, cvt_f_i> (); } \

FP_BASE_SCALAR(sh, fpu::f16)
FP_BASE_SCALAR(ss, fpu::f32)
FP_BASE_SCALAR(sd, fpu::f64)
FP_BASE_SCALAR(sq, fpu::f128)

#define FP_BASE_BRANCH_SCALAR(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION      (bfo_eq_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_oeq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfo_ne_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_one>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfo_lt_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_olt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfo_le_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ole>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfo_         ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_o  >(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfu_eq_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ueq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfu_ne_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_une>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfu_lt_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ult>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfu_le_      ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_ule>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bfu_         ## NAME) { return core.FpBranchOnCompare<TYPE, fcmp_u  >(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bf_class_    ## NAME) { return core.FpBranchOnClassify<TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfo_eq_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfo_ne_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfo_lt_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfo_le_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfo_       ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfu_eq_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfu_ne_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfu_lt_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfu_le_    ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulfu_       ## NAME) { return core.FpNullifyOnCompare<TYPE, fcmp_u  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulf_class_  ## NAME) { return core.FpNullifyOnClassify<TYPE>(); } \

FP_BASE_BRANCH_SCALAR(ss, fpu::f32)
FP_BASE_BRANCH_SCALAR(sd, fpu::f64)
FP_BASE_BRANCH_SCALAR(sq, fpu::f128)

#define FP_BASE_VECTOR(NAME, INAME, TYPE, INTTYPE, UINTTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fneg_     ## NAME) { return core.VectorOp2 < TYPE, TYPE, fneg   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabs_     ## NAME) { return core.VectorOp2 < TYPE, TYPE, fabs   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabs_    ## NAME) { return core.VectorOp2 < TYPE, TYPE, fnabs  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrt_    ## NAME) { return core.VectorOp2 < TYPE, TYPE, fsqrt  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrt_   ## NAME) { return core.VectorOp2 < TYPE, TYPE, frsqrt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fadd_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fadd   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsub_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fsub   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnadd_    ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmul_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmul   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdiv_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fdiv   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmul_    ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnmul  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsd_    ## NAME) { return core.VectorOp3 < TYPE, TYPE, fabsd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsd_   ## NAME) { return core.VectorOp3 < TYPE, TYPE, fnabsd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmin_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmin   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmax_     ## NAME) { return core.VectorOp3 < TYPE, TYPE, fmax   > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famin_    ## NAME) { return core.VectorOp3 < TYPE, TYPE, famin  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(famax_    ## NAME) { return core.VectorOp3 < TYPE, TYPE, famax  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerge_   ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmerge > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadd_    ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmadd  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsub_    ## NAME) { return core.VectorOp4 < TYPE, TYPE, fmsub  > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmadd_   ## NAME) { return core.VectorOp4 < TYPE, TYPE, fnmadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsub_   ## NAME) { return core.VectorOp4 < TYPE, TYPE, fnmsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadda_   ## NAME) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsuba_   ## NAME) { return core.VectorOp4 < complex<TYPE>, complex<TYPE>, fmsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(faddc_    ## NAME) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fadd_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubc_    ## NAME) { return core.VectorOp3 < complex<TYPE>, complex<TYPE>, fsub_alt > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(faddh_    ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fadd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubh_    ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fsub > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmulh_    ## NAME) { return core.VectorOp3Horizontal < TYPE, TYPE, fmul > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fdot_     ## NAME) { return core.VectorOp3DotProduct < TYPE, TYPE> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_eq_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_oeq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_ne_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_one>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_lt_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_olt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_le_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ole>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpo_    ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_o  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_eq_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ueq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_ne_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_une>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_lt_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ult>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_le_ ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_ule>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpu_    ## NAME) { return core.FpVectorCompare<INTTYPE, TYPE, fcmp_u  >(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmrgl_    ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fmrgh_    ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(funpkh_   ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(funpkl_   ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fpk_      ## NAME) { return core.unimplemented(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(frnd_     ## NAME) { return core.VectorOp2RoundingMode < TYPE, TYPE, frnd > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _u ## INAME) { return core.VectorOp2RoundingMode < UINTTYPE, TYPE, cvt_f_i > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_     ## NAME ## _i ## INAME) { return core.VectorOp2RoundingMode <  INTTYPE, TYPE, cvt_f_i > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_i    ## INAME ## _ ## NAME ) { return core.VectorOp2RoundingMode < TYPE,  INTTYPE, cvt_i_f > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_u    ## INAME ## _ ## NAME ) { return core.VectorOp2RoundingMode < TYPE, UINTTYPE, cvt_i_f > (); } \

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
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_ss_sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f32,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_sd_sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_sq_sh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_sd_ss) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_sq_ss) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcvt_sq_sd) { return core.ScalarOp2RoundingMode<fpu::f64, fpu::f128, cvt_f_f>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_sh_ss) { return core.ScalarOp2<fpu::f64,  fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_sh_sd) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_sh_sq) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_ss_sd) { return core.ScalarOp2<fpu::f64,  fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_ss_sq) { return core.ScalarOp2<fpu::f128, fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fext_sd_sq) { return core.ScalarOp2<fpu::f128, fpu::f64, extend>(); }

} //namespace postrisc
