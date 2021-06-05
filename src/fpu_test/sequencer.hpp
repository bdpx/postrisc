#pragma once

#include <random>

namespace postrisc {
namespace fpu {

enum {
    i32NumP1 = 32*4,
    i32NumP2 = ( i32NumP1 * (i32NumP1 + 1) ) / 2,
    i64NumP1 = 64*4,
    i64NumP2 = ( i64NumP1 * (i64NumP1 + 1) ) / 2
};

/********************************************************************
* Test sequencer
********************************************************************/

class CSequence {
public:
    unsigned  expNum, term1Num, term2Num;
    bool done;
    std::default_random_engine             generator;
    std::uniform_int_distribution<uint64_t>  distribution;

    CSequence(void) : distribution( 0, ~uint64_t(0) ) {}

    void init(uint64_t seed);

    uint8_t  random8 ( void ) { return distribution(generator); } //rand(); }
    uint16_t random16( void ) { return distribution(generator); } //rand() | ( rand() << 5 ); }
    uint32_t random32( void ) { return distribution(generator); } //( uint32_t(random16()) << 16 ) | uint32_t(random16()); }
    uint64_t random64( void ) { return distribution(generator); } //( uint64_t(random32()) << 32 ) | uint64_t(random32()); }

    uint32_t  u32_p1( uint8_t nbits, uint16_t index );
    uint32_t  u32_p1( uint8_t nbits ) { return u32_p1( nbits, random8() ); }
    uint64_t  u64_p1( uint8_t nbits, uint16_t index );
    uint64_t  u64_p1( uint8_t nbits ) { return u64_p1( nbits, random16() ); }
    u128 u128_p1( uint8_t nbits, uint16_t index );
    u128 u128_p1( uint8_t nbits ) { return u128_p1( nbits, random16() ); }
    u256 u256_p1( uint16_t index );
    u256 u256_p1( void ) { return u256_p1( random16() ); }
    u512 u512_p1( uint16_t index );
    u512 u512_p1( void ) { return u512_p1( random16() ); }

    template<class T> T make_sign(void) { return T(random8() & 1) << (8 * sizeof(T) - 1); }

    template<class T> uint64_t WeightExponent(void);

    void RandomP3( int32_t & z );
    void RandomPInf( int32_t & z );
    void RandomP3( int64_t & z );
    void RandomPInf( int64_t & z );

    void NextP1( int32_t & );
    void NextP2( int32_t & );
    void NextP1( int64_t & );
    void NextP2( int64_t & );

    void NextP1( f32 & );
    void NextP2( f32 & );
    void NextP1( f64 & );
    void NextP2( f64 & );
    void NextP1( f80 & );
    void NextP2( f80 & );
    void NextP1( f128 & );
    void NextP2( f128 & );
    void NextP1( f256 & );
    void NextP2( f256 & );
    void NextP1( f512 & );
    void NextP2( f512 & );

    void Random( f32 & );
    void Random( f64 & );
    void Random( f80 & );
    void Random( f128 & );
    void Random( f256 & );
    void Random( f512 & );

    static size_t GetTotal(int level, int32_t&);
    static size_t GetTotal(int level, int64_t&);
    static size_t GetTotal(int level, f32 &);
    static size_t GetTotal(int level, f64 &);
    static size_t GetTotal(int level, f80 &);
    static size_t GetTotal(int level, f128 &);
    static size_t GetTotal(int level, f256 &);
    static size_t GetTotal(int level, f512 &);

    static size_t GetTotal(int level, f32 &, f32 &);
    static size_t GetTotal(int level, f64 &, f64 &);
    static size_t GetTotal(int level, f80 &, f80 &);
    static size_t GetTotal(int level, f128 &, f128 &);
    static size_t GetTotal(int level, f256 &, f256 &);
    static size_t GetTotal(int level, f512 &, f512 &);

};

} //namespace fpu
} // namespace postrisc
