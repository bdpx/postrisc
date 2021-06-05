#pragma once

#include "util/quadruple.hpp"
#include "arch/Bundle.hpp"
#include "SoftFpu.hpp"
#include "Utils.hpp"

namespace postrisc {

template<typename TYPE>
using vector_array = TYPE [bytes_per_vector / sizeof(TYPE)];

typedef struct {
    // little endian order
    u64   link_address;
    u32   preserved_frame_size : bits_per_frame_size;
    u32   out_size             : bits_per_frame_size;
    u32   eip_offset           : bits_per_eip_offset;
    u32   future;
} link_info_t;

static_assert(sizeof(link_info_t) == bytes_per_vector);

template<typename T> class complex {
public:
    T re;
    T im;
};

struct alignas(16) GeneralRegister {
    u128 m_value;

    constexpr GeneralRegister(const GeneralRegister& r) : m_value(r.m_value) {}

    constexpr explicit GeneralRegister(void)     : m_value(0) {}
    constexpr explicit GeneralRegister(i128 r)   : m_value(r) {}
    constexpr explicit GeneralRegister(u128 r)   : m_value(r) {}

    constexpr explicit GeneralRegister(int low)  : GeneralRegister((i64)low, 0) {}
    constexpr explicit GeneralRegister(i64 low)  : GeneralRegister(low, 0) {}
    constexpr explicit GeneralRegister(u64 low)  : GeneralRegister(low, 0) {}

    constexpr explicit GeneralRegister(u64 v0, u64 v1)
        : GeneralRegister(static_cast<u128>(v0) | (static_cast<u128>(v1) << 64)) {}

    constexpr explicit GeneralRegister(u32 v0, u32 v1, u32 v2, u32 v3)
        : GeneralRegister(static_cast<u64>(v0) | (static_cast<u64>(v1) << 32),
                          static_cast<u64>(v2) | (static_cast<u64>(v3) << 32)) {}

    constexpr explicit GeneralRegister(u16 v0, u16 v1, u16 v2, u16 v3, u16 v4, u16 v5, u16 v6, u16 v7)
        : GeneralRegister(static_cast<u32>(v0) | (static_cast<u32>(v1) << 16),
                          static_cast<u32>(v2) | (static_cast<u32>(v3) << 16),
                          static_cast<u32>(v4) | (static_cast<u32>(v5) << 16),
                          static_cast<u32>(v6) | (static_cast<u32>(v7) << 16)) {}

    constexpr explicit GeneralRegister(u8 v0, u8 v1, u8 v2, u8 v3, u8 v4, u8 v5, u8 v6, u8 v7, u8 v8, u8 v9, u8 v10, u8 v11, u8 v12, u8 v13, u8 v14, u8 v15)
        : GeneralRegister(static_cast<u16>(v0)  | (static_cast<u16>(v1) << 8),
                          static_cast<u16>(v2)  | (static_cast<u16>(v3) << 8),
                          static_cast<u16>(v4)  | (static_cast<u16>(v5) << 8),
                          static_cast<u16>(v6)  | (static_cast<u16>(v7) << 8),
                          static_cast<u16>(v8)  | (static_cast<u16>(v9) << 8),
                          static_cast<u16>(v10) | (static_cast<u16>(v11) << 8),
                          static_cast<u16>(v12) | (static_cast<u16>(v13) << 8),
                          static_cast<u16>(v14) | (static_cast<u16>(v15) << 8)) {}

          u64& low(void)       { return vector<u64>(0); }
    const u64& low(void) const { return vector<u64>(0); }
          u64& high(void)       { return vector<u64>(1); }
    const u64& high(void) const { return vector<u64>(1); }

    const u64& base(void) const { return low(); }
          u64& base(void)       { return low(); }

    template<typename T> constexpr static size_t size(void) { return bytes_per_vector / sizeof(T); }

    // reinterpret as array
    template<typename T> ALWAYS_INLINE constexpr       T& vector(size_t index)       { return reinterpret_cast<      T*>(this)[index]; }
    template<typename T> ALWAYS_INLINE constexpr const T& vector(size_t index) const { return reinterpret_cast<const T*>(this)[index]; }

    // first element
    template<typename T> ALWAYS_INLINE constexpr       T& scalar(void)       { return vector<T>(0); }
    template<typename T> ALWAYS_INLINE constexpr const T& scalar(void) const { return vector<T>(0); }

    void operator=(const GeneralRegister& r) {
        scalar<u128>() = r.scalar<u128>();
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f16& r) {
        scalar<u128>() = r.val();
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f32& r) {
        scalar<u128>() = r.val();
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f64& r) {
        scalar<u128>() = r.val();
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f128& r) {
        scalar<u128>() = r.val();
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator=(const u128 r) {
        scalar<u128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i128 r) {
        scalar<i128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i64 r) {
        scalar<i128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u64 r) {
        scalar<u128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i32 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i32)0xffffffff);
        static_assert((i64)I64C(0x000000007fffffff) == (i64)(i32)0x7fffffff);
        scalar<i128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u32 r) {
        static_assert((u64)U64C(0xffffffff) == (u64)(u32)0xffffffff);
        static_assert((u64)U64C(0x7fffffff) == (u64)(u32)0x7fffffff);
        scalar<u128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i16 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i16)0xffff);
        static_assert((i64)I64C(0x0000000000007fff) == (i64)(i16)0x7fff);
        scalar<i128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u16 r) {
        static_assert((u64)U64C(0xffff) == (u64)(u16)0xffff);
        static_assert((u64)U64C(0x7fff) == (u64)(u16)0x7fff);
        scalar<u128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i8 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i8)0xff);
        static_assert((i64)I64C(0x000000000000007f) == (i64)(i8)0x7f);
        scalar<i128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u8 r) {
        static_assert((u64)U64C(0xff) == (u64)(u8)0xff);
        static_assert((u64)U64C(0x7f) == (u64)(u8)0x7f);
        scalar<u128>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const link_info_t& r) {
        scalar<link_info_t>() = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    GeneralRegister operator & (const GeneralRegister& r) const { return GeneralRegister( scalar<u128>() & r.scalar<u128>() ); }
    GeneralRegister operator | (const GeneralRegister& r) const { return GeneralRegister( scalar<u128>() | r.scalar<u128>() ); }
    GeneralRegister operator ^ (const GeneralRegister& r) const { return GeneralRegister( scalar<u128>() ^ r.scalar<u128>() ); }

    GeneralRegister operator ~ (void) const { return GeneralRegister(~scalar<u128>()); }

    void operator &= (const GeneralRegister& r) { scalar<u128>() &= r.scalar<u128>(); }
    void operator |= (const GeneralRegister& r) { scalar<u128>() |= r.scalar<u128>(); }
    void operator ^= (const GeneralRegister& r) { scalar<u128>() ^= r.scalar<u128>(); }

    bool operator == (const GeneralRegister& r) const { return scalar<u128>() == r.scalar<u128>(); }
    bool operator != (const GeneralRegister& r) const { return scalar<u128>() != r.scalar<u128>(); }

    friend std::ostream& operator<<(std::ostream& out, const GeneralRegister& f)
    {
        return out << fmt::hex(f.high())
                   << '_'
                   << fmt::hex(f.low());
    }

    unsigned get_bit(unsigned pos) const
    {
        return (scalar<u128>() >> pos) & 1;
    }

    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(m_value);
    }

};

static_assert(sizeof(GeneralRegister) == bytes_per_vector);

} //namespace postrisc
