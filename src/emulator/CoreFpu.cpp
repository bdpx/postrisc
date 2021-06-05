#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

Result Core::GetRoundingMode(const RoundingMode rmc, fpu::RoundingModeEnum& rm) const
{
    switch (rmc) {
        case RoundingMode::RNE: rm = fpu::RM_RNE; return Result::continue_execution;
        case RoundingMode::RDN: rm = fpu::RM_RDN; return Result::continue_execution;
        case RoundingMode::RUP: rm = fpu::RM_RUP; return Result::continue_execution;
        case RoundingMode::RTZ: rm = fpu::RM_RTZ; return Result::continue_execution;
        case RoundingMode::RMM: rm = fpu::RM_RMM; return Result::continue_execution;
        case RoundingMode::RNO: rm = fpu::RM_RNO; return Result::continue_execution;
        case RoundingMode::RTO: rm = fpu::RM_RTO; return Result::continue_execution;

        case RoundingMode::DYN:
            rm = fpu.get_rounding_mode();
            return Result::continue_execution;
    }
    return Result::illegal_instruction;
};

/***************************************************************************
* templates
***************************************************************************/
template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp2(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp2RoundingMode(void)
{
    ra() = OP:: template func<D>(fpu, rb().template scalar<T>(), cmd.rm);
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp3(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp3RoundingMode(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>(), cmd.rm);
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp4(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>(),
                                     rd().template scalar<T>());
    return FpuFinalizeInstruction();
}

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::ScalarOp4RoundingMode(void)
{
    ra() = OP::template func<D>(fpu, rb().template scalar<T>(),
                                     rc().template scalar<T>(),
                                     rd().template scalar<T>(), cmd.rm);
    return FpuFinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::FpScalarCompare(void)
{
    bool const condition = OP::template func<T>(fpu, rb().template scalar<T>(),
                                                     rc().template scalar<T>());
    ra() = condition;
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
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) = OP:: template func<D>(fpu, b.template vector<T>(i), cmd.rm);
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

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp3RoundingMode(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP::template func<D>(fpu, b.template vector<T>(i), c.template vector<T>(i), cmd.rm);
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
                                      b.template vector<T>(2*i+1), cmd.rm);
    }
    for (size_t i = 0; i < GeneralRegister::size<T>() / 2; ++i) {
        tmp.template vector<D>(i + GeneralRegister::size<T>() / 2) =
            OP::template func<D>(fpu, c.template vector<T>(2*i+0),
                                      c.template vector<T>(2*i+1), cmd.rm);
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
                    c.template vector<T>(0), cmd.rm);
    for (size_t i = 1; i < GeneralRegister::size<T>(); ++i) {
        tmp = fpu.add(tmp, fpu.mul(b.template vector<T>(i),
                                   c.template vector<T>(i), cmd.rm), cmd.rm);
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

template<typename D, typename T, typename OP> ALWAYS_INLINE Result Core::VectorOp4RoundingMode(void)
{
    static_assert(sizeof(T) == sizeof(D));
    GeneralRegister& a = ra();
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();
    const GeneralRegister& d = rd();
    for (size_t i = 0; i < GeneralRegister::size<T>(); ++i) {
        a.template vector<D>(i) =
        OP:: template func<D> (fpu, b.template vector<T>(i), c.template vector<T>(i), d.template vector<T>(i), cmd.rm);
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
    const FClassMask classify_mask = static_cast<FClassMask>(cmd.imm);
    const bool condition = (fpu.classify(rb().template scalar<T>()) & classify_mask) != FClassMask::Empty;
    ra() = condition;
    return FpuFinalizeInstruction();
}

/***************************************************************************
* unified f16/f32/f64/f128
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnegh)       { return core.ScalarOp2 < fpu::f16, fpu::f16, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsh)       { return core.ScalarOp2 < fpu::f16, fpu::f16, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsh)      { return core.ScalarOp2 < fpu::f16, fpu::f16, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrth)      { return core.ScalarOp2RoundingMode < fpu::f16, fpu::f16, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrth)     { return core.ScalarOp2RoundingMode < fpu::f16, fpu::f16, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndh)       { return core.ScalarOp2RoundingMode < fpu::f16, fpu::f16, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndxh)     { return core.ScalarOp2RoundingMode < fpu::f16, fpu::f16, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faddh)       { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubh)       { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnaddh)      { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmulh)       { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmulh)      { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdivh)       { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabdh)       { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabdh)      { return core.ScalarOp3RoundingMode < fpu::f16, fpu::f16, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminh)       { return core.ScalarOp3 < fpu::f16, fpu::f16, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxh)       { return core.ScalarOp3 < fpu::f16, fpu::f16, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnmh)     { return core.ScalarOp3 < fpu::f16, fpu::f16, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnmh)     { return core.ScalarOp3 < fpu::f16, fpu::f16, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faminnmh)    { return core.ScalarOp3 < fpu::f16, fpu::f16, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(famaxnmh)    { return core.ScalarOp3 < fpu::f16, fpu::f16, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmergeh)     { return core.ScalarOp4 < fpu::f16, fpu::f16, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaddh)      { return core.ScalarOp4RoundingMode < fpu::f16, fpu::f16, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsubh)      { return core.ScalarOp4RoundingMode < fpu::f16, fpu::f16, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmaddh)     { return core.ScalarOp4RoundingMode < fpu::f16, fpu::f16, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsubh)     { return core.ScalarOp4RoundingMode < fpu::f16, fpu::f16, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoeqh)    { return core.FpScalarCompare<fpu::f16, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmponeh)    { return core.FpScalarCompare<fpu::f16, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpolth)    { return core.FpScalarCompare<fpu::f16, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpogeh)    { return core.FpScalarCompare<fpu::f16, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpueqh)    { return core.FpScalarCompare<fpu::f16, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuneh)    { return core.FpScalarCompare<fpu::f16, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpulth)    { return core.FpScalarCompare<fpu::f16, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpugeh)    { return core.FpScalarCompare<fpu::f16, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoh)      { return core.FpScalarCompare<fpu::f16, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuh)      { return core.FpScalarCompare<fpu::f16, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fclassh)     { return core.FpClassify<fpu::f16>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiwtoh)      { return core.ScalarOp2RoundingMode < fpu::f16,  i32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuwtoh)      { return core.ScalarOp2RoundingMode < fpu::f16,  u32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fidtoh)      { return core.ScalarOp2RoundingMode < fpu::f16,  i64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fudtoh)      { return core.ScalarOp2RoundingMode < fpu::f16,  u64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiqtoh)      { return core.ScalarOp2RoundingMode < fpu::f16, i128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuqtoh)      { return core.ScalarOp2RoundingMode < fpu::f16, u128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtoiw)      { return core.ScalarOp2RoundingMode <  i32, fpu::f16, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtouw)      { return core.ScalarOp2RoundingMode <  u32, fpu::f16, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtoid)      { return core.ScalarOp2RoundingMode <  i64, fpu::f16, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtoud)      { return core.ScalarOp2RoundingMode <  u64, fpu::f16, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtoiq)      { return core.ScalarOp2RoundingMode < i128, fpu::f16, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtouq)      { return core.ScalarOp2RoundingMode < u128, fpu::f16, cvt_f_i> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnegs)       { return core.ScalarOp2 < fpu::f32, fpu::f32, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabss)       { return core.ScalarOp2 < fpu::f32, fpu::f32, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabss)      { return core.ScalarOp2 < fpu::f32, fpu::f32, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrts)      { return core.ScalarOp2RoundingMode < fpu::f32, fpu::f32, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrts)     { return core.ScalarOp2RoundingMode < fpu::f32, fpu::f32, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frnds)       { return core.ScalarOp2RoundingMode < fpu::f32, fpu::f32, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndxs)     { return core.ScalarOp2RoundingMode < fpu::f32, fpu::f32, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fadds)       { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubs)       { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnadds)      { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmuls)       { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmuls)      { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdivs)       { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabds)       { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabds)      { return core.ScalarOp3RoundingMode < fpu::f32, fpu::f32, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmins)       { return core.ScalarOp3 < fpu::f32, fpu::f32, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxs)       { return core.ScalarOp3 < fpu::f32, fpu::f32, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnms)     { return core.ScalarOp3 < fpu::f32, fpu::f32, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnms)     { return core.ScalarOp3 < fpu::f32, fpu::f32, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faminnms)    { return core.ScalarOp3 < fpu::f32, fpu::f32, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(famaxnms)    { return core.ScalarOp3 < fpu::f32, fpu::f32, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerges)     { return core.ScalarOp4 < fpu::f32, fpu::f32, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmadds)      { return core.ScalarOp4RoundingMode < fpu::f32, fpu::f32, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsubs)      { return core.ScalarOp4RoundingMode < fpu::f32, fpu::f32, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmadds)     { return core.ScalarOp4RoundingMode < fpu::f32, fpu::f32, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsubs)     { return core.ScalarOp4RoundingMode < fpu::f32, fpu::f32, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoeqs)    { return core.FpScalarCompare<fpu::f32, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpones)    { return core.FpScalarCompare<fpu::f32, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpolts)    { return core.FpScalarCompare<fpu::f32, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoges)    { return core.FpScalarCompare<fpu::f32, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpueqs)    { return core.FpScalarCompare<fpu::f32, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpunes)    { return core.FpScalarCompare<fpu::f32, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpults)    { return core.FpScalarCompare<fpu::f32, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuges)    { return core.FpScalarCompare<fpu::f32, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpos)      { return core.FpScalarCompare<fpu::f32, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpus)      { return core.FpScalarCompare<fpu::f32, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fclasss)     { return core.FpClassify<fpu::f32>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiwtos)    { return core.ScalarOp2RoundingMode < fpu::f32,  i32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuwtos)    { return core.ScalarOp2RoundingMode < fpu::f32,  u32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fidtos)    { return core.ScalarOp2RoundingMode < fpu::f32,  i64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fudtos)    { return core.ScalarOp2RoundingMode < fpu::f32,  u64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiqtos)    { return core.ScalarOp2RoundingMode < fpu::f32, i128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuqtos)    { return core.ScalarOp2RoundingMode < fpu::f32, u128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoiw)    { return core.ScalarOp2RoundingMode <  i32, fpu::f32, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstouw)    { return core.ScalarOp2RoundingMode <  u32, fpu::f32, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoid)    { return core.ScalarOp2RoundingMode <  i64, fpu::f32, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoud)    { return core.ScalarOp2RoundingMode <  u64, fpu::f32, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoiq)    { return core.ScalarOp2RoundingMode < i128, fpu::f32, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstouq)    { return core.ScalarOp2RoundingMode < u128, fpu::f32, cvt_f_i> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnegd)       { return core.ScalarOp2 < fpu::f64, fpu::f64, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsd)       { return core.ScalarOp2 < fpu::f64, fpu::f64, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsd)      { return core.ScalarOp2 < fpu::f64, fpu::f64, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrtd)      { return core.ScalarOp2RoundingMode < fpu::f64, fpu::f64, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrtd)     { return core.ScalarOp2RoundingMode < fpu::f64, fpu::f64, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndd)       { return core.ScalarOp2RoundingMode < fpu::f64, fpu::f64, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndxd)     { return core.ScalarOp2RoundingMode < fpu::f64, fpu::f64, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faddd)       { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubd)       { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnaddd)      { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmuld)       { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmuld)      { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdivd)       { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabdd)      { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabdd)     { return core.ScalarOp3RoundingMode < fpu::f64, fpu::f64, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmind)       { return core.ScalarOp3 < fpu::f64, fpu::f64, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxd)       { return core.ScalarOp3 < fpu::f64, fpu::f64, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnmd)     { return core.ScalarOp3 < fpu::f64, fpu::f64, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnmd)     { return core.ScalarOp3 < fpu::f64, fpu::f64, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faminnmd)    { return core.ScalarOp3 < fpu::f64, fpu::f64, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(famaxnmd)    { return core.ScalarOp3 < fpu::f64, fpu::f64, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmerged)     { return core.ScalarOp4 < fpu::f64, fpu::f64, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaddd)      { return core.ScalarOp4RoundingMode < fpu::f64, fpu::f64, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsubd)      { return core.ScalarOp4RoundingMode < fpu::f64, fpu::f64, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmaddd)     { return core.ScalarOp4RoundingMode < fpu::f64, fpu::f64, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsubd)     { return core.ScalarOp4RoundingMode < fpu::f64, fpu::f64, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoeqd)   { return core.FpScalarCompare<fpu::f64, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmponed)   { return core.FpScalarCompare<fpu::f64, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoltd)   { return core.FpScalarCompare<fpu::f64, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoged)   { return core.FpScalarCompare<fpu::f64, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpueqd)   { return core.FpScalarCompare<fpu::f64, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuned)   { return core.FpScalarCompare<fpu::f64, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpultd)   { return core.FpScalarCompare<fpu::f64, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuged)   { return core.FpScalarCompare<fpu::f64, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpod)     { return core.FpScalarCompare<fpu::f64, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpud)     { return core.FpScalarCompare<fpu::f64, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fclassd)     { return core.FpClassify<fpu::f64>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiwtod)   { return core.ScalarOp2RoundingMode < fpu::f64,  i32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuwtod)   { return core.ScalarOp2RoundingMode < fpu::f64,  u32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fidtod)   { return core.ScalarOp2RoundingMode < fpu::f64,  i64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fudtod)   { return core.ScalarOp2RoundingMode < fpu::f64,  u64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiqtod)   { return core.ScalarOp2RoundingMode < fpu::f64, i128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuqtod)   { return core.ScalarOp2RoundingMode < fpu::f64, u128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoiw)   { return core.ScalarOp2RoundingMode <  i32, fpu::f64, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtouw)   { return core.ScalarOp2RoundingMode <  u32, fpu::f64, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoid)   { return core.ScalarOp2RoundingMode <  i64, fpu::f64, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoud)   { return core.ScalarOp2RoundingMode <  u64, fpu::f64, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoiq)   { return core.ScalarOp2RoundingMode < i128, fpu::f64, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtouq)   { return core.ScalarOp2RoundingMode < u128, fpu::f64, cvt_f_i> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnegq)       { return core.ScalarOp2 < fpu::f128, fpu::f128, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabsq)       { return core.ScalarOp2 < fpu::f128, fpu::f128, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabsq)      { return core.ScalarOp2 < fpu::f128, fpu::f128, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsqrtq)      { return core.ScalarOp2RoundingMode < fpu::f128, fpu::f128, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frsqrtq)     { return core.ScalarOp2RoundingMode < fpu::f128, fpu::f128, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndq)       { return core.ScalarOp2RoundingMode < fpu::f128, fpu::f128, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(frndxq)     { return core.ScalarOp2RoundingMode < fpu::f128, fpu::f128, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faddq)       { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fsubq)       { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnaddq)      { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmulq)       { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmulq)      { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdivq)       { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fabdq)     { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnabdq)    { return core.ScalarOp3RoundingMode < fpu::f128, fpu::f128, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminq)       { return core.ScalarOp3 < fpu::f128, fpu::f128, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxq)       { return core.ScalarOp3 < fpu::f128, fpu::f128, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fminnmq)     { return core.ScalarOp3 < fpu::f128, fpu::f128, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaxnmq)     { return core.ScalarOp3 < fpu::f128, fpu::f128, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(faminnmq)    { return core.ScalarOp3 < fpu::f128, fpu::f128, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(famaxnmq)    { return core.ScalarOp3 < fpu::f128, fpu::f128, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmergeq)     { return core.ScalarOp4 < fpu::f128, fpu::f128, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmaddq)      { return core.ScalarOp4RoundingMode < fpu::f128, fpu::f128, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fmsubq)      { return core.ScalarOp4RoundingMode < fpu::f128, fpu::f128, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmaddq)     { return core.ScalarOp4RoundingMode < fpu::f128, fpu::f128, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnmsubq)     { return core.ScalarOp4RoundingMode < fpu::f128, fpu::f128, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoeqq)   { return core.FpScalarCompare<fpu::f128, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmponeq)   { return core.FpScalarCompare<fpu::f128, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoltq)   { return core.FpScalarCompare<fpu::f128, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpogeq)   { return core.FpScalarCompare<fpu::f128, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpueqq)   { return core.FpScalarCompare<fpu::f128, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuneq)   { return core.FpScalarCompare<fpu::f128, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpultq)   { return core.FpScalarCompare<fpu::f128, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpugeq)   { return core.FpScalarCompare<fpu::f128, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpoq)     { return core.FpScalarCompare<fpu::f128, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fcmpuq)     { return core.FpScalarCompare<fpu::f128, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fclassq)     { return core.FpClassify<fpu::f128>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiwtoq)   { return core.ScalarOp2RoundingMode < fpu::f128,  i32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuwtoq)   { return core.ScalarOp2RoundingMode < fpu::f128,  u32, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fidtoq)   { return core.ScalarOp2RoundingMode < fpu::f128,  i64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fudtoq)   { return core.ScalarOp2RoundingMode < fpu::f128,  u64, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fiqtoq)   { return core.ScalarOp2RoundingMode < fpu::f128, i128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fuqtoq)   { return core.ScalarOp2RoundingMode < fpu::f128, u128, cvt_i_f> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtoiw)   { return core.ScalarOp2RoundingMode <  i32, fpu::f128, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtouw)   { return core.ScalarOp2RoundingMode <  u32, fpu::f128, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtoid)   { return core.ScalarOp2RoundingMode <  i64, fpu::f128, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtoud)   { return core.ScalarOp2RoundingMode <  u64, fpu::f128, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtoiq)   { return core.ScalarOp2RoundingMode < i128, fpu::f128, cvt_f_i> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtouq)   { return core.ScalarOp2RoundingMode < u128, fpu::f128, cvt_f_i> (); }

DEFINE_INSN_IMPLEMENTATION(fbroeqh) { return core.FpBranchOnCompare<fpu::f16, fcmp_oeq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroneh) { return core.FpBranchOnCompare<fpu::f16, fcmp_one>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrolth) { return core.FpBranchOnCompare<fpu::f16, fcmp_olt>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrogeh) { return core.FpBranchOnCompare<fpu::f16, fcmp_oge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroh)   { return core.FpBranchOnCompare<fpu::f16, fcmp_o  >(as); }
DEFINE_INSN_IMPLEMENTATION(fbrueqh) { return core.FpBranchOnCompare<fpu::f16, fcmp_ueq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruneh) { return core.FpBranchOnCompare<fpu::f16, fcmp_une>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrulth) { return core.FpBranchOnCompare<fpu::f16, fcmp_ult>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrugeh) { return core.FpBranchOnCompare<fpu::f16, fcmp_uge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruh)   { return core.FpBranchOnCompare<fpu::f16, fcmp_u  >(as); }

DEFINE_INSN_IMPLEMENTATION(fbroeqs) { return core.FpBranchOnCompare<fpu::f32, fcmp_oeq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrones) { return core.FpBranchOnCompare<fpu::f32, fcmp_one>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrolts) { return core.FpBranchOnCompare<fpu::f32, fcmp_olt>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroges) { return core.FpBranchOnCompare<fpu::f32, fcmp_oge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbros)   { return core.FpBranchOnCompare<fpu::f32, fcmp_o  >(as); }
DEFINE_INSN_IMPLEMENTATION(fbrueqs) { return core.FpBranchOnCompare<fpu::f32, fcmp_ueq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrunes) { return core.FpBranchOnCompare<fpu::f32, fcmp_une>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrults) { return core.FpBranchOnCompare<fpu::f32, fcmp_ult>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruges) { return core.FpBranchOnCompare<fpu::f32, fcmp_uge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrus)   { return core.FpBranchOnCompare<fpu::f32, fcmp_u  >(as); }

DEFINE_INSN_IMPLEMENTATION(fbroeqd) { return core.FpBranchOnCompare<fpu::f64, fcmp_oeq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroned) { return core.FpBranchOnCompare<fpu::f64, fcmp_one>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroltd) { return core.FpBranchOnCompare<fpu::f64, fcmp_olt>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroged) { return core.FpBranchOnCompare<fpu::f64, fcmp_oge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrod)   { return core.FpBranchOnCompare<fpu::f64, fcmp_o  >(as); }
DEFINE_INSN_IMPLEMENTATION(fbrueqd) { return core.FpBranchOnCompare<fpu::f64, fcmp_ueq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruned) { return core.FpBranchOnCompare<fpu::f64, fcmp_une>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrultd) { return core.FpBranchOnCompare<fpu::f64, fcmp_ult>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruged) { return core.FpBranchOnCompare<fpu::f64, fcmp_uge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrud)   { return core.FpBranchOnCompare<fpu::f64, fcmp_u  >(as); }

DEFINE_INSN_IMPLEMENTATION(fbroeqq) { return core.FpBranchOnCompare<fpu::f128, fcmp_oeq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroneq) { return core.FpBranchOnCompare<fpu::f128, fcmp_one>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroltq) { return core.FpBranchOnCompare<fpu::f128, fcmp_olt>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrogeq) { return core.FpBranchOnCompare<fpu::f128, fcmp_oge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbroq)   { return core.FpBranchOnCompare<fpu::f128, fcmp_o  >(as); }
DEFINE_INSN_IMPLEMENTATION(fbrueqq) { return core.FpBranchOnCompare<fpu::f128, fcmp_ueq>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruneq) { return core.FpBranchOnCompare<fpu::f128, fcmp_une>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrultq) { return core.FpBranchOnCompare<fpu::f128, fcmp_ult>(as); }
DEFINE_INSN_IMPLEMENTATION(fbrugeq) { return core.FpBranchOnCompare<fpu::f128, fcmp_uge>(as); }
DEFINE_INSN_IMPLEMENTATION(fbruq)   { return core.FpBranchOnCompare<fpu::f128, fcmp_u  >(as); }


DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloeqh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloneh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulolth) { return core.FpNullifyOnCompare<fpu::f16, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulogeh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloh)   { return core.FpNullifyOnCompare<fpu::f16, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulueqh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluneh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnululth) { return core.FpNullifyOnCompare<fpu::f16, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulugeh) { return core.FpNullifyOnCompare<fpu::f16, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluh)   { return core.FpNullifyOnCompare<fpu::f16, fcmp_u  >(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloeqs) { return core.FpNullifyOnCompare<fpu::f32, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulones) { return core.FpNullifyOnCompare<fpu::f32, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulolts) { return core.FpNullifyOnCompare<fpu::f32, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloges) { return core.FpNullifyOnCompare<fpu::f32, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulos)   { return core.FpNullifyOnCompare<fpu::f32, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulueqs) { return core.FpNullifyOnCompare<fpu::f32, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulunes) { return core.FpNullifyOnCompare<fpu::f32, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulults) { return core.FpNullifyOnCompare<fpu::f32, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluges) { return core.FpNullifyOnCompare<fpu::f32, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulus)   { return core.FpNullifyOnCompare<fpu::f32, fcmp_u  >(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloeqd) { return core.FpNullifyOnCompare<fpu::f64, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloned) { return core.FpNullifyOnCompare<fpu::f64, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloltd) { return core.FpNullifyOnCompare<fpu::f64, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloged) { return core.FpNullifyOnCompare<fpu::f64, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulod)   { return core.FpNullifyOnCompare<fpu::f64, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulueqd) { return core.FpNullifyOnCompare<fpu::f64, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluned) { return core.FpNullifyOnCompare<fpu::f64, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulultd) { return core.FpNullifyOnCompare<fpu::f64, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluged) { return core.FpNullifyOnCompare<fpu::f64, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulud)   { return core.FpNullifyOnCompare<fpu::f64, fcmp_u  >(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloeqq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloneq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloltq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulogeq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuloq)   { return core.FpNullifyOnCompare<fpu::f128, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulueqq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluneq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulultq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnulugeq) { return core.FpNullifyOnCompare<fpu::f128, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fnuluq)   { return core.FpNullifyOnCompare<fpu::f128, fcmp_u  >(); }




DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnegh) { return core.VectorOp2 < fpu::f16, fpu::f16, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabsh) { return core.VectorOp2 < fpu::f16, fpu::f16, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabsh) { return core.VectorOp2 < fpu::f16, fpu::f16, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsqrth) { return core.VectorOp2RoundingMode < fpu::f16, fpu::f16, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrsqrth) { return core.VectorOp2RoundingMode < fpu::f16, fpu::f16, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaddh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnaddh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmulh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdivh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmulh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabdh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabdh) { return core.VectorOp3RoundingMode < fpu::f16, fpu::f16, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfminh)  { return core.VectorOp3 < fpu::f16, fpu::f16, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxh)  { return core.VectorOp3 < fpu::f16, fpu::f16, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfminnmh) { return core.VectorOp3 < fpu::f16, fpu::f16, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxnmh) { return core.VectorOp3 < fpu::f16, fpu::f16, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaminnmh) { return core.VectorOp3 < fpu::f16, fpu::f16, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfamaxnmh) { return core.VectorOp3 < fpu::f16, fpu::f16, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergeh) { return core.VectorOp4 < fpu::f16, fpu::f16, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaddh) { return core.VectorOp4RoundingMode < fpu::f16, fpu::f16, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubh) { return core.VectorOp4RoundingMode < fpu::f16, fpu::f16, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmaddh) { return core.VectorOp4RoundingMode < fpu::f16, fpu::f16, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmsubh) { return core.VectorOp4RoundingMode < fpu::f16, fpu::f16, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaddsubh) { return core.VectorOp4RoundingMode < complex<fpu::f16>, complex<fpu::f16>, fmadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubaddh) { return core.VectorOp4RoundingMode < complex<fpu::f16>, complex<fpu::f16>, fmsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaddsubh) { return core.VectorOp3RoundingMode < complex<fpu::f16>, complex<fpu::f16>, fadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubaddh) { return core.VectorOp3RoundingMode < complex<fpu::f16>, complex<fpu::f16>, fsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhaddh) { return core.VectorOp3Horizontal < fpu::f16, fpu::f16, fadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhsubh) { return core.VectorOp3Horizontal < fpu::f16, fpu::f16, fsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhmulh) { return core.VectorOp3Horizontal < fpu::f16, fpu::f16, fmul > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdoth) { return core.VectorOp3DotProduct < fpu::f16, fpu::f16> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoeqh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmponeh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpolth) { return core.FpVectorCompare<i16, fpu::f16, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpogeh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpueqh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpuneh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpulth) { return core.FpVectorCompare<i16, fpu::f16, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpugeh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpuh) { return core.FpVectorCompare<i16, fpu::f16, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergelh) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergehh) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupkhh) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupklh) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfpackh) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrndh) { return core.VectorOp2RoundingMode < fpu::f16, fpu::f16, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrndxh) { return core.VectorOp2RoundingMode < fpu::f16, fpu::f16, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhtouh) { return core.VectorOp2RoundingMode<u16, fpu::f16, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhtoih) { return core.VectorOp2RoundingMode< i16, fpu::f16, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfihtoh) { return core.VectorOp2RoundingMode<fpu::f16,  i16, cvt_i_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfuhtoh) { return core.VectorOp2RoundingMode<fpu::f16, u16, cvt_i_f>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnegs) { return core.VectorOp2 < fpu::f32, fpu::f32, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabss) { return core.VectorOp2 < fpu::f32, fpu::f32, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabss) { return core.VectorOp2 < fpu::f32, fpu::f32, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsqrts) { return core.VectorOp2RoundingMode < fpu::f32, fpu::f32, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrsqrts) { return core.VectorOp2RoundingMode < fpu::f32, fpu::f32, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfadds) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubs) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnadds) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmuls) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdivs) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmuls) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabds) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabds) { return core.VectorOp3RoundingMode < fpu::f32, fpu::f32, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmins) { return core.VectorOp3 < fpu::f32, fpu::f32, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxs) { return core.VectorOp3 < fpu::f32, fpu::f32, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfminnms) { return core.VectorOp3 < fpu::f32, fpu::f32, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxnms) { return core.VectorOp3 < fpu::f32, fpu::f32, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaminnms) { return core.VectorOp3 < fpu::f32, fpu::f32, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfamaxnms) { return core.VectorOp3 < fpu::f32, fpu::f32, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmerges) { return core.VectorOp4 < fpu::f32, fpu::f32, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmadds) { return core.VectorOp4RoundingMode < fpu::f32, fpu::f32, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubs) { return core.VectorOp4RoundingMode < fpu::f32, fpu::f32, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmadds)  { return core.VectorOp4RoundingMode < fpu::f32, fpu::f32, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmsubs)  { return core.VectorOp4RoundingMode < fpu::f32, fpu::f32, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaddsubs) { return core.VectorOp4RoundingMode < complex<fpu::f32>, complex<fpu::f32>, fmadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubadds) { return core.VectorOp4RoundingMode < complex<fpu::f32>, complex<fpu::f32>, fmsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaddsubs) { return core.VectorOp3RoundingMode < complex<fpu::f32>, complex<fpu::f32>, fadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubadds) { return core.VectorOp3RoundingMode < complex<fpu::f32>, complex<fpu::f32>, fsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhadds) { return core.VectorOp3Horizontal < fpu::f32, fpu::f32, fadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhsubs) { return core.VectorOp3Horizontal < fpu::f32, fpu::f32, fsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhmuls) { return core.VectorOp3Horizontal < fpu::f32, fpu::f32, fmul > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdots) { return core.VectorOp3DotProduct < fpu::f32, fpu::f32> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoeqs) { return core.FpVectorCompare<i32, fpu::f32, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpones) { return core.FpVectorCompare<i32, fpu::f32, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpolts) { return core.FpVectorCompare<i32, fpu::f32, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoges) { return core.FpVectorCompare<i32, fpu::f32, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpos) { return core.FpVectorCompare<i32, fpu::f32, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpueqs) { return core.FpVectorCompare<i32, fpu::f32, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpunes) { return core.FpVectorCompare<i32, fpu::f32, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpults) { return core.FpVectorCompare<i32, fpu::f32, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpuges) { return core.FpVectorCompare<i32, fpu::f32, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpus) { return core.FpVectorCompare<i32, fpu::f32, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergels) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergehs) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupkhs) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupkls) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfpacks) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrnds) { return core.VectorOp2RoundingMode < fpu::f32, fpu::f32, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrndxs) { return core.VectorOp2RoundingMode < fpu::f32, fpu::f32, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfstouw) { return core.VectorOp2RoundingMode<u32, fpu::f32, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfstoiw) { return core.VectorOp2RoundingMode< i32, fpu::f32, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfiwtos) { return core.VectorOp2RoundingMode<fpu::f32,  i32, cvt_i_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfuwtos) { return core.VectorOp2RoundingMode<fpu::f32, u32, cvt_i_f>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnegd) { return core.VectorOp2 < fpu::f64, fpu::f64, fneg   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabsd) { return core.VectorOp2 < fpu::f64, fpu::f64, fabs   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabsd) { return core.VectorOp2 < fpu::f64, fpu::f64, fnabs  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsqrtd) { return core.VectorOp2RoundingMode < fpu::f64, fpu::f64, fsqrt  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrsqrtd) { return core.VectorOp2RoundingMode < fpu::f64, fpu::f64, frsqrt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaddd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fadd   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fsub   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnaddd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fnadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmuld) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fmul   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdivd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fdiv   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmuld) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fnmul  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfabdd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fabs_diff > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnabdd) { return core.VectorOp3RoundingMode < fpu::f64, fpu::f64, fnabs_diff> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmind) { return core.VectorOp3 < fpu::f64, fpu::f64, fmin   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxd) { return core.VectorOp3 < fpu::f64, fpu::f64, fmax   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfminnmd) { return core.VectorOp3 < fpu::f64, fpu::f64, fminnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaxnmd) { return core.VectorOp3 < fpu::f64, fpu::f64, fmaxnum> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaminnmd) { return core.VectorOp3 < fpu::f64, fpu::f64, famin  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfamaxnmd) { return core.VectorOp3 < fpu::f64, fpu::f64, famax  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmerged) { return core.VectorOp4 < fpu::f64, fpu::f64, fmerge > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaddd) { return core.VectorOp4RoundingMode < fpu::f64, fpu::f64, fmadd  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubd) { return core.VectorOp4RoundingMode < fpu::f64, fpu::f64, fmsub  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmaddd)  { return core.VectorOp4RoundingMode < fpu::f64, fpu::f64, fnmadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfnmsubd)  { return core.VectorOp4RoundingMode < fpu::f64, fpu::f64, fnmsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmaddsubd) { return core.VectorOp4RoundingMode < complex<fpu::f64>, complex<fpu::f64>, fmadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmsubaddd) { return core.VectorOp4RoundingMode < complex<fpu::f64>, complex<fpu::f64>, fmsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfaddsubd) { return core.VectorOp3RoundingMode < complex<fpu::f64>, complex<fpu::f64>, fadd_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfsubaddd) { return core.VectorOp3RoundingMode < complex<fpu::f64>, complex<fpu::f64>, fsub_alt > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhaddd) { return core.VectorOp3Horizontal < fpu::f64, fpu::f64, fadd > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhsubd) { return core.VectorOp3Horizontal < fpu::f64, fpu::f64, fsub > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfhmuld) { return core.VectorOp3Horizontal < fpu::f64, fpu::f64, fmul > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdotd) { return core.VectorOp3DotProduct < fpu::f64, fpu::f64> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoeqd) { return core.FpVectorCompare<i64, fpu::f64, fcmp_oeq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmponed) { return core.FpVectorCompare<i64, fpu::f64, fcmp_one>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoltd) { return core.FpVectorCompare<i64, fpu::f64, fcmp_olt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpoged) { return core.FpVectorCompare<i64, fpu::f64, fcmp_oge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpod) { return core.FpVectorCompare<i64, fpu::f64, fcmp_o  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpueqd) { return core.FpVectorCompare<i64, fpu::f64, fcmp_ueq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpuned) { return core.FpVectorCompare<i64, fpu::f64, fcmp_une>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpultd) { return core.FpVectorCompare<i64, fpu::f64, fcmp_ult>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpuged) { return core.FpVectorCompare<i64, fpu::f64, fcmp_uge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfcmpud) { return core.FpVectorCompare<i64, fpu::f64, fcmp_u  >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergeld) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfmergehd) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupkhd) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfupkld) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfpackd) { return core.unimplemented(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrndd) { return core.VectorOp2RoundingMode < fpu::f64, fpu::f64, fround > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfrndxd) { return core.VectorOp2RoundingMode < fpu::f64, fpu::f64, froundnx > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdtoud) { return core.VectorOp2RoundingMode<u64, fpu::f64, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfdtoid) { return core.VectorOp2RoundingMode<i64, fpu::f64, cvt_f_i>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfidtod) { return core.VectorOp2RoundingMode<fpu::f64, i64, cvt_i_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(vfudtod) { return core.VectorOp2RoundingMode<fpu::f64, u64, cvt_i_f>(); }

/***************************************************************************
* f128 custom
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(fscaleq)
{
    const int scale = static_cast<int>(core.cmd.rc);
    core.ra() = core.fpu.scalbn(core.rb().sfq(), scale, core.cmd.rm);
    return core.FpuFinalizeInstruction();
}

/***************************************************************************
* all-to-all
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f32,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtoh) { return core.ScalarOp2RoundingMode<fpu::f16, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtos) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f64,  cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtos) { return core.ScalarOp2RoundingMode<fpu::f32, fpu::f128, cvt_f_f>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fqtod) { return core.ScalarOp2RoundingMode<fpu::f64, fpu::f128, cvt_f_f>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtos) { return core.ScalarOp2<fpu::f64,  fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtod) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fhtoq) { return core.ScalarOp2<fpu::f128, fpu::f16, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstod) { return core.ScalarOp2<fpu::f64,  fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fstoq) { return core.ScalarOp2<fpu::f128, fpu::f32, extend>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(fdtoq) { return core.ScalarOp2<fpu::f128, fpu::f64, extend>(); }

} //namespace postrisc
