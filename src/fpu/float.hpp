/*****************************************************************************
inspired by:
SoftFloat IEC/IEEE Floating-point Arithmetic Package, Release 2b. John R. Hauser.
International Computer Science Institute, Suite 600, 1947 Center Street, Berkeley, California 94704.

The original version was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page:

http://www.cs.berkeley.edu/~jhauser/arithmetic/SoftFloat.html

These details are target-specific.
 (1) whether tininess for underflow is detected before or after rounding,
 (2) what (if anything) happens when exceptions are raised,
 (3) how signaling NaNs are distinguished from quiet NaNs,
 (4) the default generated quiet NaNs,
 (5) how NaNs are propagated from function inputs to output.

Derived from TestFloat, Release 2a, a package of programs
for testing the correctness of floating-point arithmetic complying to the
IEC/IEEE Standard for Floating-Point, written by John R. Hauser.
More information is available through the Web page:
http://HTTP.CS.Berkeley.EDU/~jhauser/arithmetic/TestFloat.html
*****************************************************************************/

#pragma once

#include "util/common.hpp"
#include "bigint.hpp"

#include "u128.hpp"
#include "u192.hpp"
#include "u256.hpp"
#include "u320.hpp"
#include "u512.hpp"
#include "u576.hpp"
#include "u1024.hpp"

#include "float_base.hpp"

#include "float16.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "float80.hpp"
#include "float128.hpp"
#include "float256.hpp"
#include "float512.hpp"

namespace postrisc {
namespace fpu {

/******************************************************************************
| Internal canonical NaN format.
******************************************************************************/
class commonNaNT {
public:
    sign_type    sign;
    uint64_t     high, low;
};


/*****************************************************************************************
| Software IEC/IEEE floating-point exception flags.
******************************************************************************************/
enum float_flag : uint8_t {
    FLOAT_FLAG_NOERROR         = 0,
    FLOAT_FLAG_INVALID         = 1 << 0,
    FLOAT_FLAG_DIVBYZERO       = 1 << 1,
    FLOAT_FLAG_OVERFLOW        = 1 << 2,
    FLOAT_FLAG_UNDERFLOW       = 1 << 3,
    FLOAT_FLAG_INEXACT         = 1 << 4,
    FLOAT_FLAG_INPUT_DENORMAL  = 1 << 5,
    FLOAT_FLAG_OUTPUT_DENORMAL = 1 << 6
};

inline float_flag operator | ( float_flag a, float_flag b ) { return float_flag( uint8_t(a) | uint8_t(b) ); }
inline float_flag operator & ( float_flag a, float_flag b ) { return float_flag( uint8_t(a) & uint8_t(b) ); }
inline float_flag operator ~ ( float_flag a ) { return float_flag( ~uint8_t(a) ); }

std::ostream & operator << (std::ostream & out, float_flag flags );

/*****************************************************************************************
| Software IEC/IEEE floating-point rounding mode.
******************************************************************************************/
enum rounding_mode_t : uint8_t {
    FLOAT_ROUND_TONEAREST_EVEN,
    FLOAT_ROUND_DOWNWARD,
    FLOAT_ROUND_UPWARD,
    FLOAT_ROUND_TOWARDZERO
};

enum rounding_precision_t : uint8_t {
    FLOAT_ROUND_PRECISION_32,
    FLOAT_ROUND_PRECISION_64,
    FLOAT_ROUND_PRECISION_80
};

enum compare_mode_t : uint8_t {
    FLOAT_COMPARE_LESS        = 0x01,
    FLOAT_COMPARE_GREATER     = 0x02,
    FLOAT_COMPARE_EQUAL       = 0x04,
    FLOAT_COMPARE_UNORDERED   = 0x08,
    FLOAT_COMPARE_SIGNALED    = 0x10,
    FLOAT_COMPARE_LESS_EQUAL     = FLOAT_COMPARE_LESS | FLOAT_COMPARE_EQUAL,
    FLOAT_COMPARE_GREATER_EQUAL  = FLOAT_COMPARE_GREATER | FLOAT_COMPARE_EQUAL,
    FLOAT_COMPARE_LESS_NOT_EQUAL = FLOAT_COMPARE_LESS | FLOAT_COMPARE_GREATER,
};

/*****************************************************************************************
  Software IEC/IEEE floating-point underflow tininess-detection mode.
  Detecting tininess after rounding is preferred mode by ieee754-2008 (x86),
  but some architectures use detecting before rounding, like the 32-bit ARM
******************************************************************************************/
enum tininess_mode_t : uint8_t {
    FLOAT_TININESS_AFTER_ROUNDING,
    FLOAT_TININESS_BEFORE_ROUNDING
};

/*****************************************************************************
* Software IEC/IEEE floating-point NaN operands handling mode.
******************************************************************************/
enum nan_handling_mode_t : uint8_t {
    FLOAT_DEFAULT_NAN_MODE,        // NaN payload ignored, always return default NaN
    FLOAT_LARGER_SIGNIFICAND_NAN,  // this mode used by x87 FPU
    FLOAT_FIRST_OPERAND_NAN        // this mode used by SSE, POWER-PC, SPARC, MIPS
};

/*****************************************************************************************
| Options to indicate which negations to perform in muladd()
| Using these differs from negating an input or output before calling the muladd function
| in that this means that a NaN doesn't have its sign bit inverted before it is propagated.
******************************************************************************************/
enum muladd_negate_t : uint8_t {
    FLOAT_MULADD_NEGATE_NONE    = 0,
    FLOAT_MULADD_NEGATE_ADDEND  = 1,
    FLOAT_MULADD_NEGATE_PRODUCT = 2,
    FLOAT_MULADD_NEGATE_RESULT  = FLOAT_MULADD_NEGATE_ADDEND | FLOAT_MULADD_NEGATE_PRODUCT
};

inline muladd_negate_t operator | (muladd_negate_t a, muladd_negate_t b) { return muladd_negate_t( uint8_t(a) | uint8_t(b) ); }

class CBaseFPU {
public:
    CBaseFPU()
        : m_rounding_mode(FLOAT_ROUND_TONEAREST_EVEN)
        , m_rounding_precision(FLOAT_ROUND_PRECISION_80)
        , m_tininess_mode(FLOAT_TININESS_AFTER_ROUNDING)
        , m_exception_flags(FLOAT_FLAG_NOERROR)
        , m_nan_handling_mode(FLOAT_DEFAULT_NAN_MODE)
        , m_flush_to_zero(false)
        , m_flush_inputs_to_zero(false)
        , m_default_nan_mode(false)
    {}

    rounding_mode_t get_rounding_mode(void) const { return m_rounding_mode; }
    void set_rounding_mode( rounding_mode_t val ) { m_rounding_mode = val; }

    rounding_precision_t get_rounding_precision(void) const { return m_rounding_precision; }
    void set_rounding_precision( rounding_precision_t val ) { m_rounding_precision = val; }

    tininess_mode_t get_tininess_mode(void) const { return m_tininess_mode; }
    void set_tininess_mode( tininess_mode_t val ) { m_tininess_mode = val; }
    bool tininess_before_rounding(void) const { return get_tininess_mode() == FLOAT_TININESS_BEFORE_ROUNDING; }

    float_flag get_flags(void) const { return m_exception_flags; }
    void set_exception_flags( float_flag val ) { m_exception_flags = val; }
    void clear_flags( void ) { m_exception_flags = FLOAT_FLAG_NOERROR; }
    void clear_flags( float_flag mask ) { m_exception_flags = m_exception_flags & (~ mask); }

    bool default_nan_mode(void) const { return m_default_nan_mode; }
    void set_default_nan_mode( bool val ) { m_default_nan_mode = val; }

    bool flush_to_zero(void) const { return m_flush_to_zero; }
    void set_flush_to_zero( bool val ) { m_flush_to_zero = val; }

    bool flush_inputs_to_zero(void) const { return m_flush_inputs_to_zero; }
    void set_flush_inputs_to_zero( bool val ) { m_flush_inputs_to_zero = val; }

    nan_handling_mode_t nan_handling_mode(void) const { return m_nan_handling_mode; }

    uint64_t get_control_word(void) const;
    void set_control_word( uint64_t word );
    void change_control_word(uint64_t and_mask, uint64_t or_mask);

    friend std::ostream & operator << ( std::ostream & out, const CBaseFPU &fpu );

    template<class Archive> void serialize(Archive & ar, const unsigned int version);

protected:
/*****************************************************************************************
| Routine to raise any or all of the software IEC/IEEE floating-point exception flags.
| Raises the exceptions specified by `flags'.  Floating-point traps can be
| defined here if desired.  It is currently not possible for such a trap
| to substitute a result value.  If traps are not implemented, this routine
| should be simply `exception_flags |= flags;'.
******************************************************************************************/
    void raise(float_flag flags) { m_exception_flags = m_exception_flags | flags; }

private:
    rounding_mode_t      m_rounding_mode;
    rounding_precision_t m_rounding_precision;
    tininess_mode_t      m_tininess_mode;
    float_flag           m_exception_flags;
    nan_handling_mode_t  m_nan_handling_mode;
    bool                 m_flush_to_zero;         // should denormalised results go to zero and set the inexact flag?
    bool                 m_flush_inputs_to_zero;  // should denormalised inputs go to zero and set the input_denormal flag?
    bool                 m_default_nan_mode;      // should nans payload ignored?
};

template<class Archive>
void
CBaseFPU::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_rounding_mode);
    ar & BOOST_SERIALIZATION_NVP(m_rounding_precision);
    ar & BOOST_SERIALIZATION_NVP(m_tininess_mode);
    ar & BOOST_SERIALIZATION_NVP(m_exception_flags);
    ar & BOOST_SERIALIZATION_NVP(m_nan_handling_mode);
    ar & BOOST_SERIALIZATION_NVP(m_flush_to_zero);
    ar & BOOST_SERIALIZATION_NVP(m_flush_inputs_to_zero);
    ar & BOOST_SERIALIZATION_NVP(m_default_nan_mode);
}

class CSoftFPU : public CBaseFPU {
public:

/*****************************************************************************************
| Software IEC/IEEE half-precision.
******************************************************************************************/
    f16 to_f16( f32, bool ieee = true );
    f32 to_f32( f16, bool ieee = true );


/*****************************************************************************************
| Software IEC/IEEE single-precision.
******************************************************************************************/
    int32_t to_i32( f32 );
    uint32_t to_u32( f32 );
    int64_t to_i64( f32 );

    int16_t to_i16_round_to_zero( f32 );
    int32_t to_i32_round_to_zero( f32 );
    int64_t to_i64_round_to_zero( f32 );
    uint16_t to_u16_round_to_zero( f32 );
    uint32_t to_u32_round_to_zero( f32 );

    f32 to_f32( int32_t );
    f32 to_f32( uint32_t );
    f32 to_f32( int64_t );
    f32 to_f32( uint64_t );

    f32 round_to_int( f32 );
    f32 round_to_int( f32, rounding_mode_t );
    f32 add( f32, f32 );
    f32 sub( f32, f32 );
    f32 mul( f32, f32 );
    f32 div( f32, f32 );
    f32 rem( f32, f32 );
    f32 sqrt( f32 );
    f32 exp2( f32 );
    f32 log2( f32 );
    f32 scalbn( f32, int );
    f32 muladd( f32 a, f32 b, f32 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f32 min( f32, f32, bool ignore_nans );
    f32 max( f32, f32, bool ignore_nans );
    f32 merge( f32 sign, f32 exponent, f32 mantissa );

    bool eq_signaling( f32, f32 );
    bool le_signaling( f32, f32 );
    bool lt_signaling( f32, f32 );
    bool unordered( f32, f32 );
    bool eq_quiet( f32, f32 );
    bool le_quiet( f32, f32 );
    bool lt_quiet( f32, f32 );
    bool unordered_quiet( f32, f32 );

    float_relation compare( f32, f32 );
    float_relation compare_quiet( f32, f32 );
    compare_mode_t compare( f32, f32, compare_mode_t );

/*****************************************************************************************
| Software IEC/IEEE double-precision.
******************************************************************************************/
    int32_t to_i32( f64 );
    uint32_t to_u32( f64 );
    int64_t to_i64( f64 );
    uint64_t to_u64 ( f64 );

    f64 to_f64( int32_t );
    f64 to_f64( uint32_t );
    f64 to_f64( int64_t );
    f64 to_f64( uint64_t );

    int16_t to_i16_round_to_zero( f64 );
    int32_t to_i32_round_to_zero( f64 );
    int64_t to_i64_round_to_zero( f64 );
    uint16_t to_u16_round_to_zero( f64 );
    uint32_t to_u32_round_to_zero( f64 );
    uint64_t to_u64_round_to_zero( f64 );

    f32 to_f32( f64 );
    f64 to_f64( f32 );

    f64 round_to_int( f64 );
    f64 round_to_int( f64, rounding_mode_t );
    f64 trunc_to_int( f64 );
    f64 add( f64, f64 );
    f64 sub( f64, f64 );
    f64 mul( f64, f64 );
    f64 div( f64, f64 );
    f64 rem( f64, f64 );
    f64 sqrt( f64 );
    f64 log2( f64 );
    f64 scalbn( f64, int );
    f64 muladd( f64 a, f64 b, f64 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f64 min( f64, f64, bool ignore_nans );
    f64 max( f64, f64, bool ignore_nans );
    f64 merge( f64 sign, f64 exponent, f64 mantissa );

    bool eq_signaling( f64, f64 );
    bool le_signaling( f64, f64 );
    bool lt_signaling( f64, f64 );
    bool unordered( f64, f64 );
    bool eq_quiet( f64, f64 );
    bool le_quiet( f64, f64 );
    bool lt_quiet( f64, f64 );
    bool unordered_quiet( f64, f64 );
    float_relation compare( f64, f64 );
    float_relation compare_quiet( f64, f64 );
    compare_mode_t compare( f64, f64, compare_mode_t );

/*****************************************************************************************
| Software IEC/IEEE extended double-precision.
******************************************************************************************/
    int32_t to_i32( f80 );
    int64_t to_i64( f80 );
    int32_t to_i32_round_to_zero( f80 );
    int64_t to_i64_round_to_zero( f80 );

    f80 to_f80( int32_t );
    f80 to_f80( int64_t );

    f32 to_f32( f80 );
    f64 to_f64( f80 );

    f80 to_f80( f32 );
    f80 to_f80( f64 );

    f80 round_to_int( f80 );
    f80 round_to_int( f80, rounding_mode_t );
    f80 add( f80, f80 );
    f80 sub( f80, f80 );
    f80 mul( f80, f80 );
    f80 div( f80, f80 );
    f80 rem( f80, f80 );
    f80 sqrt( f80 );
    f80 log2( f80 );
    f80 scalbn( f80, int );
    f80 muladd( f80 a, f80 b, f80 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f80 min( f80, f80, bool ignore_nans );
    f80 max( f80, f80, bool ignore_nans );
    f80 merge( f80 sign, f80 exponent, f80 mantissa );

    bool eq_signaling( f80, f80 );
    bool le_signaling( f80, f80 );
    bool lt_signaling( f80, f80 );
    bool unordered( f80, f80 );
    bool eq_quiet( f80, f80 );
    bool le_quiet( f80, f80 );
    bool lt_quiet( f80, f80 );
    bool unordered_quiet( f80, f80 );
    float_relation compare( f80, f80 );
    float_relation compare_quiet( f80, f80 );
    compare_mode_t compare( f80, f80, compare_mode_t );

/*****************************************************************************************
| Software IEC/IEEE quadruple-precision.
******************************************************************************************/
    int32_t to_i32( f128 );
    int64_t to_i64( f128 );
    int32_t to_i32_round_to_zero( f128 );
    int64_t to_i64_round_to_zero( f128 );

    f128 to_f128( int32_t );
    f128 to_f128( int64_t );
    f128 to_f128( uint64_t );

    f32 to_f32( f128 );
    f64 to_f64( f128 );
    f80 to_f80( f128 );

    f128 to_f128( f32 );
    f128 to_f128( f64 );
    f128 to_f128( f80 );

    f128 round_to_int( f128 );
    f128 round_to_int( f128, rounding_mode_t );
    f128 add( f128, f128 );
    f128 sub( f128, f128 );
    f128 mul( f128, f128 );
    f128 div( f128, f128 );
    f128 rem( f128, f128 );
    f128 sqrt( f128 );
    f128 log2( f128 );
    f128 scalbn( f128, int );
    f128 muladd( f128 a, f128 b, f128 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f128 min( f128, f128, bool ignore_nans );
    f128 max( f128, f128, bool ignore_nans );
    f128 merge( f128 sign, f128 exponent, f128 mantissa );

    bool eq_signaling( f128, f128 );
    bool le_signaling( f128, f128 );
    bool lt_signaling( f128, f128 );
    bool unordered( f128, f128 );
    bool eq_quiet( f128, f128 );
    bool le_quiet( f128, f128 );
    bool lt_quiet( f128, f128 );
    bool unordered_quiet( f128, f128 );
    float_relation compare( f128, f128 );
    float_relation compare_quiet( f128, f128 );
    compare_mode_t compare( f128, f128, compare_mode_t );


/*****************************************************************************************
| Software IEC/IEEE octuple-precision.
******************************************************************************************/
    int32_t to_i32( f256 );
    int64_t to_i64( f256 );
    int32_t to_i32_round_to_zero(f256);
    int64_t to_i64_round_to_zero(f256);

    f256 to_f256(int32_t);
    f256 to_f256(uint32_t);
    f256 to_f256(int64_t);
    f256 to_f256(uint64_t);

    f32 to_f32( f256 );
    f64 to_f64( f256 );
    f80 to_f80( f256 );
    f128 to_f128( f256 );

    f256 to_f256( f32 );
    f256 to_f256( f64 );
    f256 to_f256( f80 );
    f256 to_f256( f128 );

    f256 round_to_int( f256 );
    f256 round_to_int( f256, rounding_mode_t );
    f256 add( f256, f256 );
    f256 sub( f256, f256 );
    f256 mul( f256, f256 );
    f256 div( f256, f256 );
    f256 rem( f256, f256 );
    f256 sqrt( f256 );
    f256 log2( f256 );
    f256 scalbn( f256, int );
    f256 muladd( f256 a, f256 b, f256 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f256 min( f256, f256, bool ignore_nans );
    f256 max( f256, f256, bool ignore_nans );
    f256 merge( f256 sign, f256 exponent, f256 mantissa );

    bool eq_signaling( f256, f256 );
    bool le_signaling( f256, f256 );
    bool lt_signaling( f256, f256 );
    bool unordered( f256, f256 );
    bool eq_quiet( f256, f256 );
    bool le_quiet( f256, f256 );
    bool lt_quiet( f256, f256 );
    bool unordered_quiet( f256, f256 );
    float_relation compare( f256, f256 );
    float_relation compare_quiet( f256, f256 );

/*****************************************************************************************
| float 512 bit
******************************************************************************************/
    int32_t to_i32( f512 );
    int64_t to_i64( f512 );
    int32_t to_i32_round_to_zero(f512);
    int64_t to_i64_round_to_zero(f512);

    f512 to_f512(int32_t);
    f512 to_f512(uint32_t);
    f512 to_f512(int64_t);
    f512 to_f512(uint64_t);

    f32 to_f32( f512 );
    f64 to_f64( f512 );
    f80 to_f80( f512 );
    f128 to_f128( f512 );
    f256 to_f256( f512 );

    f512 to_f512( f32 );
    f512 to_f512( f64 );
    f512 to_f512( f80 );
    f512 to_f512( f128 );
    f512 to_f512( f256 );

    f512 round_to_int( f512 );
    f512 round_to_int( f512, rounding_mode_t );
    f512 add( f512, f512 );
    f512 sub( f512, f512 );
    f512 mul( f512, f512 );
    f512 div( f512, f512 );
    f512 rem( f512, f512 );
    f512 sqrt( f512 );
    f512 log2( f512 );
    f512 scalbn( f512, int );
    f512 muladd( f512 a, f512 b, f512 c, muladd_negate_t mode = FLOAT_MULADD_NEGATE_NONE );
    f512 min( f512, f512, bool ignore_nans );
    f512 max( f512, f512, bool ignore_nans );
    f512 merge( f512 sign, f512 exponent, f512 mantissa );

    bool eq_signaling( f512, f512 );
    bool le_signaling( f512, f512 );
    bool lt_signaling( f512, f512 );
    bool unordered( f512, f512 );
    bool eq_quiet( f512, f512 );
    bool le_quiet( f512, f512 );
    bool lt_quiet( f512, f512 );
    bool unordered_quiet( f512, f512 );
    float_relation compare( f512, f512 );
    float_relation compare_quiet( f512, f512 );

private:
    static uint64_t estimateDiv128To64( uint64_t a0, uint64_t a1, uint64_t b );
    static uint32_t estimateSqrt32(int aExp, uint32_t a);
    static uint64_t divide_step( const u512 & bSig, u576 & rem );

    f32 RoundPackTo_f32( sign_type zSign, int32_t zExp, f32::fraction_t zSig );
    f64 RoundPackTo_f64( sign_type zSign, int32_t zExp, uint64_t zSig );
    f80 RoundPackTo_f80( rounding_precision_t roundingPrecision, sign_type zSign, int32_t zExp, uint64_t zSig0, uint64_t zSig1 );
    f128 RoundPackTo_f128( sign_type zSign, int32_t zExp, u128 zSig, uint64_t zSigExtra );
    f256 RoundPackTo_f256( sign_type zSign, int32_t zExp, u256 & zSig, uint64_t zSigExtra );
    f512 RoundPackTo_f512( sign_type zSign, int32_t zExp, u512 & zSig, uint64_t zSigExtra );

    f32 NormalizeRoundPackTo_f32( sign_type zSign, int32_t zExp, uint32_t zSig );
    f64 NormalizeRoundPackTo_f64( sign_type zSign, int32_t zExp, uint64_t zSig );
    f80 NormalizeRoundPackTo_f80( rounding_precision_t roundingPrecision, sign_type zSign, int32_t zExp, uint64_t zSig0, uint64_t zSig1 );
    f128 NormalizeRoundPackTo_f128( sign_type zSign, int32_t zExp, u128 zSig );
    f256 NormalizeRoundPackTo_f256( sign_type zSign, int32_t zExp, u256 & zSig );
    f512 NormalizeRoundPackTo_f512( sign_type zSign, int32_t zExp, u512 & zSig );

    template<class FLOAT> FLOAT propagate_nan( FLOAT a );
    template<class FLOAT> FLOAT propagate_nan( FLOAT a, FLOAT b );
    template<class FLOAT> FLOAT propagate_nan( FLOAT a, FLOAT b, FLOAT c, bool infzero );

    enum int_nan_mode_t : uint8_t {
        INT_NAN_MINONLY,  // only negative INT_MIN for both positive/negative NANS
        INT_NAN_MINMAX,   // INT_MIN for negative NANS, INT_MAX for positive
        INT_NAN_DEFAULT = INT_NAN_MINMAX, // INT_NAN_MINONLY,  // default for x86
        // defaults for types
        INT_NAN_F32 = INT_NAN_DEFAULT,
        INT_NAN_F64 = INT_NAN_DEFAULT,
        INT_NAN_F80 = INT_NAN_DEFAULT,
        INT_NAN_F128 = INT_NAN_MINMAX,  // default for GNU f128
        INT_NAN_F256 = INT_NAN_MINMAX,  // default for f256
        INT_NAN_F512 = INT_NAN_MINMAX   // default for f512
    };

    int32_t round_pack_to_i32( int_nan_mode_t bothSignedNAN, sign_type zSign, uint64_t absZ );
    int64_t round_pack_to_i64( int_nan_mode_t bothSignedNAN, sign_type zSign, uint64_t absZ0, uint64_t absZ1 );

    f32 addSigs( f32 a, f32 b, sign_type zSign );
    f32 subSigs( f32 a, f32 b, sign_type zSign );

    f64 addSigs( f64 a, f64 b, sign_type zSign );
    f64 subSigs( f64 a, f64 b, sign_type zSign );

    f80 addSigs( f80 a, f80 b, sign_type zSign );
    f80 subSigs( f80 a, f80 b, sign_type zSign );

    f128 addSigs( f128 a, f128 b, sign_type zSign);
    f128 subSigs( f128 a, f128 b, sign_type zSign);

    f256 addSigs( f256 a, f256 b, sign_type zSign);
    f256 subSigs( f256 a, f256 b, sign_type zSign);

    f512 addSigs( f512 a, f512 b, sign_type zSign);
    f512 subSigs( f512 a, f512 b, sign_type zSign);

    commonNaNT ToCommonNaN( f16 );
    commonNaNT ToCommonNaN( f32 );
    commonNaNT ToCommonNaN( f64 );
    commonNaNT ToCommonNaN( f80 );
    commonNaNT ToCommonNaN( f128 );
    commonNaNT ToCommonNaN( f256 );
    commonNaNT ToCommonNaN( f512 );

    f16 to_f16( commonNaNT );
    f32 to_f32( commonNaNT );
    f64 to_f64( commonNaNT );
    f80 to_f80( commonNaNT );
    f128 to_f128( commonNaNT );
    f256 to_f256( commonNaNT );
    f512 to_f512( commonNaNT );

    template<class FLOAT> float_relation compare_internal( FLOAT a, FLOAT b, bool is_quiet );
    template<class FLOAT> compare_mode_t compare_internal( FLOAT a, FLOAT b, compare_mode_t mode);
    template<class FLOAT> FLOAT minmax( FLOAT a, FLOAT b, bool ignore_nans, bool ismin );
};

} // namespace fpu
} // namespace postrisc
