#include "core.hpp"

namespace postrisc {

class fmin   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.min(a, b, true);               }};
class fmax   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.max(a, b, true);               }};
class famin  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.min(a.abs(), b.abs(), true);   }};
class famax  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.max(a.abs(), b.abs(), true);   }};
class fadd   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.add(a, b);                     }};
class fsub   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.sub(a, b);                     }};
class fnadd  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.add(a, b).chs();               }};
class fmul   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.mul(a, b);                     }};
class fdiv   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.div(a, b);                     }};
class fnmul  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.mul(a, b).chs();               }};
class fabsd  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.sub(a, b).abs();               }};
class fnabsd { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, T b) { return fpu.sub(a, b).nabs();              }};

class fcmp { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a, T b, fpu::compare_mode_t cm) { return D(0) - ((cm & fpu.compare(a, b, cm)) != 0); }};
class fbcmp { public: template<class D, class T> static bool func(fpu::CSoftFPU & fpu, T a, T b, fpu::compare_mode_t cm) { return (cm & fpu.compare(a, b, cm)) != 0;  }};

class cvti2f32 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_f32(a);   }};
class cvtu2f32 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_f32(a);   }};
class cvtf2i32 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_i32(a);   }};
class cvtf2u32 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_u32(a);  }};

class cvti2f64 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_f64(a);   }};
class cvtu2f64 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_f64(a);   }};
class cvtf2i64 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_i64(a);   }};
class cvtf2u64 { public: template<class D, class T> static D func(fpu::CSoftFPU & fpu, T a) { return fpu.to_u64(a);  }};

class fneg   { public: template<class T> static T func(fpu::CSoftFPU & UNUSED(fpu), T a) { return a.chs();   }};
class fabs   { public: template<class T> static T func(fpu::CSoftFPU & UNUSED(fpu), T a) { return a.abs();   }};
class fnabs  { public: template<class T> static T func(fpu::CSoftFPU & UNUSED(fpu), T a) { return a.nabs();  }};
class fsqrt  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a) { return fpu.sqrt(a);  }};
class frsqrt { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a) { return fpu.div( T(fpu::SIGN_POSITIVE, 0), fpu.sqrt(a));   }};
class frnd   { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a, fpu::rounding_mode_t rm) { return fpu.round_to_int(a, rm);   }};

class fmerge { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a,T b, T c) { return fpu.merge(a, b, c);   }};
class fmadd  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a,T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_NONE);    }};
class fmsub  { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a,T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_ADDEND);  }};
class fnmadd { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a,T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_PRODUCT); }};
class fnmsub { public: template<class T> static T func(fpu::CSoftFPU & fpu, T a,T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_RESULT);  }};

inline
CStatus
Core::FpuFinalizeInstruction()
{
    return FinalizeInstruction();
}

inline
CStatus
Core::GetRoundingMode(unsigned roundmode, fpu::rounding_mode_t & rm)
{
    switch (roundmode) {
        case fpu::FLOAT_ROUND_TONEAREST_EVEN:
        case fpu::FLOAT_ROUND_DOWNWARD:
        case fpu::FLOAT_ROUND_UPWARD:
        case fpu::FLOAT_ROUND_TOWARDZERO:
            rm = static_cast<fpu::rounding_mode_t>(roundmode);
            break;
        case 4: // use current mode
            rm = fpu.get_rounding_mode();
            break;
        default:
            LOG_ERROR(LOG_EXCEPTION) << "Invalid rounding mode: " << roundmode;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::ScalarOp2(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    const T & bb = b.template scalar<T>();

    a = OP:: template func<D>(fpu, bb);

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::ScalarOp2RoundingMode(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    fpu::rounding_mode_t rm;
    CHECK_STATUS(GetRoundingMode(cmd.GetRoundingMode(), rm));

    const T & bb = b.template scalar<T>();

    a = OP:: template func<D>(fpu, bb, rm);

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorOp2(const CMD cmd)
{
    STATIC_ASSERT(sizeof(T) == sizeof(D));

    GET_WRA;
    GET_RRB;

          D * aa = a.template vector<D>();
    const T * bb = b.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP:: template func<D> ( fpu, bb[i] );
    }

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorOp2RoundingMode(const CMD cmd)
{
    STATIC_ASSERT(sizeof(T) == sizeof(D));

    fpu::rounding_mode_t rm;
    CHECK_STATUS(GetRoundingMode(cmd.GetRoundingMode(), rm));

    GET_WRA;
    GET_RRB;

          D * aa = a.template vector<D>();
    const T * bb = b.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP:: template func<D> ( fpu, bb[i], rm );
    }

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::ScalarOp3(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const T & bb = b.template scalar<T>();
    const T & cc = c.template scalar<T>();

    a = OP:: template func<D>(fpu, bb, cc);

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class PARAM, class CMD>
ALWAYS_INLINE
CStatus
Core::ScalarOp3(const CMD cmd, PARAM param)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const T & bb = b.template scalar<T>();
    const T & cc = c.template scalar<T>();

    a = OP:: template func<D, T>(fpu, bb, cc, param);

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorOp3(const CMD cmd)
{
    STATIC_ASSERT(sizeof(T) == sizeof(D));

    GET_WRA;
    GET_RRB;
    GET_RRC;

          D * aa = a.template vector<D>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP:: template func<D> ( fpu, bb[i], cc[i] );
    }

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class PARAM, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorOp3(const CMD cmd, PARAM param)
{
    STATIC_ASSERT(sizeof(T) == sizeof(D));

    GET_WRA;
    GET_RRB;
    GET_RRC;

          D * aa = a.template vector<D>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP:: template func<D, T> ( fpu, bb[i], cc[i], param );
    }

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::ScalarOp4(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const T & bb = b.template scalar<T>();
    const T & cc = c.template scalar<T>();
    const T & dd = d.template scalar<T>();

    a = OP:: template func<D>(fpu, bb, cc, dd);

    return FpuFinalizeInstruction();
}

template<class D, class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::VectorOp4(const CMD cmd)
{
    STATIC_ASSERT(sizeof(T) == sizeof(D));

    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

          D * aa = a.template vector<D>();
    const T * bb = b.template vector<T>();
    const T * cc = c.template vector<T>();
    const T * dd = d.template vector<T>();

    for (size_t i=0; i < CGeneralRegister::size<T>(); ++i) {
        aa[i] = OP:: template func<D> ( fpu, bb[i], cc[i], dd[i] );
    }

    return FpuFinalizeInstruction();
}

/////////////////////////////////////////////////////////////////////////////////
// f32
/////////////////////////////////////////////////////////////////////////////////

DEFINE_INSN_IMPLEMENTATION(cvti2ps)  {  return VectorOp2 < fpu::f32,  int32_t, cvti2f32 > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cvtu2ps)  {  return VectorOp2 < fpu::f32, uint32_t, cvtu2f32 > (cmd); }

DEFINE_INSN_IMPLEMENTATION(cvti2ss)  {  GET_WRA; GET_RRB; a = fpu.to_f32( int64_t(b) );   return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtu2ss)  {  GET_WRA; GET_RRB; a = fpu.to_f32( uint64_t(b) );   return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtss2i)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfs() );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtss2u)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfs() );  return FpuFinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(cvtps2u)  {  return VectorOp2 < uint32_t, fpu::f32, cvtf2u32 > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(cvtps2i)  {  return VectorOp2 <  int32_t, fpu::f32, cvtf2i32 > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fminss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fmin   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmaxss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fmax   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(faminss)  {  return ScalarOp3 < fpu::f32, fpu::f32, famin  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(famaxss)  {  return ScalarOp3 < fpu::f32, fpu::f32, famax  > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fminps)   {  return VectorOp3 < fpu::f32, fpu::f32, fmin   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmaxps)   {  return VectorOp3 < fpu::f32, fpu::f32, fmax   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(faminps)  {  return VectorOp3 < fpu::f32, fpu::f32, famin  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(famaxps)  {  return VectorOp3 < fpu::f32, fpu::f32, famax  > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(frndss)   {  return ScalarOp2RoundingMode < fpu::f32, fpu::f32, frnd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fnegps)   {  return VectorOp2 < fpu::f32, fpu::f32, fneg   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsqrtps)  {  return VectorOp2 < fpu::f32, fpu::f32, fsqrt  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frsqrtps) {  return VectorOp2 < fpu::f32, fpu::f32, frsqrt > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(frndps)   {  return VectorOp2RoundingMode < fpu::f32, fpu::f32, frnd > (cmd);  }

template<class CMD>
ALWAYS_INLINE
CStatus
Core::PackedF32MulAddAlternate(const CMD cmd, fpu::muladd_negate_t even_sig, fpu::muladd_negate_t odd_sig)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    a.sfs(0) = fpu.muladd(b.sfs(0), c.sfs(0), d.sfs(0), even_sig);
    a.sfs(1) = fpu.muladd(b.sfs(1), c.sfs(1), d.sfs(1), odd_sig);
    a.sfs(2) = fpu.muladd(b.sfs(2), c.sfs(2), d.sfs(2), even_sig);
    a.sfs(3) = fpu.muladd(b.sfs(3), c.sfs(3), d.sfs(3), odd_sig);

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fmaddaps) {  return PackedF32MulAddAlternate(cmd, fpu::FLOAT_MULADD_NEGATE_NONE, fpu::FLOAT_MULADD_NEGATE_ADDEND); }
DEFINE_INSN_IMPLEMENTATION(fmsubaps) {  return PackedF32MulAddAlternate(cmd, fpu::FLOAT_MULADD_NEGATE_ADDEND, fpu::FLOAT_MULADD_NEGATE_NONE); }

DEFINE_INSN_IMPLEMENTATION(fmaddss)  {  return ScalarOp4 < fpu::f32, fpu::f32, fmadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmsubss)  {  return ScalarOp4 < fpu::f32, fpu::f32, fmsub  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmaddss) {  return ScalarOp4 < fpu::f32, fpu::f32, fnmadd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmsubss) {  return ScalarOp4 < fpu::f32, fpu::f32, fnmsub > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fmaddps)  {  return VectorOp4 < fpu::f32, fpu::f32, fmadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmsubps)  {  return VectorOp4 < fpu::f32, fpu::f32, fmsub  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmaddps) {  return VectorOp4 < fpu::f32, fpu::f32, fnmadd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmsubps) {  return VectorOp4 < fpu::f32, fpu::f32, fnmsub > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fmergess) {  return ScalarOp4 < fpu::f32, fpu::f32, fmerge > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmergeps) {  return VectorOp4 < fpu::f32, fpu::f32, fmerge > (cmd);  }


DEFINE_INSN_IMPLEMENTATION(fcmpss) {  return ScalarOp3 < bool, fpu::f32, fbcmp > (cmd, cmd.GetFpCompareMode());  }

DEFINE_INSN_IMPLEMENTATION(faddss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fadd   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsubss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fsub   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnaddss)  {  return ScalarOp3 < fpu::f32, fpu::f32, fnadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmulss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fmul   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmulss)  {  return ScalarOp3 < fpu::f32, fpu::f32, fnmul  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fdivss)   {  return ScalarOp3 < fpu::f32, fpu::f32, fdiv   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabsdss)  {  return ScalarOp3 < fpu::f32, fpu::f32, fabsd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsdss) {  return ScalarOp3 < fpu::f32, fpu::f32, fnabsd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fabsss)   {  return ScalarOp2 < fpu::f32, fpu::f32, fabs   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsss)  {  return ScalarOp2 < fpu::f32, fpu::f32, fnabs  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnegss)   {  return ScalarOp2 < fpu::f32, fpu::f32, fneg   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsqrtss)  {  return ScalarOp2 < fpu::f32, fpu::f32, fsqrt  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frsqrtss) {  return ScalarOp2 < fpu::f32, fpu::f32, frsqrt > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(faddps)   {  return VectorOp3 < fpu::f32, fpu::f32, fadd   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsubps)   {  return VectorOp3 < fpu::f32, fpu::f32, fsub   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnaddps)  {  return VectorOp3 < fpu::f32, fpu::f32, fnadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmulps)   {  return VectorOp3 < fpu::f32, fpu::f32, fmul   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fdivps)   {  return VectorOp3 < fpu::f32, fpu::f32, fdiv   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmulps)  {  return VectorOp3 < fpu::f32, fpu::f32, fnmul  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabsdps)  {  return VectorOp3 < fpu::f32, fpu::f32, fabsd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsdps) {  return VectorOp3 < fpu::f32, fpu::f32, fnabsd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fcmpps) {  return VectorOp3 < uint32_t,     fpu::f32, fcmp > (cmd, cmd.GetFpCompareMode());  }

DEFINE_INSN_IMPLEMENTATION(fmrglps)  {  return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmrghps)  {  return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(funpkhps) {  return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(funpklps) {  return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(fpkps)    {  return unimplemented(cmd);  }

DEFINE_INSN_IMPLEMENTATION(faddcps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a.sfs(0) = fpu.add( b.sfs(0), c.sfs(0) );
    a.sfs(1) = fpu.sub( b.sfs(1), c.sfs(1) );
    a.sfs(2) = fpu.add( b.sfs(2), c.sfs(2) );
    a.sfs(3) = fpu.sub( b.sfs(3), c.sfs(3) );

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fsubcps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a.sfs(0) = fpu.sub( b.sfs(0), c.sfs(0) );
    a.sfs(1) = fpu.add( b.sfs(1), c.sfs(1) );
    a.sfs(2) = fpu.sub( b.sfs(2), c.sfs(2) );
    a.sfs(3) = fpu.add( b.sfs(3), c.sfs(3) );

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(faddhps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfs(0) = fpu.add( b.sfs(0), b.sfs(1) );
    tmp.sfs(1) = fpu.add( b.sfs(2), b.sfs(3) );
    tmp.sfs(2) = fpu.add( c.sfs(0), c.sfs(1) );
    tmp.sfs(3) = fpu.add( c.sfs(2), c.sfs(3) );

    a = tmp;

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fsubhps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfs(0) = fpu.sub( b.sfs(0), b.sfs(1) );
    tmp.sfs(1) = fpu.sub( b.sfs(2), b.sfs(3) );
    tmp.sfs(2) = fpu.sub( c.sfs(0), c.sfs(1) );
    tmp.sfs(3) = fpu.sub( c.sfs(2), c.sfs(3) );

    a = tmp;

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fmulhps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfs(0) = fpu.mul( b.sfs(0), b.sfs(1) );
    tmp.sfs(1) = fpu.mul( b.sfs(2), b.sfs(3) );
    tmp.sfs(2) = fpu.mul( c.sfs(0), c.sfs(1) );
    tmp.sfs(3) = fpu.mul( c.sfs(2), c.sfs(3) );

    a = tmp;

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fdotps)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a = fpu.add( fpu.add( fpu.mul( b.sfs(0), c.sfs(0) ),
                          fpu.mul( b.sfs(1), c.sfs(1) ) ),
                 fpu.add( fpu.mul( b.sfs(2), c.sfs(2) ),
                          fpu.mul( b.sfs(3), c.sfs(3) ) ) );

    return FpuFinalizeInstruction();
}


/////////////////////////////////////////////////////////////////////////////////
// f64
/////////////////////////////////////////////////////////////////////////////////

DEFINE_INSN_IMPLEMENTATION(cvtsd2ss) {  GET_WRA; GET_RRB; a = fpu.to_f32 ( b.sfd() );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtss2sd) {  GET_WRA; GET_RRB; a = fpu.to_f64 ( b.sfs() );  return FpuFinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(cvtpd2u)  {  return VectorOp2 < uint64_t, fpu::f64, cvtf2u64 > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cvtpd2i)  {  return VectorOp2 < int64_t, fpu::f64, cvtf2i64 > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cvti2pd)  {  return VectorOp2 < fpu::f64, int64_t, cvti2f64 > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cvtu2pd)  {  return VectorOp2 < fpu::f64, uint64_t, cvtu2f64 > (cmd); }

DEFINE_INSN_IMPLEMENTATION(cvti2sd)  {  GET_WRA; GET_RRB; a = fpu.to_f64( int64_t(b) );     return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtu2sd)  {  GET_WRA; GET_RRB; a = fpu.to_f64( uint64_t(b) );     return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsd2i)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfd() );    return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsd2u)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfd() );    return FpuFinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(fminsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fmin   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmaxsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fmax   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(faminsd)  {  return ScalarOp3 < fpu::f64, fpu::f64, famin  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(famaxsd)  {  return ScalarOp3 < fpu::f64, fpu::f64, famax  > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fminpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fmin   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmaxpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fmax   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(faminpd)  {  return VectorOp3 < fpu::f64, fpu::f64, famin  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(famaxpd)  {  return VectorOp3 < fpu::f64, fpu::f64, famax  > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(frndsd)   {  return ScalarOp2RoundingMode < fpu::f64, fpu::f64, frnd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frndpd)   {  return VectorOp2RoundingMode < fpu::f64, fpu::f64, frnd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fnegpd)   {  return VectorOp2 < fpu::f64, fpu::f64, fneg   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsqrtpd)  {  return VectorOp2 < fpu::f64, fpu::f64, fsqrt  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frsqrtpd) {  return VectorOp2 < fpu::f64, fpu::f64, frsqrt > (cmd);  }

template<class CMD>
ALWAYS_INLINE
CStatus
Core::PackedF64MulAddAlternate(const CMD cmd, fpu::muladd_negate_t even_sig, fpu::muladd_negate_t odd_sig)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    a.sfd(0) = fpu.muladd(b.sfd(0), c.sfd(0), d.sfd(0), even_sig);
    a.sfd(1) = fpu.muladd(b.sfd(1), c.sfd(1), d.sfd(1), odd_sig);

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fmaddapd) {  return PackedF64MulAddAlternate(cmd, fpu::FLOAT_MULADD_NEGATE_NONE, fpu::FLOAT_MULADD_NEGATE_ADDEND); }
DEFINE_INSN_IMPLEMENTATION(fmsubapd) {  return PackedF64MulAddAlternate(cmd, fpu::FLOAT_MULADD_NEGATE_ADDEND, fpu::FLOAT_MULADD_NEGATE_NONE); }

DEFINE_INSN_IMPLEMENTATION(fmaddsd)  {  return ScalarOp4 < fpu::f64, fpu::f64, fmadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmsubsd)  {  return ScalarOp4 < fpu::f64, fpu::f64, fmsub  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmaddsd) {  return ScalarOp4 < fpu::f64, fpu::f64, fnmadd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmsubsd) {  return ScalarOp4 < fpu::f64, fpu::f64, fnmsub > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmergesd) {  return ScalarOp4 < fpu::f64, fpu::f64, fmerge > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fmaddpd)  {  return VectorOp4 < fpu::f64, fpu::f64, fmadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmsubpd)  {  return VectorOp4 < fpu::f64, fpu::f64, fmsub  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmaddpd) {  return VectorOp4 < fpu::f64, fpu::f64, fnmadd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmsubpd) {  return VectorOp4 < fpu::f64, fpu::f64, fnmsub > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmergepd) {  return VectorOp4 < fpu::f64, fpu::f64, fmerge > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fcmpsd) {  return ScalarOp3 < bool, fpu::f64, fbcmp > (cmd, cmd.GetFpCompareMode());  }

DEFINE_INSN_IMPLEMENTATION(faddsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fadd   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsubsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fsub   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnaddsd)  {  return ScalarOp3 < fpu::f64, fpu::f64, fnadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmulsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fmul   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmulsd)  {  return ScalarOp3 < fpu::f64, fpu::f64, fnmul  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fdivsd)   {  return ScalarOp3 < fpu::f64, fpu::f64, fdiv   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabsdsd)  {  return ScalarOp3 < fpu::f64, fpu::f64, fabsd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsdsd) {  return ScalarOp3 < fpu::f64, fpu::f64, fnabsd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fnegsd)   {  return ScalarOp2 < fpu::f64, fpu::f64, fneg   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabssd)   {  return ScalarOp2 < fpu::f64, fpu::f64, fabs   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabssd)  {  return ScalarOp2 < fpu::f64, fpu::f64, fnabs  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsqrtsd)  {  return ScalarOp2 < fpu::f64, fpu::f64, fsqrt  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frsqrtsd) {  return ScalarOp2 < fpu::f64, fpu::f64, frsqrt > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(faddpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fadd   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsubpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fsub   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnaddpd)  {  return VectorOp3 < fpu::f64, fpu::f64, fnadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmulpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fmul   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fdivpd)   {  return VectorOp3 < fpu::f64, fpu::f64, fdiv   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmulpd)  {  return VectorOp3 < fpu::f64, fpu::f64, fnmul  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabsdpd)  {  return VectorOp3 < fpu::f64, fpu::f64, fabsd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsdpd) {  return VectorOp3 < fpu::f64, fpu::f64, fnabsd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fcmppd) {  return VectorOp3 < uint64_t,     fpu::f64, fcmp > (cmd, cmd.GetFpCompareMode());  }

DEFINE_INSN_IMPLEMENTATION(faddcpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a.sfd(0) = fpu.add( b.sfd(0), c.sfd(0) );
    a.sfd(1) = fpu.sub( b.sfd(1), c.sfd(1) );

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fsubcpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a.sfd(0) = fpu.sub( b.sfd(0), c.sfd(0) );
    a.sfd(1) = fpu.add( b.sfd(1), c.sfd(1) );

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(faddhpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfd(0) = fpu.add( b.sfd(0), b.sfd(1) );
    tmp.sfd(1) = fpu.add( c.sfd(0), c.sfd(1) );

    a = tmp;
    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fsubhpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfd(0) = fpu.sub( b.sfd(0), b.sfd(1) );
    tmp.sfd(1) = fpu.sub( c.sfd(0), c.sfd(1) );

    a = tmp;
    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fmulhpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister tmp;

    tmp.sfd(0) = fpu.mul( b.sfd(0), b.sfd(1) );
    tmp.sfd(1) = fpu.mul( c.sfd(0), c.sfd(1) );

    a = tmp;
    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fdotpd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a = fpu.add ( fpu.mul( b.sfd(0), c.sfd(0) ),
                  fpu.mul( b.sfd(1), c.sfd(1) ) );

    return FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fmrglpd)  {    return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmrghpd)  {    return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(funpkhpd) {    return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(funpklpd) {    return unimplemented(cmd);  }
DEFINE_INSN_IMPLEMENTATION(fpkpd)    {    return unimplemented(cmd);  }

/////////////////////////////////////////////////////////////////////////////////
// f128
/////////////////////////////////////////////////////////////////////////////////

DEFINE_INSN_IMPLEMENTATION(cvti2sq)  {  GET_WRA; GET_RRB; a = fpu.to_f128(  int64_t(b) );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtu2sq)  {  GET_WRA; GET_RRB; a = fpu.to_f128( uint64_t(b) );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsq2i)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfq() );    return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsq2u)  {  GET_WRA; GET_RRB; a = fpu.to_i64( b.sfq() );    return FpuFinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(cvtsq2ss) {  GET_WRA; GET_RRB; a = fpu.to_f32 ( b.sfq() );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsq2sd) {  GET_WRA; GET_RRB; a = fpu.to_f64 ( b.sfq() );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtsd2sq) {  GET_WRA; GET_RRB; a = fpu.to_f128( b.sfd() );  return FpuFinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(cvtss2sq) {  GET_WRA; GET_RRB; a = fpu.to_f128( b.sfs() );  return FpuFinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(fnegsq)   {  return ScalarOp2 < fpu::f128, fpu::f128, fneg   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabssq)   {  return ScalarOp2 < fpu::f128, fpu::f128, fabs   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabssq)  {  return ScalarOp2 < fpu::f128, fpu::f128, fnabs  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsqrtsq)  {  return ScalarOp2 < fpu::f128, fpu::f128, fsqrt  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(frsqrtsq) {  return ScalarOp2 < fpu::f128, fpu::f128, frsqrt > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(frndsq)   {  return ScalarOp2RoundingMode < fpu::f128, fpu::f128, frnd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fcmpsq) {  return ScalarOp3 < bool, fpu::f128, fbcmp > (cmd, cmd.GetFpCompareMode());  }

DEFINE_INSN_IMPLEMENTATION(fminsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fmin   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmaxsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fmax   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(faminsq)  {  return ScalarOp3 < fpu::f128, fpu::f128, famin  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(famaxsq)  {  return ScalarOp3 < fpu::f128, fpu::f128, famax  > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(faddsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fadd   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fsubsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fsub   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnaddsq)  {  return ScalarOp3 < fpu::f128, fpu::f128, fnadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmulsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fmul   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmulsq)  {  return ScalarOp3 < fpu::f128, fpu::f128, fnmul  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fdivsq)   {  return ScalarOp3 < fpu::f128, fpu::f128, fdiv   > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fabsdsq)  {  return ScalarOp3 < fpu::f128, fpu::f128, fabsd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnabsdsq) {  return ScalarOp3 < fpu::f128, fpu::f128, fnabsd > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fmaddsq)  {  return ScalarOp4 < fpu::f128, fpu::f128, fmadd  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmsubsq)  {  return ScalarOp4 < fpu::f128, fpu::f128, fmsub  > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmaddsq) {  return ScalarOp4 < fpu::f128, fpu::f128, fnmadd > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fnmsubsq) {  return ScalarOp4 < fpu::f128, fpu::f128, fnmsub > (cmd);  }
DEFINE_INSN_IMPLEMENTATION(fmergesq) {  return ScalarOp4 < fpu::f128, fpu::f128, fmerge > (cmd);  }

DEFINE_INSN_IMPLEMENTATION(fscale)
{
    GET_WRA;
    GET_RRB;
    const int scale = static_cast<int>(cmd.get_vshamt_c());
    a = fpu.scalbn(b.sfq(), scale);
    return FpuFinalizeInstruction();
}

} //namespace postrisc
