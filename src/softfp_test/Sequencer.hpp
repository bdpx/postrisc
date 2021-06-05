#pragma once

#include <random>
#include <boost/test/unit_test.hpp>

#include "emulator/SoftFpu.hpp"
#include "Controller.hpp"
#include "ValuePrinter.hpp"

namespace postrisc {
namespace fpu {

class Sequence {
public:
    unsigned expNum = 0;
    u64 term1Num = 0;
    template<typename T> T uint_next(u8 nbits, u32 index);
    template<typename T> void next(T&);
};

template<typename T> T Sequence::uint_next(u8 nbits, u32 index)
{
    int bitnum = (index >> 3) % (nbits + 1);
    T value = T{1} << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;
    if (index & 4) value |= (T{1} << (8*sizeof(T)-2));

    T mask = (T{1} << nbits) - 1;
    value = value & mask;
    return value;
}


template<typename FLOAT> constexpr auto special_sfs(void) {
    return std::to_array({
        SoftFPU::pack_sf<FLOAT>(0, 0, 0), // zero
        SoftFPU::pack_sf<FLOAT>(1, 0, 0),
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::EXP_MASK, 0), // INF
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK, 0),
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::BIAS, 0), // normal (1.0)
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::BIAS, 0),
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::BIAS+6, 0), // normal
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::BIAS+6, 0),
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::BIAS-6, 0), // normal
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::BIAS-6, 0),
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::EXP_MASK, FLOAT::QNAN_MASK), //QNAN
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK),//QNAN
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::EXP_MASK, FLOAT::QNAN_MASK|4), //QNAN, nontrivial
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK|4), //QNAN, nontrivial
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::EXP_MASK, 1|4), // SNAN, nontrivial
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK, 1|4), // SNAN, nontrivial
        SoftFPU::pack_sf<FLOAT>(0, FLOAT::EXP_MASK, 1), // SNAN
        SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK, 1), // SNAN
        SoftFPU::pack_sf<FLOAT>(0, 0, 1), // subnormal
        SoftFPU::pack_sf<FLOAT>(1, 0, 1),
        SoftFPU::pack_sf<FLOAT>(0, 0, 0xFF), // subnormal
        SoftFPU::pack_sf<FLOAT>(1, 0, 0xFF),
    });
}

template<typename INTEGER> constexpr auto special_ints(void) {
    return std::to_array({
        ~static_cast<INTEGER>(0),
        -static_cast<INTEGER>(1) << (8*sizeof(INTEGER)-1),
        -static_cast<INTEGER>(1) << (8*sizeof(INTEGER)-2),
        -static_cast<INTEGER>(0xFFFFFF),
        -static_cast<INTEGER>(10000000),
        static_cast<INTEGER>(0),
        static_cast<INTEGER>(10000000),
        static_cast<INTEGER>(0xFFFFFF),
        static_cast<INTEGER>(1) << (8*sizeof(INTEGER)-2),
        static_cast<INTEGER>(1) << (8*sizeof(INTEGER)-1),
    });
}


/* random integer with long sequences of '0' and '1' */
template<typename UINT> UINT rrandom_u(size_t len)
{
    size_t bit, pos = 0, n, end;
    UINT a = 0;

    bit = rand() & 1;
    for(;;) {
        n = (rand() % len) + 1;
        end = pos + n;
        if (end > len)
            end = len;
        if (bit) {
            n = end - pos;
            if (n == 8 * sizeof(UINT))
                a = -(UINT)1;
            else
                a |= (((UINT)1 << n) - 1) << pos;
        }
        if (end >= len)
            break;
        pos = end;
        bit ^= 1;
    }
    return a;
}

template<typename FLOAT> FLOAT rrandom_sf(void)
{
    u32 a_exp, a_sign;
    a_sign = rand() & 1;

    /* generate exponent close to the min/max more often than random */
    switch(rand() & 15) {
    case 0:
        a_exp = (rand() % (2 * FLOAT::FRAC_SIZE)) & FLOAT::EXP_MASK;
        break;
    case 1:
        a_exp = (FLOAT::EXP_MASK - (rand() % (2 * FLOAT::FRAC_SIZE))) & FLOAT::EXP_MASK;
        break;
    default:
        a_exp = rand() & FLOAT::EXP_MASK;
        break;
    }
    typename FLOAT::F_UINT a_frac = rrandom_u<typename FLOAT::F_UINT>(FLOAT::FRAC_SIZE);
    return SoftFPU::pack_sf<FLOAT>(a_sign, a_exp, a_frac);
}


template<typename T> void Sequence::next(T& z)
{
    if (Controller::s_maxCount < 100000000) {
        if constexpr (fp_traits<T>::is_fp()) {
            z = rrandom_sf<T>();
        } else {
            z = rrandom_u<T>(8 * sizeof(T));
        }
        term1Num++;
        return;
    }

    term1Num++;
    if constexpr (fp_traits<T>::is_fp()) {
        static constexpr const int exponent[] = {
            -T::BIAS, // +0, subnormal
            -T::BIAS+1,
            -T::BIAS+2,
            -T::BIAS/2, -T::BIAS/2-1, -T::BIAS/2-2,
            -T::BIAS/3, -T::BIAS/3-1,
            -4, -3, -2, -1,
            0,
            1, 2, 3, 4,
            T::BIAS/3,
            T::BIAS/2-1, T::BIAS/2,
            T::BIAS-1, T::BIAS,
            T::BIAS+1, //infinity or NaN
        };

        typename T::F_UINT frac = uint_next<typename T::F_UINT>(T::FRAC_SIZE, term1Num);
        frac |= (typename T::F_UINT)(term1Num & 1) << (T::F_SIZE - 1);
        u32 exp = exponent[expNum] & ((u32{1} << T::EXP_SIZE) -1 );
        frac |= (typename T::F_UINT)(exp + T::BIAS) << T::FRAC_SIZE;

        z = T{frac};
        ++expNum;
        if (sizearray(exponent) <= expNum) {
            expNum = 0;
        }
    } else { // integer
        z = uint_next<T>(8 * sizeof(T), term1Num)
          + uint_next<T>(8 * sizeof(T), term1Num ^ 0xdeadbeaf);
    }
}

class Sequencer {
public:
    Sequencer(void);
    ~Sequencer(void);

public:
    // setup start values in sequences
    template<typename T> void start(T& a);
    template<typename T> void start(T& a, T& b);
    template<typename T> void start(T& a, T& b, T& c);

    // get next values in sequence
    template<typename T> void next(T& a);
    template<typename T> void next(T& a, T& b);
    template<typename T> void next(T& a, T& b, T& c);

private:
    static const u64 ROLL_STEP = 16;
    Sequence sequenceA;
    Sequence sequenceB;
    Sequence sequenceC;
    u64   count = 0;
};

template<typename T> void Sequencer::next(T& a)
{
    sequenceA.next(a);
    ++count;
}

template<typename T> void Sequencer::next(T& a, T& b)
{
    sequenceA.next(a);
    if (count % ROLL_STEP == 0)
        sequenceB.next(b);
    ++count;
}

template<typename T> void Sequencer::next(T& a, T& b, T& c)
{
    sequenceA.next(a);
    if (count % ROLL_STEP == 0) {
        sequenceB.next(b);
        if (count % (ROLL_STEP * ROLL_STEP) == 0)
            sequenceC.next(c);
    }
    ++count;
}


template<typename T> bool is_int_nan(T v)
{
    static_assert(std::numeric_limits<T>::is_integer);
    return v == std::numeric_limits<T>::max() ||
           v == std::numeric_limits<T>::min();
}

template<typename T>
bool is_ok(T trueZ, fp_flags trueFlags, T testZ, fp_flags testFlags)
{
    bool const is_flags_ok = !Controller::s_checkFlags || trueFlags == testFlags;

    if constexpr (fp_traits<T>::is_fp()) {
        if (trueZ.val() == testZ.val())
            return is_flags_ok;
        if (!Controller::s_checkNaNs) {
            if (is_nan(trueZ) && is_nan(testZ))
                return is_flags_ok;
        }
        return false;
    } else { // integer, bool
        if (trueZ == testZ)
            return is_flags_ok;
        // if booleans differ, it's enough to fail, otherwise check NaN values
        if constexpr (std::is_same<T, bool>::value)
            return false;
        if (!Controller::s_checkIntNaNs) {
            // integer NaNs for host may be wrong/different, invalid flags check may be enough
            if ((testFlags & trueFlags & FFLAG_INVALID) == FFLAG_INVALID)
                return is_flags_ok;
            //if (is_int_nan(trueZ) && is_int_nan(testZ)) return is_flags_ok;
        }
        return false;
    }
    return false;
}

} //namespace fpu
} // namespace postrisc
