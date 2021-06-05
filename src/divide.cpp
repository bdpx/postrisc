#include "pch.hpp"
#include "util/fmt.hpp"

namespace postrisc {
// intrinsic function for narrowing 128-bit by 64-bit division

struct __uint128_t {
    __uint128_t() {}
    __uint128_t(u64 h, u64 l) : low(l), high(h) {}

    bool operator==(const __uint128_t& r) const { return low == r.low && high == r.high; }
    friend std::ostream& operator<<(std::ostream& out, const __uint128_t& r) {
       return out << fmt::hex(r.high) << ' ' << fmt::hex(r.low);
    }

    u64 low, high;
};

static inline u64 _udiv128(u64 DividendHigh, u64 DividendLow, u64 Divisor, u64 *Remainder)
{
    u128 d = ((u128)DividendHigh << 64) | DividendLow;
    *Remainder = d % Divisor;
    return d / Divisor;
}

static inline u64 _umul128(u64 Multiplier, u64 Multiplicand, u64 *HighProduct)
{
    u128 prod = (u128)Multiplier * (u128)Multiplicand;
    *HighProduct = prod >> 64;
    return (u64)prod;
}

static inline u64 __shiftleft128 (u64 LowPart, u64 HighPart, unsigned Shift) { return (HighPart << Shift) | (LowPart >> (64 - Shift)); }
static inline u64 __shiftright128(u64 LowPart, u64 HighPart, unsigned Shift) { return (HighPart >> (64 - Shift)) | (LowPart << Shift); }

std::tuple<u128, u128>
divrem_u(u128 DividendHigh, u128 DividendLow, u128 Divisor)
{
    u64 temp;
    u128  Quotient, Remainder;

    u64 u0 = (u64)DividendLow;
    u64 u1 = DividendLow >> 64;
    u64 u2 = DividendHigh;
    u64 u3 = DividendHigh >> 64;
    u64 v0 = Divisor;
    u64 v1 = Divisor >> 64;

    if (!(v0 | v1)              // division by 0?
     || !(u3 < v1 || u3 == 0 && u2 < v0)) {
        std::cerr << "quotient overflow?" << std::endl;
        return {Quotient, Remainder};               // quotient overflow?
    }

    if (v1 == 0) {
        // divisor < 2**64, dividend < 2**192 - 1:
        //  perform "long" division
        u64 high = _udiv128(u2, u1, v0, &temp);
        u64 low = _udiv128(temp, u0, v0, &temp);
        Quotient = ((u128)high << 64) | low;
        low = temp;
        high = 0;
        Remainder = ((u128)high << 64) | low;

        std::cerr << "v1 == 0" << std::endl;
        return { Quotient, Remainder };
    }

    // normalize divisor
    const unsigned shift = __builtin_clzll(v1);
    if (shift != 0) {
        u3 = __shiftleft128(u2, u3, shift);
        u2 = __shiftleft128(u1, u2, shift);
        u1 = __shiftleft128(u0, u1, shift);
        u0 <<= shift;
        v1 = __shiftleft128(v0, v1, shift);
        v0 <<= shift;
    }

    u64 phat, rhat;

    // compute high quotient digit
    u64 qhat = _udiv128(u3, u2, v1, &rhat);

    while (_umul128(qhat, v0, &temp) > u1 && temp == rhat || temp > rhat) {
        qhat -= 1;
        rhat += v1;
        if (rhat < v1)
            break;
    }
    const u64 q_high = qhat;

    u64 high, low;
    // multiply and subtract:
    //  compute intermediate remainder
    u2 -= qhat * v1;
    low = _umul128(qhat, v0, &high);
    u2 -= high + (u1 < low);
    u1 -= low;

    // compute low quotient digit
    phat = u2 >= v1;
    qhat = _udiv128(u2 - v1 * phat, u1, v1, &rhat);

    while (phat != 0 || _umul128(qhat, v0, &temp) > u0 && temp == rhat || temp > rhat) {
        phat -= qhat == 0;
        qhat -= 1;
        rhat += v1;
        if (rhat < v1)
            break;
    }
    const u64 q_low = qhat;
    Quotient = (((u128)q_high) << 64) | q_low;

    //  compute remainder
    u1 -= qhat * v1;
    low = _umul128(qhat, v0, &high);
    u1 -= high + (u0 < low);
    u0 -= low;

    low = __shiftright128(u0, u1, shift);
    high = u1 >> shift;
    Remainder = (((u128)high) << 64) | low;

    return { Quotient, Remainder };
}

std::tuple<u128, u128> divrem_u_naive(u128 a1, u128 a0, u128 b)
{
    size_t const F_SIZE = 8 * sizeof(u128);

    assert(a1 < b);
    for (size_t i = 0; i < F_SIZE; i++) {
        int qb, ab;
        ab = (int)(a1 >> (F_SIZE - 1));
        a1 = (a1 << 1) | (a0 >> (F_SIZE - 1));
        if (ab || a1 >= b) {
            a1 -= b;
            qb = 1;
        } else {
            qb = 0;
        }
        a0 = (a0 << 1) | qb;
    }
    return {a0, a1};
}

std::tuple<u128, u128> divlu2(u128 u1, u128 u0, u128 v) {
   const u128 b = ((u128)1) << 64; // Number base (64 bits).
   u128 un1, un0,        // Norm. dividend LSD's.
            vn1, vn0,        // Norm. divisor digits.
            q1, q0,          // Quotient digits.
            un32, un21, un10,// Dividend digit pairs.
            rhat;            // A remainder.

   if (u1 >= v) {            // If overflow, set rem to an impossible value, and return the largest possible quotient.
      return { 0xFFFFFFFF, 0xFFFFFFFF };
   }

   const int s = __builtin_clzll(v >> 64);  // 0 <= s <= 63.
   if (s) {
     v = v << s;               // Normalize divisor.
   }
   vn1 = v >> 64;        // Break divisor up into
   vn0 = (u64)v;         // two 16-bit digits.

   if (s) {
     // Shift dividend left.
     un32 = (u1 << s) | (u0 >> (64 - s));
     un10 = u0 << s;
   } else {
     un32 = u1;
     un10 = u0;
   }

   un1 = un10 >> 64;         // Break right half of
   un0 = (u64)un10;      // dividend into two digits.

   q1 = un32/vn1;            // Compute the first
   rhat = un32 - q1*vn1;     // quotient digit, q1.
again1:
   if (q1 >= b || q1*vn0 > b*rhat + un1) {
     q1 = q1 - 1;
     rhat = rhat + vn1;
     if (rhat < b) goto again1;}

   un21 = un32*b + un1 - q1*v;  // Multiply and subtract.

   q0 = un21/vn1;            // Compute the second
   rhat = un21 - q0*vn1;     // quotient digit, q0.
again2:
   if (q0 >= b || q0*vn0 > b*rhat + un0) {
     q0 = q0 - 1;
     rhat = rhat + vn1;
     if (rhat < b) goto again2;}

   return {q1*b + q0, (un21*b + un0 - q0*v) >> s };
}


void test(u128 hi, u128 lo, u128 div)
{
    std::cout << "hi:   " << fmt::hex(hi) << std::endl;
    std::cout << "lo:   " << fmt::hex(lo) << std::endl;
    std::cout << "div:  " << fmt::hex(div) << std::endl;

    const auto [true_quot, true_rem] = divrem_u_naive(hi, lo, div);

    std::cout << "tquot: " << fmt::hex(true_quot) << std::endl;
    std::cout << "trem:  " << fmt::hex(true_rem) << std::endl;

    const auto [test_quot, test_rem] = divrem_u(hi, lo, div);

    std::cout << "aquot: " << fmt::hex(test_quot) << std::endl;
    std::cout << "arem:  " << fmt::hex(test_rem) << std::endl;

    const auto [x_quot, x_rem] = divlu2(hi, lo, div);

    std::cout << "xquot: " << fmt::hex(x_quot) << std::endl;
    std::cout << "xrem:  " << fmt::hex(x_rem) << std::endl;

    if (true_quot == test_quot && true_rem == test_rem) {
        std::cout << "test success" << std::endl;
    } else {
        std::cerr << "divrem_u error:" << fmt::hex(hi) << ' ' << fmt::hex(lo) << ' ' << fmt::hex(div) << '\n'
                  << "true: " << fmt::hex(true_quot) << ' ' << fmt::hex(true_rem) << '\n'
                  << "test: " << fmt::hex(test_quot) << ' ' << fmt::hex(test_rem) << '\n';
    }
}

void test()
{
    const u128  hi = (static_cast<u128>(U64C(0x4000000000000001)) << 64) | U64C(0x0000000000000000);
    const u128  lo = (static_cast<u128>(U64C(0x0000000000000000)) << 64) | U64C(0x0000000000000000);
    const u128 div = (static_cast<u128>(U64C(0x8000000000000000)) << 64) | U64C(0xFFFFFFFFFFFFFFFF);

    test(hi, lo, div);
}

} // namespace postrisc

int main()
{
    postrisc::test();
    return 0;
}
