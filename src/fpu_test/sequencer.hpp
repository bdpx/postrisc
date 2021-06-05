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

class TestSequencer {
public:
    unsigned  expNum, term1Num, term2Num;
    bool done;
    std::default_random_engine             generator;
    std::uniform_int_distribution<u64>  distribution;

    TestSequencer(void) : distribution( 0, ~u64(0) ) {}

    void init(u64 seed);

    u8  random8 ( void ) { return distribution(generator); } //rand(); }
    u16 random16( void ) { return distribution(generator); } //rand() | ( rand() << 5 ); }
    u32 random32( void ) { return distribution(generator); } //( u32(random16()) << 16 ) | u32(random16()); }
    u64 random64( void ) { return distribution(generator); } //( u64(random32()) << 32 ) | u64(random32()); }

    u32  u32_p1( u8 nbits, u16 index );
    u32  u32_p1( u8 nbits ) { return u32_p1( nbits, random8() ); }
    u64  u64_p1( u8 nbits, u16 index );
    u64  u64_p1( u8 nbits ) { return u64_p1( nbits, random16() ); }
    u128 u128_p1( u8 nbits, u16 index );
    u128 u128_p1( u8 nbits ) { return u128_p1( nbits, random16() ); }
    u256 u256_p1( u16 index );
    u256 u256_p1( void ) { return u256_p1( random16() ); }
    u512 u512_p1( u16 index );
    u512 u512_p1( void ) { return u512_p1( random16() ); }

    template<typename T> T make_sign(void) { return T(random8() & 1) << (8 * sizeof(T) - 1); }

    template<typename T> u64 WeightExponent(void);

    void RandomP3( i32 & z );
    void RandomP3( i64 & z );
    void RandomPInf( i32 & z );
    void RandomPInf( i64 & z );

    void NextP1( i32 & );
    void NextP2( i32 & );
    void NextP1( i64 & );
    void NextP2( i64 & );

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

    static size_t GetTotal(int level, i32&);
    static size_t GetTotal(int level, i64&);
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
