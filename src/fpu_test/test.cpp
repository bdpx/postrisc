#include <iostream>
#include <iomanip>
#include <cassert>
#include <limits>
#include <cmath>

#include "util/mpreal.hpp"

#include "softfloat.hpp"

#if 0
#if defined (TEST_FLOAT) && FLT_MANT_DIG == 24
  TEST_fff_f (fma, 0x1.7ff8p+13, 0x1.000002p+0, 0x1.ffffp-24, 0x1.7ff802p+13);
  TEST_fff_f (fma, 0x1.fffp+0, 0x1.00001p+0, -0x1.fffp+0, 0x1.fffp-20);
  TEST_fff_f (fma, 0x1.9abcdep+127, 0x0.9abcdep-126, -0x1.f08948p+0, 0x1.bb421p-25);
  TEST_fff_f (fma, 0x1.9abcdep+100, 0x0.9abcdep-126, -0x1.f08948p-27, 0x1.bb421p-52);
  TEST_fff_f (fma, 0x1.fffffep+127, 0x1.001p+0, -0x1.fffffep+127, 0x1.fffffep+115);
  TEST_fff_f (fma, -0x1.fffffep+127, 0x1.fffffep+0, 0x1.fffffep+127, -0x1.fffffap+127);
  TEST_fff_f (fma, 0x1.fffffep+127, 2.0, -0x1.fffffep+127, 0x1.fffffep+127);
#endif
#if defined (TEST_DOUBLE) && DBL_MANT_DIG == 53
  TEST_fff_f (fma, 0x1.7fp+13, 0x1.0000000000001p+0, 0x1.ffep-48, 0x1.7f00000000001p+13);
  TEST_fff_f (fma, 0x1.fffp+0, 0x1.0000000000001p+0, -0x1.fffp+0, 0x1.fffp-52);
  TEST_fff_f (fma, 0x1.0000002p+0, 0x1.ffffffcp-1, 0x1p-300, 1.0);
  TEST_fff_f (fma, 0x1.0000002p+0, 0x1.ffffffcp-1, -0x1p-300, 0x1.fffffffffffffp-1);
  TEST_fff_f (fma, 0x1.deadbeef2feedp+1023, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp+1, 0x1.0989687bc9da4p-53);
  TEST_fff_f (fma, 0x1.deadbeef2feedp+900, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp-122, 0x1.0989687bc9da4p-176);
  TEST_fff_f (fma, 0x1.fffffffffffffp+1023, 0x1.001p+0, -0x1.fffffffffffffp+1023, 0x1.fffffffffffffp+1011);
  TEST_fff_f (fma, -0x1.fffffffffffffp+1023, 0x1.fffffffffffffp+0, 0x1.fffffffffffffp+1023, -0x1.ffffffffffffdp+1023);
  TEST_fff_f (fma, 0x1.fffffffffffffp+1023, 2.0, -0x1.fffffffffffffp+1023, 0x1.fffffffffffffp+1023);
  TEST_fff_f (fma, 0x1.6a09e667f3bccp-538, 0x1.6a09e667f3bccp-538, 0.0, 0.0);
  TEST_fff_f (fma, 0x1.deadbeef2feedp-495, 0x1.deadbeef2feedp-495, -0x1.bf86a5786a574p-989, 0x0.0000042625a1fp-1022);
  TEST_fff_f (fma, 0x1.deadbeef2feedp-503, 0x1.deadbeef2feedp-503, -0x1.bf86a5786a574p-1005, 0x0.0000000004262p-1022);
  TEST_fff_f (fma, 0x1p-537, 0x1p-538, 0x1p-1074, 0x0.0000000000002p-1022);
  TEST_fff_f (fma, 0x1.7fffff8p-968, 0x1p-106, 0x0.000001p-1022, 0x0.0000010000001p-1022);
  TEST_fff_f (fma, 0x1.4000004p-967, 0x1p-106, 0x0.000001p-1022, 0x0.0000010000003p-1022);
  TEST_fff_f (fma, 0x1.4p-967, -0x1p-106, -0x0.000001p-1022, -0x0.0000010000002p-1022);
  TEST_fff_f (fma, -0x1.19cab66d73e17p-959, 0x1.c7108a8c5ff51p-107, -0x0.80b0ad65d9b64p-1022, -0x0.80b0ad65d9d59p-1022);
  TEST_fff_f (fma, -0x1.d2eaed6e8e9d3p-979, -0x1.4e066c62ac9ddp-63, -0x0.9245e6b003454p-1022, -0x0.9245c09c5fb5dp-1022);
  TEST_fff_f (fma, 0x1.153d650bb9f06p-907, 0x1.2d01230d48407p-125, -0x0.b278d5acfc3cp-1022, -0x0.b22757123bbe9p-1022);
  TEST_fff_f (fma, -0x1.fffffffffffffp-711, 0x1.fffffffffffffp-275, 0x1.fffffe00007ffp-983, 0x1.7ffffe00007ffp-983);
#endif
#if defined (TEST_LDOUBLE) && LDBL_MANT_DIG == 113
#endif
#endif

typedef struct _float_info {
  char  a[64];
  char  b[64];
  char  c[64];
  char  d[64];
} float_info;


static const float_info f128_info[] =
{
  { "0x1.bb2de33e02ccbbfa6e245a7c1f71p-2584",  "-0x1.6b500daf0580d987f1bc0cadfcddp-13777", "0x1.613cd91d9fed34b33820e5ab9d8dp-16378", "-0x1.3a79fb50eb9ce887cffa0f09bd9fp-16360"},
  { "-0x1.f949b880cacb0f0c61540105321dp-5954", "-0x1.3876cec84b4140f3bd6198731b7ep-10525", "-0x0.a5dc1c6cfbc498c54fb0b504bf19p-16382", "-0x0.a5dc1c6cfbc498c54fb0b5038abbp-16382"},
  { "-0x1.0000fffffffffp-16221",               "0x1.0000001fffff8007fep-239", "0x0.ff87ffffffffffffe000003fffffp-16382", "0x0.ff87ffffffffffffdffc003bff7fp-16382"},
  { "-0x1.ac79c9376ef447f3827c9e9de008p-2228", "-0x1.5ba830022b6139e21fbe7270cad8p-6314", "0x1.e8282b6a26bb6a9daf5c8e73e9f9p-8616", "0x1.22f14a0253878a730cd1aee373adp-8541"},
  { "-0x1.c69749ec574caaa2ab8e97ddb9f3p+2652", "0x1.f34235ff9d095449c29b4831b62dp+3311", "0x1.fbe4302df23354dbd0c4d3cfe606p+5879", "-0x1.bb473bfdfb7a6e18886ce6e57eafp+5964"},
  { "-0x1.ca8835fc6ecfb5398625fc891be5p-1686", "0x1.621e1972bbe2180e5be9dd7d8df5p-7671", "-0x1.7d2d21b73b52cf20dec2a83902a4p-9395", "-0x1.3d2322191c9c88bc68a62ab8042cp-9356"},
  { "-0x1.55cff679ec49c2541fab41fc843ep-11819", "0x1.e60e9f464f9e8df0509647c7c971p+12325", "0x1.eaa2a7649d765c2f564f7a5beca7p+454", "-0x1.447e29fa7e406a285f4e350fcf86p+507"},
  { "0x1.f0e7b1454908576f2537d863cf9bp+11432", "0x1.cdce52f09d4ca76e68706f34b5d5p-1417", "-0x1.2e986187c70f146235ea2066e486p+9979", "0x1.c030dad3cc5643f3dd0f5619f661p+10016"},
  { "0x1.f102f7da4a57a3a4aab620e29452p-3098", "-0x1.cc06a4ff40248f9e2dcc4b6afd84p-11727", "0x1.d512a11126b5ac8ed8973b8580c8p-14849", "-0x1.be8f1cf737ab4d1c31c54f5ec23bp-14824"},
  { "-0x1.fc47ac7434b993cd8dcb2b431f25p-3816", "0x1.fbc9750da8468852d84558e1db6dp-5773", "-0x1.00a98abf783f75c40fe5b7a37d86p-9607", "-0x1.f81917b166f45e763cfcc057e2adp-9588"},
  { "0x1.00000000000007ffffffffffffffp-9045", "-0x1.ffffffffffff80000001ffffffffp+4773", "-0x1.f8p-4316", "-0x1.00000000000f88000000fffffdffp-4271"},
  { "0x1.4e922764c90701d4a2f21d01893dp-8683", "-0x1.955a12e2d7c9447c27fa022fc865p+212", "-0x1.e9634462eaef96528b90b6944578p-8521", "-0x1.08e1783184a371943d3598e10865p-8470"},
  { "0x1.801181509c03bdbef10d6165588cp-15131", "0x1.ad86f8e57d3d40bfa8007780af63p-368", "-0x1.6e9df0dab1c9f1d7a6043c390741p-15507", "0x1.417c9b2b15e2ad57dc9e0e920844p-15498"},
};

static const float_info f80_info[] = 
{
  { "0x8.07fffffffffffffp+7418L", "0xcp-11363L", "-0xf.ff80f8p-4070L", "0xc.0bffffffffffffep-3942L"},
  { "-0x8.03fcp+3696L", "0xf.fffffffffffffffp-6140L", "0x8.3ffffffffffffffp-2450L", "-0x8.01ecp-2440L"},
  { " 0x9.fcp+2033L", "-0x8.000e1f000ff800fp-3613L", "-0xf.fffffffffffc0ffp-1579L", "-0xd.fc119fb093ed092p-1577L"},
  { " 0xc.7fc000003ffffffp-1194L", "0x8.1e0003fffffffffp+15327L", "-0x8.fffep+14072L", "0xc.ae9f164020effffp+14136L"},
  { "-0x8.0001fc000000003p+1798L", "0xcp-2230L", "0x8.f7e000000000007p-468L", "-0xc.0002f9ffee10404p-429L"},
  { " 0xc.0000000000007ffp+10130L", "-0x8.000000000000001p+4430L", "0xc.07000000001ffffp+14513L", "-0xb.fffffffffffd7e4p+14563L"},
  { " 0xb.ffffp-4777L", "0x8.000000fffffffffp-11612L", "-0x0.3800fff8p-16385L", "0x5.c7fe80c7ffeffffp-16385L"},
};


template<typename T> T carry_add(T a[], T b[], T result[])
{
    T carry = 0;
    for (int i = 4-1; i >= 0; i--) {
        result[i] = a[i] + b[i];
        if (carry) {
          result[i]++;
          carry = (a[i] >= result[i]);
        } else {
          carry = (a[i] > result[i]);
        }
//        result[i] = a[i] + b[i] + carry;
//        carry = carry ? (a[i] >= result[i]) : (a[i] > result[i]);
    }
    return carry;
}


using namespace postrisc;

constexpr quadruple fact(unsigned int n) { return n <= 1 ? 1.0 : fact(n-1) * n; }
constexpr quadruple inv_fact(unsigned int n) { return 1.0 / fact(n); }

int main(void)
{
    SoftFPU  fpu;

//    a.hard() = -0x1.c139d7b84f1a3079263afcc5bae3p-51L;
//    b.hard() = -0x1.600e7a2a164840edbe2e7d301a72p+32L;
//    c.hard() =  0x1.26558cac315807eb07e448042101p-38L;
//    d.hard() =  0x1.34e48a78aae96c76ed36077dd387p-18L;

    f128 a( strtoflt128("-0x1.c139d7b84f1a3079263afcc5bae3p-51", NULL) );
    f128 b( strtoflt128("-0x1.600e7a2a164840edbe2e7d301a72p+32", NULL) );
    f128 c( strtoflt128("0x1.26558cac315807eb07e448042101p-38", NULL) );
    f128 d( strtoflt128("0x1.34e48a78aae96c76ed36077dd387p-18", NULL) );

    writeHex(std::cout, a);
    std::cout << ' ' << a.hard() << '\n';
    writeHex(std::cout, b);
    std::cout << ' ' << b.hard() << '\n';
    writeHex(std::cout, c);
    std::cout << ' ' << c.hard() << '\n' << '\n';

    f128 e = fpu.muladd(a,b,c,FLOAT_MULADD_NEGATE_NONE);
    f128 f( fmaq(a.hard(), b.hard(), c.hard() ) );
    std::cout << fpu.get_flags() << '\n';

    std::cout << std::setprecision(24) << std::fixed << std::scientific;
    writeHex(std::cout, e);
    std::cout << ' ' << e.hard() << '\n';

    writeHex(std::cout, d);
    std::cout << ' ' << d.hard() << '\n';

    writeHex(std::cout, f);
    std::cout << ' ' << f.hard() << '\n';

    for (int i=0; i<sizeof(f80_info) / sizeof(f80_info[0]); i++) {
        f80   a( strtold( f80_info[i].a, NULL ) ),
        b( strtold( f80_info[i].b, NULL ) ),
        c( strtold( f80_info[i].c, NULL ) ),
        d( strtold( f80_info[i].d, NULL ) );
        f80 e = fpu.muladd( a, b, c, FLOAT_MULADD_NEGATE_NONE );
        f80 f( __builtin_fmal(a.hard(), b.hard(), c.hard() ) );
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, e);
        std::cout << ' ' << e.hard() << '\n';
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, f);
        std::cout << ' ' << f.hard() << '\n';
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, d);
        std::cout << ' ' << d.hard() << '\n';
    }

    for (int i=0; i<sizeof(f128_info) / sizeof(f128_info[0]); i++) {
        f128
        a( strtoflt128( f128_info[i].a, NULL ) ),
        b( strtoflt128( f128_info[i].b, NULL ) ),
        c( strtoflt128( f128_info[i].c, NULL ) ),
        d( strtoflt128( f128_info[i].d, NULL ) );
        f128 e = fpu.muladd( a, b, c, FLOAT_MULADD_NEGATE_NONE );
        f128 f( fmaq(a.hard(), b.hard(), c.hard() ) );
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, e);
        std::cout << ' ' << e.hard() << '\n';
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, f);
        std::cout << ' ' << f.hard() << '\n';
        std::cout << (e == f ? "OK ": "NO ");
        writeHex(std::cout, d);
        std::cout << ' ' << d.hard() << '\n';
    }

#if 0
    std::cout << "fact:"<< '\n';
    fpu.clear_flags();
    f128 one(SIGN_POSITIVE, 0);
    f128 x = one, prod = one;
    for (int i=0; i<32; i++) {
        f128 inv = fpu.div(one, prod);
        writeHex(std::cout, inv);
        std::cout << ' ' << fpu.get_flags() << ' ';
        fpu.clear_flags();
        prod = fpu.mul(prod, x);
        x = fpu.add(x, one);
        f128 tt(inv_fact(i));
        writeHex(std::cout, tt);
        std::cout << ' ' << fpu.get_flags() << '\n';
        fpu.clear_flags();
    }
#endif
/*
0x1.34e48a78aae96c76ed36077dd388p-18L,
0x1.34e48a78aae96c76ed36077dd387p-18L,
0x1.34e48a78aae96c76ed36077dd387p-18L,
ALL_STD_EXCEPT, FE_INEXACT);

testrnd(fmal, 0x1.98210ac83fe2a8f6p53L, 0x1.9556ac1475f0f28ap-24L,
0x1.d87da3aafc60d83p70L, 0x1.d87da3aafda3f36ap70L,
0x1.d87da3aafda3f36ap70L, 0x1.d87da3aafda3f368p70L,
0x1.d87da3aafda3f368p70L, ALL_STD_EXCEPT, FE_INEXACT);

*/
  {
    f256 a( U64C(0x3ffde53628719485), U64C(0x5736453628719485), U64C(0x5736453628719485), U64C(0x5736453628719485) );
    f256 b( U64C(0x3ffDEF3464587482), U64C(0xDFEABCEDDF432156), U64C(0xabcdef4567890123), U64C(0x9876543210abcdef) );
    writeHex(std::cout, a);
    std::cout << '\n';
    writeHex(std::cout, b);
    std::cout << '\n';
    writeHex(std::cout, fpu.add(a, b));
    std::cout << '\n';
    writeHex(std::cout, fpu.sub(a, b));
    std::cout << '\n';
    writeHex(std::cout, fpu.mul(a, b));
    std::cout << '\n';

    f128 ax( strtoflt128("1.2345263578539876543215678922311879", NULL) );
    f128 bx( strtoflt128("3.2345123123765432165431344590226417", NULL) );
    writeHex(std::cout, ax);
    std::cout << '\n';
    writeHex(std::cout, bx);
    std::cout << '\n';
    f128  cx( 1.2345 );
    f256  cxx = fpu.mul( fpu.to_f256(ax), fpu.to_f256(bx) );
    writeHex(std::cout, cxx);
    std::cout << '\n';
    writeHex(std::cout, fpu.to_f128(cxx) );
    std::cout << '\n';
    std::cout << fpu.to_f128(cxx).hard() << '\n';
    std::cout << ( ax.hard() * bx.hard() ) << '\n';

    cxx = fpu.div( fpu.to_f256(ax), fpu.to_f256(bx) );
    writeHex(std::cout, cxx);
    std::cout << '\n';
    writeHex(std::cout, fpu.to_f128(cxx) );
    std::cout << '\n';
    std::cout << fpu.to_f128(cxx).hard() << '\n';
    cx = ax.hard() / bx.hard();
    writeHex(std::cout, cx);
    std::cout << '\n';
    std::cout << cx.hard() << '\n';

    {
      f256 a( U64C(0x400031234567890a), U64C(0x436536365467890a), U64C(0x987865765567890a), U64C(0x57434adce567890a) );
      f256 b( U64C(0x3ffa12345678abcd), U64C(0xdefacb6543288709), U64C(0x40876543214567ad), U64C(0x123456789adcefaa) );

      writeHex(std::cout, a); std::cout << "a\n";
      writeHex(std::cout, b); std::cout << "b\n";
      f256 c = fpu.div(a,b);
      writeHex(std::cout, c); std::cout << "a/b\n";
      f256 d = fpu.mul(c,b);
      writeHex(std::cout, d); std::cout << "(a/b)*b\n";

      c = fpu.rem(a,b);
      writeHex(std::cout, c); std::cout << "rem(a,b)\n";
      d = fpu.sub(a,c);
      d = fpu.div(d,b);
      writeHex(std::cout, d); std::cout << "(rem(a,b) - a) / b\n";

      f128 ax = fpu.to_f128(a);
      f128 bx = fpu.to_f128(b);
      writeHex(std::cout, ax); std::cout << "a\n";
      writeHex(std::cout, bx); std::cout << "b\n";
      f128 e = fpu.rem(ax, bx);
      writeHex(std::cout, e); std::cout << "rem128(a,b)\n";
      e = fpu.sub(ax,e);
      e = fpu.div(e,bx);
      writeHex(std::cout, e); std::cout << "(rem128(a,b) - a) / b\n";
      e = fpu.div(ax,bx);
      writeHex(std::cout, e); std::cout << "div128(a,b)\n";

      f128 f = fpu.round_to_int( fpu.div(ax, bx) );
      f = fpu.mul(f, bx);
      f = fpu.sub(ax, f);
      writeHex(std::cout, f); std::cout << "a - int(a/b)*b\n";
    }

    {
        f80 a( 0x0000, U64C(0x0000000000000001) );
        writeHex(std::cout, a); std::cout << "\n";
        f256 ax = fpu.to_f256(a);
        writeHex(std::cout, ax); std::cout << "\n";
        f80 b = fpu.to_f80( ax );
        writeHex(std::cout, b); std::cout << "\n";
    }
//    writeHex(std::cout, m32);
//    std::cout << '\n';
//    writeHex( std::cout, fpu.to_f256(m32) );
//    std::cout << '\n';
  }
  {
    uint512_t a( U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F) );

    uint512_t b( U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F),
                 U64C(0xABCDEF123456789F), U64C(0xABCDEF123456789F) );

    uint1024_t p;
    p.mul( a, b );
    std::cout << "a=" << a << '\n';
    std::cout << "b=" << b << '\n';
    std::cout << "p=" << p << '\n';

  }
    return 0;
}
