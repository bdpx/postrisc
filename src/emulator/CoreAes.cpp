#include "Core.hpp"
#include "util/crc32.h"

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "aes.hpp"

namespace postrisc {

namespace aes {

//
// XMM - Byte Representation of a 128-bit AES State
//
//      F E D C B A
//      1 1 1 1 1 1
//      5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//     --+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--
//      P O N M L K J I H G F E D C B A
//
//
// XMM - Matrix Representation of a 128-bit AES State
//
// | A E I M |   | S(0,0) S(0,1) S(0,2) S(0,3) |   | S(0) S(4) S(8) S(C) |
// | B F J N | = | S(1,0) S(1,1) S(1,2) S(1,3) | = | S(1) S(5) S(9) S(D) |
// | C G K O |   | S(2,0) S(2,1) S(2,2) S(2,3) |   | S(2) S(6) S(A) S(E) |
// | D H L P |   | S(3,0) S(3,1) S(2,3) S(3,3) |   | S(3) S(7) S(B) S(F) |
//

//
// AES ShiftRows transformation
//
// | A E I M |    | A E I M |
// | B F J N | => | F J N B | 
// | C G K O |    | K O C G |
// | D H L P |    | P D H L |
//

static inline void ShiftRows(GeneralRegister &state)
{
    GeneralRegister tmp = state;

    state.vector<u8>(0x0) = tmp.vector<u8>(0x0); // A => A
    state.vector<u8>(0x1) = tmp.vector<u8>(0x5);
    state.vector<u8>(0x2) = tmp.vector<u8>(0xA);
    state.vector<u8>(0x3) = tmp.vector<u8>(0xF);
    state.vector<u8>(0x4) = tmp.vector<u8>(0x4); // E => E
    state.vector<u8>(0x5) = tmp.vector<u8>(0x9);
    state.vector<u8>(0x6) = tmp.vector<u8>(0xE);
    state.vector<u8>(0x7) = tmp.vector<u8>(0x3);
    state.vector<u8>(0x8) = tmp.vector<u8>(0x8); // I => I
    state.vector<u8>(0x9) = tmp.vector<u8>(0xD);
    state.vector<u8>(0xA) = tmp.vector<u8>(0x2);
    state.vector<u8>(0xB) = tmp.vector<u8>(0x7);
    state.vector<u8>(0xC) = tmp.vector<u8>(0xC); // M => M
    state.vector<u8>(0xD) = tmp.vector<u8>(0x1);
    state.vector<u8>(0xE) = tmp.vector<u8>(0x6);
    state.vector<u8>(0xF) = tmp.vector<u8>(0xB);
}

//
// AES InverseShiftRows transformation
//
// | A E I M |    | A E I M |
// | B F J N | => | N B F J | 
// | C G K O |    | K O C G |
// | D H L P |    | H L P D |
//

static inline void InverseShiftRows(GeneralRegister &state)
{
    GeneralRegister tmp = state;

    state.vector<u8>(0x0) = tmp.vector<u8>(0x0); // A => A
    state.vector<u8>(0x1) = tmp.vector<u8>(0xD);
    state.vector<u8>(0x2) = tmp.vector<u8>(0xA);
    state.vector<u8>(0x3) = tmp.vector<u8>(0x7);
    state.vector<u8>(0x4) = tmp.vector<u8>(0x4); // E => E
    state.vector<u8>(0x5) = tmp.vector<u8>(0x1);
    state.vector<u8>(0x6) = tmp.vector<u8>(0xE);
    state.vector<u8>(0x7) = tmp.vector<u8>(0xB);
    state.vector<u8>(0x8) = tmp.vector<u8>(0x8); // I => I
    state.vector<u8>(0x9) = tmp.vector<u8>(0x5);
    state.vector<u8>(0xA) = tmp.vector<u8>(0x2);
    state.vector<u8>(0xB) = tmp.vector<u8>(0xF);
    state.vector<u8>(0xC) = tmp.vector<u8>(0xC); // M => M
    state.vector<u8>(0xD) = tmp.vector<u8>(0x9);
    state.vector<u8>(0xE) = tmp.vector<u8>(0x6);
    state.vector<u8>(0xF) = tmp.vector<u8>(0x3);
}

static const u8 sbox_transformation[256] =
{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const u8 inverse_sbox_transformation[256] =
{
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static inline void SubstituteBytes(GeneralRegister &state)
{
    for (int i=0; i<16; i++)
        state.vector<u8>(i) = sbox_transformation[state.vector<u8>(i)];
}

static inline void InverseSubstituteBytes(GeneralRegister &state)
{
    for (int i=0; i<16; i++)
        state.vector<u8>(i) = inverse_sbox_transformation[state.vector<u8>(i)];
}

/*
 * Galois Field multiplication of a by b, modulo m.
 * Just like arithmetic multiplication, except that additions and
 * subtractions are replaced by XOR.
 * The code was taken from: http://www.darkside.com.au/ice/index.html
 */

static inline unsigned gf_mul(unsigned a, unsigned b)
{
    unsigned res = 0, m = 0x11b;

    while (b) {
        if (b & 1)
            res ^= a;

        a <<= 1;
        b >>= 1;

        if (a >= 256)
            a ^= m;
    }
    return res;
}

#define AES_STATE(s,a,b) (s.vector<u8>((b)*4+(a)))

static void MixColumns(GeneralRegister &state)
{
    GeneralRegister tmp = state;

    for(int j=0; j<4; j++) {
        AES_STATE(state, 0, j) = gf_mul(0x2, AES_STATE(tmp, 0, j)) ^
                                 gf_mul(0x3, AES_STATE(tmp, 1, j)) ^
                                 AES_STATE(tmp, 2, j) ^
                                 AES_STATE(tmp, 3, j);

        AES_STATE(state, 1, j) = AES_STATE(tmp, 0, j) ^
                                 gf_mul(0x2, AES_STATE(tmp, 1, j)) ^
                                 gf_mul(0x3, AES_STATE(tmp, 2, j)) ^
                                 AES_STATE(tmp, 3, j);

        AES_STATE(state, 2, j) = AES_STATE(tmp, 0, j) ^
                                 AES_STATE(tmp, 1, j) ^
                                 gf_mul(0x2, AES_STATE(tmp, 2, j)) ^
                                 gf_mul(0x3, AES_STATE(tmp, 3, j));

        AES_STATE(state, 3, j) = gf_mul(0x3, AES_STATE(tmp, 0, j)) ^
                                 AES_STATE(tmp, 1, j) ^
                                 AES_STATE(tmp, 2, j) ^
                                 gf_mul(0x2, AES_STATE(tmp, 3, j));
    }
}

static void InverseMixColumns(GeneralRegister &state)
{
    GeneralRegister tmp = state;
  
    for(int j=0; j<4; j++) {
        AES_STATE(state, 0, j) = gf_mul(0xE, AES_STATE(tmp, 0, j)) ^
                                 gf_mul(0xB, AES_STATE(tmp, 1, j)) ^
                                 gf_mul(0xD, AES_STATE(tmp, 2, j)) ^
                                 gf_mul(0x9, AES_STATE(tmp, 3, j));

        AES_STATE(state, 1, j) = gf_mul(0x9, AES_STATE(tmp, 0, j)) ^
                                 gf_mul(0xE, AES_STATE(tmp, 1, j)) ^
                                 gf_mul(0xB, AES_STATE(tmp, 2, j)) ^
                                 gf_mul(0xD, AES_STATE(tmp, 3, j));

        AES_STATE(state, 2, j) = gf_mul(0xD, AES_STATE(tmp, 0, j)) ^
                                 gf_mul(0x9, AES_STATE(tmp, 1, j)) ^
                                 gf_mul(0xE, AES_STATE(tmp, 2, j)) ^
                                 gf_mul(0xB, AES_STATE(tmp, 3, j));

        AES_STATE(state, 3, j) = gf_mul(0xB, AES_STATE(tmp, 0, j)) ^
                                 gf_mul(0xD, AES_STATE(tmp, 1, j)) ^
                                 gf_mul(0x9, AES_STATE(tmp, 2, j)) ^
                                 gf_mul(0xE, AES_STATE(tmp, 3, j));
    }
}

static inline u32 SubWord(u32 x)
{
    u8 const b0 = sbox_transformation[(x)     & 0xff];
    u8 const b1 = sbox_transformation[(x>>8)  & 0xff];
    u8 const b2 = sbox_transformation[(x>>16) & 0xff];
    u8 const b3 = sbox_transformation[(x>>24) & 0xff];

    return b0 |
         ((u32)(b1) <<  8) | 
         ((u32)(b2) << 16) |
         ((u32)(b3) << 24);
}

static inline u32 RotWord(u32 x)
{
    return (x >> 8) | (x << 24);
}

GeneralRegister aesimc(GeneralRegister op)
{
    InverseMixColumns(op);

    return op;
}

GeneralRegister encrypt(GeneralRegister op1, GeneralRegister op2)
{
    ShiftRows(op1);
    SubstituteBytes(op1);
    MixColumns(op1);

    op1.vector<u64>(0) ^= op2.vector<u64>(0);
    op1.vector<u64>(1) ^= op2.vector<u64>(1);

    return op1;
}

GeneralRegister encrypt_last(GeneralRegister op1, GeneralRegister op2)
{
    ShiftRows(op1);
    SubstituteBytes(op1);

    op1.vector<u64>(0) ^= op2.vector<u64>(0);
    op1.vector<u64>(1) ^= op2.vector<u64>(1);

    return op1;
}

GeneralRegister decrypt(GeneralRegister op1, GeneralRegister op2)
{
    InverseShiftRows(op1);
    InverseSubstituteBytes(op1);
    InverseMixColumns(op1);

    op1.vector<u64>(0) ^= op2.vector<u64>(0);
    op1.vector<u64>(1) ^= op2.vector<u64>(1);

    return op1;
}

GeneralRegister decrypt_last(GeneralRegister op1, GeneralRegister op2)
{
    InverseShiftRows(op1);
    InverseSubstituteBytes(op1);

    op1.vector<u64>(0) ^= op2.vector<u64>(0);
    op1.vector<u64>(1) ^= op2.vector<u64>(1);

    return op1;
}

GeneralRegister keygen_assist(GeneralRegister op, u32 rcon32)
{
    GeneralRegister result;

    u32 const op1 = SubWord(op.vector<u32>(1));
    result.vector<u32>(0) = op1;
    result.vector<u32>(1) = RotWord(op1) ^ rcon32;

    u32 const op3 = SubWord(op.vector<u32>(3));
    result.vector<u32>(2) = op3;
    result.vector<u32>(3) = RotWord(op3) ^ rcon32;
    return result;
}

GeneralRegister pclmulqdq(u64 op1, u64 op2)
{
    GeneralRegister r, a;

    //
    // Initialize sources for Carry Less Multiplication [R = A CLMUL B]
    //

    // A determined by imm8[0]
    a.vector<u64>(0) = op1;
    a.vector<u64>(1) = 0;

    // B determined by imm8[4]
    u64 b = op2;

    r.vector<u64>(0) = 0;
    r.vector<u64>(1) = 0;

    for (int n = 0; b && n < 64; n++) {
        if (b & 1) {
            r.vector<u64>(0) ^= a.vector<u64>(0);
            r.vector<u64>(1) ^= a.vector<u64>(1);
        }
        a.vector<u64>(1) = (a.vector<u64>(1) << 1) | (a.vector<u64>(0) >> 63);
        a.vector<u64>(0) <<= 1;
        b >>= 1;
    }

    return r;
}

GeneralRegister clmul(u64 src1, u64 src2)
{
    u64 hi = 0;
    u64 lo = (src2 & 1) ? src1 : 0;
    src2 >>= 1;

    for (int i=1; i<64; ++i) {
       if (src2 & 1) {
          lo ^= src1 << i;
          hi ^= src1 >> (64-i);
       }
       src2 >>= 1;
    }

    return GeneralRegister(hi, lo);
}

#if 0
#define XT(x) (((x) << 1) ^ ((((x) >> 7) & 1) * 0x1b))

void aesenc(u8 *s, const u8 *rk) {
    u8 i, t, u, v[4][4];
    for (i = 0; i < 16; ++i) v[((i / 4) + 4 - (i%4) ) % 4][i % 4] = sbox_transformation[s[i]];
    for (i = 0; i < 4; ++i) {
        t = v[i][0];
        u = v[i][0] ^ v[i][1] ^ v[i][2] ^ v[i][3];
        v[i][0] ^= u ^ XT(v[i][0] ^ v[i][1]);
        v[i][1] ^= u ^ XT(v[i][1] ^ v[i][2]);
        v[i][2] ^= u ^ XT( v[i][2] ^ v[i][3]);
        v[i][3] ^= u ^ XT(v[i][3] ^ t);
    }
    for (i = 0; i < 16; ++i) s[i] = v[i / 4][i % 4] ^ rk[i];
}

void aesenclast (u8 *s, const u8 *rk) {
    u8 i, v[4][4];
    for (i = 0; i < 16; ++i) v[((i / 4) + 4 - (i%4) ) % 4][i % 4] = sbox_transformation[s[i]];
    for (i = 0; i < 16; ++i) s[i] = v[i / 4][i % 4] ^ rk[i];
}

int test()
{
    u32 rk[4]      = { 0x0f0e0d0c, 0x0b0a0908, 0x07060504, 0x03020100 };
    u32 ptext[4]   = { 0xffeeddcc, 0xbbaa9988, 0x77665544, 0x33221100 };
    u32 ctext[4]   = { 0 };
    u32 exptext[4] = { 0x5ac5b470, 0x80b7cdd8, 0x30047b6a, 0xd8e0c469 };

    printf("%08x, %08x, %08x, %08x\n", exptext[0], exptext[1], exptext[2], exptext[3]);

    GeneralRegister r = encrypt(reinterpret_cast<const GeneralRegister&>(ptext),
            reinterpret_cast<const GeneralRegister&>(rk));
    LOG_DEBUG(LOG_EMULATOR) << r << '\n';
    LOG_DEBUG(LOG_EMULATOR)
                << fmt::hex(ctext[0])
        << ", " << fmt::hex(ctext[1])
        << ", " << fmt::hex(ctext[2])
        << ", " << fmt::hex(ctext[3])
        << fmt::endl;

    aesenc((u8*)ptext, (u8*)rk);
    LOG_DEBUG(LOG_EMULATOR)
                << fmt::hex(ptext[0])
        << ", " << fmt::hex(ptext[1])
        << ", " << fmt::hex(ptext[2])
        << ", " << fmt::hex(ptext[3])
        << fmt::endl;

    return 0;
}
#endif

} // namespace aes

DEFINE_INSN_IMPLEMENTATION_NOMEM(clmul_ll)
{
    core.ra() = aes::clmul( core.rb().vector<u64>(0), core.rc().vector<u64>(0) );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(clmul_hl)
{
    core.ra() = aes::clmul( core.rb().vector<u64>(1), core.rc().vector<u64>(0) );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(clmul_hh)
{
    core.ra() = aes::clmul( core.rb().vector<u64>(1), core.rc().vector<u64>(1) );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(crc32c)
{
    size_t const len = std::min<u64>(core.rd().base(), sizeof(GeneralRegister));
    u32 const prev = core.rb().scalar<u32>();
    u32 const res = crc32c::hash_intermediate(prev, &core.rc().template vector<u8>(0), len);
    core.ra() = res;
    return core.FpuFinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_enc)      { core.ra() = aes::encrypt       ( core.rb(), core.rc() );   return core.FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_enc_last) { core.ra() = aes::encrypt_last  ( core.rb(), core.rc() );   return core.FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_dec)      { core.ra() = aes::decrypt       ( core.rb(), core.rc() );   return core.FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_dec_last) { core.ra() = aes::decrypt_last  ( core.rb(), core.rc() );   return core.FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_imc)      { core.ra() = aes::aesimc        ( core.rb() );              return core.FinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(aes_keygen_assist)
{
    unsigned const rcon = core.cmd.imm & 0xFF;
    core.ra() = aes::keygen_assist(core.rb(), rcon);
    return core.FinalizeInstruction();
}

} // namespace postrisc
