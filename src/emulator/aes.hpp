#pragma once

#include "core.hpp"

namespace postrisc {
namespace aes {

CGeneralRegister aesimc(CGeneralRegister op);

CGeneralRegister encrypt(CGeneralRegister op1, CGeneralRegister op2);
CGeneralRegister decrypt(CGeneralRegister op1, CGeneralRegister op2);

CGeneralRegister encrypt_last(CGeneralRegister op1, CGeneralRegister op2);
CGeneralRegister decrypt_last(CGeneralRegister op1, CGeneralRegister op2);

CGeneralRegister keygen_assist(CGeneralRegister op, u32 rcon32);
CGeneralRegister pclmulqdq(u64 op1, u64 op2);

CGeneralRegister clmul(u64 src1, u64 src2);

} // namespace aes
} // namespace postrisc
