#pragma once

#include <cmath>
#include <cfenv> // for fegetround/fesetround/fegetexceptflag/etc

#include "emulator/SoftFpu.hpp"

namespace postrisc {
namespace fpu {

/********************************************************************
*  Hardware FPU class
********************************************************************/
class HardFPU
{
private:
    RoundingModeEnum m_rounding_mode = RM_RNE;
public:
    void clear_flags(fp_flags mask = FFLAG_ALL_EXCEPT);
    RoundingModeEnum get_rounding_mode(void) const { return m_rounding_mode; }
    void set_rounding_mode(RoundingModeEnum val);
    fp_flags get_flags(void) const;
    void raise(int flags) { std::feraiseexcept(flags); }

    static bool avx_fp16_supports() { return __builtin_cpu_supports("avx512fp16"); }

    // use current rounding mode (rint-like functions)
    template<typename T> T round(T a, RoundingModeEnum UNUSED(rm), bool inexact);

    template<typename FLOAT> FLOAT add(FLOAT a, FLOAT b, RoundingModeEnum UNUSED(rm));
    template<typename FLOAT> FLOAT sub(FLOAT a, FLOAT b, RoundingModeEnum UNUSED(rm));
    template<typename FLOAT> FLOAT mul(FLOAT a, FLOAT b, RoundingModeEnum UNUSED(rm));
    template<typename FLOAT> FLOAT div(FLOAT a, FLOAT b, RoundingModeEnum UNUSED(rm));

    template<typename FLOAT> FLOAT min(FLOAT a, FLOAT b) { return FLOAT::soft( std::min(a.hard(), b.hard()) ); }
    template<typename FLOAT> FLOAT max(FLOAT a, FLOAT b) { return FLOAT::soft( std::max(a.hard(), b.hard()) ); }

    template<typename FLOAT> FLOAT muladd(FLOAT a, FLOAT b, FLOAT c, RoundingModeEnum UNUSED(rm), muladd_negate_t UNUSED(negate));
    template<typename FLOAT> FLOAT sqrt(FLOAT a, RoundingModeEnum UNUSED(rm));
    template<typename FLOAT> FLOAT exp(FLOAT a, RoundingModeEnum UNUSED(rm));
    template<typename FLOAT> FLOAT log2(FLOAT a, RoundingModeEnum UNUSED(rm));

    // for conversions - just simple assignment
    template<typename FLOAT, typename INTEGER> FLOAT cvt_i_f(INTEGER a, RoundingModeEnum rm); // { return FLOAT::soft( (typename FLOAT::F_HARD)(a) ); }
    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST extend(FLOAT_SRC a) { return FLOAT_DST::soft((typename FLOAT_DST::F_HARD)(a.hard())); }
    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST cvt_f_f(FLOAT_SRC a, RoundingModeEnum UNUSED(rm));
    template<typename INTEGER, typename FLOAT> INTEGER cvt_f_i(FLOAT a, RoundingModeEnum rm);

    template<typename FLOAT> FLOAT abs(FLOAT v);
    template<typename FLOAT> FLOAT chs(FLOAT v);
    template<typename FLOAT> FLOAT nabs(FLOAT v);
    template<typename FLOAT> FLOAT merge(FLOAT sign, FLOAT exponent, FLOAT mantissa);

    template<typename FLOAT> bool compare_ordered(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return false;
        }
        return true;
    }
    template<typename FLOAT> bool compare_ordered_eq(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return false;
        }
        return a.hard() == b.hard();
    }
    template<typename FLOAT> bool compare_ordered_ne(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return false;
        }
        return a.hard() != b.hard();
    }
    template<typename FLOAT> bool compare_ordered_lt(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return false;
        }
        return a.hard() < b.hard();
    }
    template<typename FLOAT> bool compare_ordered_ge(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return false;
        }
        return a.hard() >= b.hard();
    }

    template<typename FLOAT> bool compare_unordered(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return true;
        }
        return false;
    }
    template<typename FLOAT> bool compare_unordered_eq(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return true;
        }
        return a.hard() == b.hard();
    }
    template<typename FLOAT> bool compare_unordered_ne(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return true;
        }
        return a.hard() != b.hard();
    }
    template<typename FLOAT> bool compare_unordered_lt(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return true;
        }
        return a.hard() < b.hard();
    }
    template<typename FLOAT> bool compare_unordered_ge(FLOAT a, FLOAT b) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b))
                raise(FE_INVALID);
            return true;
        }
        return a.hard() >= b.hard();
    }
};

template<typename FLOAT> FLOAT HardFPU::abs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v & ~(typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

template<typename FLOAT> FLOAT HardFPU::chs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v ^ (typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

template<typename FLOAT> FLOAT HardFPU::nabs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v | (typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

} // namespace fpu
} // namespace postrisc
