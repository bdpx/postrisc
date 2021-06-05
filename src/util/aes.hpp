#pragma once

#include "fmt.hpp"

namespace postrisc {
namespace aes {

class aes_block {
public:
    const u8& get(size_t index) const { return data[index]; }
    u8& get(size_t index) { return data[index]; }
    bool operator==(const aes_block& r) const {
        for (size_t i = 0; i < 16; i++) {
            if (data[0] != r.data[0]) return false;
        }
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, const aes_block& r) {
        for (size_t i = 0; i < 16; i++) {
            out << fmt::hex(r.data[i]);
        }
        return out;
    }
private:
    u8 data[16];
};

aes_block aesimc(aes_block op);

aes_block encrypt(aes_block op1, aes_block op2);
aes_block decrypt(aes_block op1, aes_block op2);

aes_block encrypt_last(aes_block op1, aes_block op2);
aes_block decrypt_last(aes_block op1, aes_block op2);

aes_block keygen_assist(aes_block op, u32 rcon32);

u128 clmul(u64 src1, u64 src2);

} // namespace aes
} // namespace postrisc
