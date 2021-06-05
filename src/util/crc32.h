#pragma once

namespace postrisc {

template<u32 polynomial> class crc32 {
private:
    static constexpr std::array<u32, 256> generate_table(void)
    {
        std::array<u32, 256> tmp = {0};
        // u32 polynomial = 0xEDB88320;
        for (u32 i = 0; i < 256; i++) {
            u32 c = i;
            for (size_t j = 0; j < 8; j++) {
                if (c & 1) {
                    c = polynomial ^ (c >> 1);
                } else {
                    c >>= 1;
                }
            }
            tmp[i] = c;
        }
        return tmp;
    }

    static const constexpr std::array<u32, 256> table = generate_table();

public:
    static u32 hash(u32 initial, const void* buf, size_t len)
    {
        return hash_intermediate(initial ^ 0xFFFFFFFF, buf, len) ^ 0xFFFFFFFF;
    }

    static u32 hash_intermediate(u32 c, const void* buf, size_t len)
    {
        const u8* u = static_cast<const u8*>(buf);
        for (size_t i = 0; i < len; ++i) {
            c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
        }
        return c;
    }
};


using crc32x = crc32<0xEDB88320>;
using crc32c = crc32<0x82F63B78>; //0x1EDC6F41

} // namespace postrisc
