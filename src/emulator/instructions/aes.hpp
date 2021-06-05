#pragma once

#include "core.hpp"

namespace postrisc {

namespace aes {

CGeneralRegister aesimc(CGeneralRegister op);

CGeneralRegister encrypt(CGeneralRegister op1, CGeneralRegister op2);
CGeneralRegister decrypt(CGeneralRegister op1, CGeneralRegister op2);

CGeneralRegister encrypt_last(CGeneralRegister op1, CGeneralRegister op2);
CGeneralRegister decrypt_last(CGeneralRegister op1, CGeneralRegister op2);

CGeneralRegister keygen_assist(CGeneralRegister op, uint32_t rcon32);
CGeneralRegister pclmulqdq(uint64_t op1, uint64_t op2);

u128 clmul(uint64_t src1, uint64_t src2);

} // namespace aes

DEFINE_INSN_IMPLEMENTATION(clmul)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    const unsigned scale = cmd.GetIndexedScale();
    const unsigned bnum = scale & 1;
    const unsigned cnum = (scale >> 1) & 1;
    a = aes::clmul( b.vector<uint64_t>(bnum), c.vector<uint64_t>(cnum) );
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(aesenc)     { GET_WRA; GET_RRB; GET_RRC; a = aes::encrypt       ( b, c );   return FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(aesenclast) { GET_WRA; GET_RRB; GET_RRC; a = aes::encrypt_last  ( b, c );   return FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(aesdec)     { GET_WRA; GET_RRB; GET_RRC; a = aes::decrypt       ( b, c );   return FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(aesdeclast) { GET_WRA; GET_RRB; GET_RRC; a = aes::decrypt_last  ( b, c );   return FinalizeInstruction(); }
DEFINE_INSN_IMPLEMENTATION(aesimc)     { GET_WRA; GET_RRB;          a = aes::aesimc        ( b );      return FinalizeInstruction(); }

DEFINE_INSN_IMPLEMENTATION(aeskeygen)
{
    GET_WRA;
    GET_RRB;
    int64_t const rcon = cmd.GetUintStride();
    a = aes::keygen_assist(b, rcon);
    return FinalizeInstruction();
}

} // namespace postrisc
