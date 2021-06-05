#pragma once

#include "GeneralRegister.hpp"

namespace postrisc {
namespace aes {

GeneralRegister aesimc(GeneralRegister op);

GeneralRegister encrypt(GeneralRegister op1, GeneralRegister op2);
GeneralRegister decrypt(GeneralRegister op1, GeneralRegister op2);

GeneralRegister encrypt_last(GeneralRegister op1, GeneralRegister op2);
GeneralRegister decrypt_last(GeneralRegister op1, GeneralRegister op2);

GeneralRegister keygen_assist(GeneralRegister op, u32 rcon32);
GeneralRegister pclmulqdq(u64 op1, u64 op2);

GeneralRegister clmul(u64 src1, u64 src2);

} // namespace aes
} // namespace postrisc
