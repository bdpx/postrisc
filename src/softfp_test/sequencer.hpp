#pragma once

#include <random>

#include "emulator/SoftFpu.hpp"

namespace postrisc {
namespace fpu {

class Sequencer {
public:
    unsigned expNum = 0;
    unsigned term1Num = 0;
    template<typename T> T uint_next(u8 nbits, u32 index);
    template<typename T> void next(T&, int step);
};

template<typename T> T Sequencer::uint_next(u8 nbits, u32 index)
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

template<typename T> void Sequencer::next(T& z, int step)
{
    term1Num += step;
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

        typename T::F_UINT frac = uint_next<typename T::F_UINT>(T::MANT_SIZE, term1Num);
        frac |= (typename T::F_UINT)(term1Num & 1) << (T::F_SIZE - 1);
        u32 exp = exponent[expNum] & ((u32{1} << T::EXP_SIZE) -1 );
        frac |= (typename T::F_UINT)(exp + T::BIAS) << T::MANT_SIZE;

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

class TestSequencer {
public:
    TestSequencer(void);
    ~TestSequencer(void);
    void setup(void);
    void teardown(void);

public:
    // setup start values in sequences
    template<typename T> void start(T& a);
    template<typename T> void start(T& a, T& b);
    template<typename T> void start(T& a, T& b, T& c);

    // get next values in sequence
    template<typename T> void next(T& a);
    template<typename T> void next(T& a, T& b);
    template<typename T> void next(T& a, T& b, T& c);

public:
    template<typename T> bool is_ok(T trueZ, fp_flags trueFlags, T testZ, fp_flags testFlags);

    template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU> void test_1 (TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name);
    template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU> void test_1a(TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name);
    template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU> void test_2 (TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name);
    template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU> void test_3 (TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name);

private:
    Sequencer sequenceA;
    Sequencer sequenceB;
    Sequencer sequenceC;

    u64   total = 0;
    u32   errorCount = 0;
};

template<typename T> void TestSequencer::next(T& a)
{
    sequenceA.next(a, 1);
}

template<typename T> void TestSequencer::next(T& a, T& b)
{
    sequenceA.next(a, 1);
    sequenceB.next(b, 5);
}

template<typename T> void TestSequencer::next(T& a, T& b, T& c)
{
    sequenceA.next(a, 1);
    sequenceB.next(b, 5);
    sequenceC.next(c, 7);
}

} //namespace fpu
} // namespace postrisc
