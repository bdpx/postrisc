#pragma once

#include "Result.hpp"

namespace postrisc {

/***************************************************************************
* this function compute high 64 bits of full 128-bit multiplier.
***************************************************************************/
class multiply_high { public: static u64 func(u64 left, u64 right)
{
    // (lh,ll) x (rh,rl)
    u64   lh, ll, rh, rl;

    ll = left & util::makemask<u64>(32);
    lh = (left >> 32);
    rl = right & util::makemask<u64>(32);
    rh = (right >> 32);

    return (lh * rh + ((lh * rl) >> 32) + ((ll * rh) >> 32));
}};

/***************************************************************************
* basic bitwise functions
***************************************************************************/
inline unsigned int get_shift_amount(u64 num)
{
    return static_cast<unsigned>(num & util::makemask<u64>(bits_per_shamt));
}

class neg             { public: template<typename T> static T func(T a) { return -a; }};
class add             { public: template<typename T> static T func(T a, T b) { return    a + b;   }};
class nadd            { public: template<typename T> static T func(T a, T b) { return - (a + b);  }};
class subtract        { public: template<typename T> static T func(T a, T b) { return    a - b;   }};
class subtract_rev    { public: template<typename T> static T func(T a, T b) { return    b - a;   }};
class mul             { public: template<typename T> static T func(T a, T b) { return    a * b;   }};
class div             { public: template<typename T> static T func(T a, T b) { return    a / b;   }};
class mod             { public: template<typename T> static T func(T a, T b) { return    a % b;   }};
class sign_extend     { public: template<typename T> static T func(T a) { return a; }}; // do nothing
class zero_extend     { public: template<typename T> static T func(T a) { return a; }}; // do nothing
class bitwise_and     { public: template<typename T> static T func(T a, T b) { return    a  & b ; }};
class bitwise_not     { public: template<typename T> static T func(T a) { return ~a; }};
class bitwise_or      { public: template<typename T> static T func(T a, T b) { return    a  | b ; }};
class bitwise_xor     { public: template<typename T> static T func(T a, T b) { return    a  ^ b ; }};
class bitwise_nand    { public: template<typename T> static T func(T a, T b) { return ~( a  & b); }};
class bitwise_nor     { public: template<typename T> static T func(T a, T b) { return ~( a  | b); }};
class bitwise_xnor    { public: template<typename T> static T func(T a, T b) { return ~( a  ^ b); }};
class bitwise_andn    { public: template<typename T> static T func(T a, T b) { return  (~a) & b ; }};  // non-commutative!!!
class bitwise_orn     { public: template<typename T> static T func(T a, T b) { return  (~a) | b ; }};  // non-commutative!!!
class bitwise_andneg  { public: template<typename T> static T func(T a, T b) { return  a & (T(0) - b); }};
class bitwise_xordec  { public: template<typename T> static T func(T a, T b) { return  a ^ (b-1); }};
class bitwise_anddec  { public: template<typename T> static T func(T a, T b) { return  a & (b-1); }};
class min             { public: template<typename T> static T func(T a, T b) { return  std::min(a, b); }};
class max             { public: template<typename T> static T func(T a, T b) { return  std::max(a, b); }};
class shift_left      { public: template<typename T> static T func(T a, T b) { b &= (8*sizeof(T)-1); return (a << b); }};
class shift_right     { public: template<typename T> static T func(T a, T b) { b &= (8*sizeof(T)-1); return (a >> b); }};
class divide_pow2     { public: template<typename T> static T func(T a, T b) { b &= (8*sizeof(T)-1); return a / (static_cast<T>(1) << b); }};
class rotate_left     { public: template<typename T> static T func(T a, T b) { b &= (8*sizeof(T)-1); return (a << b) | (a >> (8*sizeof(T) - b)); }};
class rotate_right    { public: template<typename T> static T func(T a, T b) { b &= (8*sizeof(T)-1); return (a >> b) | (a << (8*sizeof(T) - b)); }};
class bit_set         { public: template<typename T> static T func(T a, T b) { return a | (T(1) << b); }};
class bit_clear       { public: template<typename T> static T func(T a, T b) { return a & ~(T(1) << b); }};
class bit_flip        { public: template<typename T> static T func(T a, T b) { return a ^ (T(1) << b); }};

class shift_left_immediate      { public: template<typename T> static T func(T a, unsigned b) { return (a << b); }};
class shift_right_immediate     { public: template<typename T> static T func(T a, unsigned b) { return (a >> b); }};
class divide_pow2_immediate     { public: template<typename T> static T func(T a, unsigned b) { return a / (static_cast<T>(1) << b); }};
class bit_set_immediate         { public: template<typename T> static T func(T a, unsigned b) { return a | (static_cast<T>(1) << b); }};
class bit_clear_immediate       { public: template<typename T> static T func(T a, unsigned b) { return a & ~(static_cast<T>(1) << b); }};
class bit_flip_immediate        { public: template<typename T> static T func(T a, unsigned b) { return a ^ (static_cast<T>(1) << b); }};

class cmp_eq          { public: template<typename T> static bool func(T x, T y) { return x == y; }};
class cmp_ne          { public: template<typename T> static bool func(T x, T y) { return x != y; }};
class cmp_lt          { public: template<typename T> static bool func(T x, T y) { return x <  y; }};
class cmp_le          { public: template<typename T> static bool func(T x, T y) { return x <= y; }};
class cmp_gt          { public: template<typename T> static bool func(T x, T y) { return x >  y; }};
class cmp_ge          { public: template<typename T> static bool func(T x, T y) { return x >= y; }};

class cmp_mask_all    { public: template<typename T> static bool func(T x, T y) { return (x & y) == y; }};
class cmp_mask_any    { public: template<typename T> static bool func(T x, T y) { return (x & y) != static_cast<T>(0); }};
class cmp_mask_none   { public: template<typename T> static bool func(T x, T y) { return (x & y) == static_cast<T>(0); }};
class cmp_mask_notall { public: template<typename T> static bool func(T x, T y) { return (x & y) != y; }};

class addadd          { public: template<typename T> static T func(T a, T b, T c) { return a + b + c; }};
class addsub          { public: template<typename T> static T func(T a, T b, T c) { return a + b - c; }};
class subsub          { public: template<typename T> static T func(T a, T b, T c) { return a - b - c; }};
class muladd          { public: template<typename T> static T func(T a, T b, T c) { return a * b + c; }};
class mulsub          { public: template<typename T> static T func(T a, T b, T c) { return a * b - c; }};
class mulsub_rev      { public: template<typename T> static T func(T a, T b, T c) { return c - a * b; }};

// ((0x33333333 ^ 0x55555555) & 0xff00ff00) ^ 0x55555555 = 0x33553355
class bitwise_select  { public: template<typename T> static T func(T a, T b, T amask) { return ((a ^ b) & amask) ^ b; }};

class shifted_add          { public: template<typename T> static T func(T a, T b, unsigned shamt) { return a + (b << shamt); }};
class shifted_subtract     { public: template<typename T> static T func(T a, T b, unsigned shamt) { return a - (b << shamt); }};
class shifted_subtract_rev { public: template<typename T> static T func(T a, T b, unsigned shamt) { return (b << shamt) - a; }};
class shifted_or           { public: template<typename T> static T func(T a, T b, unsigned shamt) { return a | (b << shamt); }};
class shifted_xor          { public: template<typename T> static T func(T a, T b, unsigned shamt) { return a ^ (b << shamt); }};

//===----------------------------------------------------------------------===//
// funnel shifts (LLVM)
// FSHL returns high part after left shift
// FSHR returns low part after right shift
// fshl(HI,LO,Z): (HI <<       (Z % BW) ) | (LO >> (BW - (Z % BW)))
// fshr(HI,LO,Z): (HI << (BW - (Z % BW))) | (LO >>       (Z % BW) )
//===----------------------------------------------------------------------===//
class shift_left_pair      { public: template<typename T> static T func(T a, T b, T shamt)        { shamt &= (8*sizeof(T)-1);  return (a << shamt) | (b >> (8*sizeof(T) - shamt)); }};
class shift_left_pair_imm  { public: template<typename T> static T func(T a, T b, unsigned shamt) { shamt &= (8*sizeof(T)-1);  return (a << shamt) | (b >> (8*sizeof(T) - shamt)); }};

class shift_right_pair     { public: template<typename T> static T func(T a, T b, T shamt)        { shamt &= (8*sizeof(T)-1);  return (a << (8*sizeof(T) - shamt)) | (b >> shamt); }};
class shift_right_pair_imm { public: template<typename T> static T func(T a, T b, unsigned shamt) { shamt &= (8*sizeof(T)-1);  return (a << (8*sizeof(T) - shamt)) | (b >> shamt); }};

class add_carry {
public:
template<typename T> static T func(T x, T y)
{
    return (T)(x+y) < x;
}
template<typename T> static T func(T x, T y, T z)
{
    return func(x, y) + func(x+y, z);
}
};

class subtract_borrow { public: template<typename T> static T func(T x, T y)
{
    return (T)(x-y) > x;
}

template<typename T> static T func(T x, T y, T z)
{
    return func(x, y) + func(x-y, z);
}
};

class absolute_value { public: template<typename T> static T func(T x)
{
    return (x < 0) ? -x : x;
}};

class absolute_difference { public: template<typename T> static T func(T x, T y)
{
    return (x < y) ? (y - x) : (x - y);
}};

class add_unsigned_saturated { public: template<typename T> static T func(T x, T y)
{
    T sum = x + y;
    const T uintmax = std::numeric_limits<T>::max(); //~T(0);
    if (x > uintmax - y) sum = uintmax;
    return sum;
}};

class add_signed_saturated { public: template<typename T> static T func(T x, T y)
{
    T sum = x + y;
    const T intmin = std::numeric_limits<T>::min(); //T(T(1) << (8*sizeof(T)-1));
    const T intmax = std::numeric_limits<T>::max(); //~intmin;

    if (y > 0) {
       if (x > intmax - y) sum = intmax;
    } else {
       if (x < intmin - y) sum = intmin;
    }
    return sum;
}};

class subtract_unsigned_saturated { public: template<typename T> static T func(T x, T y)
{
    T diff = x - y;
    if (x < y) diff = 0;
    return diff;
}};

class  subtract_signed_saturated { public: template<typename T> static T func(T x, T y)
{
    T diff = x - y;
    const T intmin = std::numeric_limits<T>::min(); //(T) ( T(1) << (8*sizeof(T)-1) );
    const T intmax = std::numeric_limits<T>::max(); //~intmin;

    if (y < 0) {
       if (x > intmax + y) diff = intmax;
    } else {
       if (x < intmin + y) diff = intmin;
    }
    return diff;
}};

class add_overflow { public: template<typename T> static T func(T x, T y)
{
    T sum = x + y;
    return  ((x ^ sum) & (y ^ sum)) >> (8*sizeof(T)-1);
}};

class subtract_overflow { public: template<typename T> static T func(T x, T y)
{
    return add_overflow::func(x, T(~y));
}};


/***************************************************************************
unsigned averages:
   (a+b)/2 = avgu(a,b) = (a & b) + ((a ^ b) >> 1)
   (a+b+1)/2 = ~avgu(~a,~b)
***************************************************************************/
class average_unsigned { public: template<typename T> static T func(T x, T y)
{
    x = ~x;
    y = ~y;
    return ~((x & y) + ((x ^ y) >> 1));
}};

/***************************************************************************
signed averages:
   (a+b+1)/1 = (a | b) - ((a ^ b) >>signed 1)
   (a+b)/2 = bias(avg(bias(a),bias(b))), bias(x) = sign ^ x
   (a+b+1)/2 = (a & b & 1) + (a >>signed 1) + (b >>signed 1)
***************************************************************************/
class average_signed { public: template<typename T> static T func(T x, T y)
{
     return (x & y & 1) + (x >> 1) + (y >> 1);
}};


class saturate_modulo { public: template<typename T> static T func(T x)
{
    return x & util::makemask<u64>(4 * sizeof(T));
}};

class saturate_unsigned { public: template<typename T> static T func(T x)
{
    // get maximum for narrow type (4 versus 8)
    const T uintmax = ( T(1) << (4 * sizeof(T)) ) - T(1);
    LOG_DEBUG(LOG_EMULATOR) << "tsize=" << (4*sizeof(T)) << " tmaxu=" << uintmax;
    return std::min(x, uintmax);
}};

class saturate_signed { public: template<typename T> static T func(T x)
{
    // get minimax for narrow type (4 versus 8)
    const T intmax = ( T(1) << (4 * sizeof(T) - 1) ) - T(1);
    const T intmin = ~intmax;
    LOG_DEBUG(LOG_EMULATOR) << "tsize=" << (4*sizeof(T)) << " tmin=" << intmin << " tmax=" << intmax;
    return std::min(intmax, std::max(x, intmin));
}};

class signed_saturate_unsigned { public: template<typename T> static T func(T x)
{
    // get maximum for narrow type (4 versus 8)
    const T uintmax = ( T(1) << (4 * sizeof(T)) ) - T(1);
    LOG_DEBUG(LOG_EMULATOR) << "tsize=" << (4*sizeof(T)) << " tmaxu=" << uintmax;
    return std::min(uintmax, std::max(x, T(0)));
}};

class bit_gather {
public:
template<typename T> static T func(T value, T mask)
{
    T imm = 0;
    for (T sum = 1; mask; sum += sum)
    {
         if (value & mask & (T(0) - mask)) // unary minus unsigned error
             imm |= sum;
         mask &= mask - 1;
    }
    return imm;
}};


class bit_scatter {
public:
template<typename T> static T func(T value, T mask)
{
    T imm = 0;
    for (T sum = 1; mask; sum += sum)
    {
        if (value & sum)
            imm |= mask & (T(0) - mask); // unary minus unsigned error
        mask &= mask - 1;
    }
    return imm;
}};

// count trailing zeroes (least significant)
// popc(x^(~(-x)));
class trailing_zeros {
public:
template<typename T> static T func(T uval, unsigned shamt)
{
    if (uval == 0)
    {
        return sizeof(T) * bits_per_byte;
    }

    T sum = 0;

    for (unsigned i = 0; i <= shamt; ++i)
    {
        if (((uval >> i) & 1) != 0)
          break;
        sum++;
    }
    return sum;
}};

// count leading zeroes (most significant)
// popc((x-1) & ~x);
class leading_zeros {
public:
template<typename T> static T func(T uval, unsigned shamt)
{
    if (uval == 0)
    {
        return sizeof(T) * bits_per_byte;
    }

    T sum = 0;

    for (int i = shamt; i >= 0; --i)
    {
        if (((uval >> i) & 1) != 0)
             break;
        sum++;
    }

    return sum;
}};

// count population
class population {
public:
template<typename T> static T func(T uval, unsigned shamt)
{
    const u64 mask = util::makemask<u64>(shamt+1);
    const u64 value = uval & mask;

    return util::population::func(value);
}};

// 64-bit long bit permutation (for fft-like algorithms, endian-swap)
class permute {
public:
template<typename T> static T func(T value, unsigned shamt)
{
    return util::permute::func(value, shamt);
}};

template<typename T> inline T sll(T val, unsigned int shamt) { return val << shamt; }
template<typename T> inline T srl(T val, unsigned int shamt) { return val >> shamt; }
template<typename T> inline T sra(T val, unsigned int shamt) { return val >> shamt; }
template<typename T> inline T divp2(T val, unsigned int shamt) { return val / (static_cast<T>(1) << shamt); }

template<typename T> inline T rol(T val, unsigned int shamt) { return (val << shamt) | (val >> (8*sizeof(T) - shamt)); }
template<typename T> inline T ror(T val, unsigned int shamt) { return (val >> shamt) | (val << (8*sizeof(T) - shamt)); }

inline u64 RoundToBundle(u64 address)
{
    return address & ~util::makemask<u64>(log_bytes_per_bundle);
}

} // namespace postrisc
