#pragma once

#include "util/quadruple.hpp"
#include "arch/instructions.hpp"
#include "fpu.hpp"
#include "utils.hpp"

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

template<class T> class complex {
public:
    T re;
    T im;
};

union alignas(16) CGeneralRegister {
    vector_array<u8>                  m_u8;
    vector_array<u16>                 m_u16;
    vector_array<u32>                 m_u32;
    vector_array<u64>                 m_u64;
    vector_array<i8>                  m_i8;
    vector_array<i16>                 m_i16;
    vector_array<i32>                 m_i32;
    vector_array<i64>                 m_i64;
    vector_array<char>                m_char;
    i128                              m_i128;
    u128                              m_u128;
    vector_array<fpu::f16>            m_f16;
    vector_array<fpu::f32>            m_f32;
    vector_array<fpu::f64>            m_f64;
    vector_array<complex<fpu::f16>>   m_complex_f16;
    vector_array<complex<fpu::f32>>   m_complex_f32;
    vector_array<complex<fpu::f64>>   m_complex_f64;
    fpu::f128                         m_f128;
    vector_array<native::f16>         m_native_f16;
    vector_array<native::f32>         m_native_f32;
    vector_array<native::f64>         m_native_f64;
    native::f128                      m_native_f128;
    link_info_t                       link_info;

    constexpr CGeneralRegister(const CGeneralRegister& r) : m_u128{r.m_u128} {}

    constexpr explicit CGeneralRegister(void)
        : m_u64{0, 0} {}
    constexpr explicit CGeneralRegister(int low)
        : m_i64{(i64)low, 0} {}
    constexpr explicit CGeneralRegister(i64 low)
        : m_i64{low, 0} {}
    constexpr explicit CGeneralRegister(u64 low)
        : m_u64{low, 0} {}
    constexpr explicit CGeneralRegister(u64 hi, u64 low)
        : m_u64{low, hi} {}
    constexpr explicit CGeneralRegister(i128 r)
        : m_i128{r} {}
    constexpr explicit CGeneralRegister(u128 r)
        : m_u128{r} {}
    constexpr explicit CGeneralRegister(u32 v3, u32 v2, u32 v1, u32 v0)
        : m_u32{ v0, v1, v2, v3 } {}
    constexpr explicit CGeneralRegister(u16 v7, u16 v6, u16 v5, u16 v4, u16 v3, u16 v2, u16 v1, u16 v0)
        : m_u16{ v0, v1, v2, v3, v4, v5, v6, v7 } {}
    constexpr explicit CGeneralRegister(
        u8 v15, u8 v14, u8 v13, u8 v12, u8 v11, u8 v10, u8 v9, u8 v8, u8 v7, u8 v6, u8 v5, u8 v4, u8 v3, u8 v2, u8 v1, u8 v0)
        : m_u8{ v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15 } {}

    u64& low(void) { return m_u64[0]; }
    const u64& low(void) const { return m_u64[0]; }
    u64& high(void) { return m_u64[1]; }
    const u64& high(void) const { return m_u64[1]; }

    const u64& base(void) const { return low(); }
    u64& base(void) { return low(); }

    template<class T> constexpr static size_t size(void) { return bytes_per_vector / sizeof(T); }

    // reinterpret as array
    template<class T> T* vector(void);
    template<class T> T& vector(size_t index);
    template<class T> T const* vector(void) const;
    template<class T> T const& vector(size_t index) const;

    // first element
    template<class T> T& scalar(void) { return vector<T>(0); }
    template<class T> T const& scalar(void) const { return vector<T>(0); }

    fpu::f16& sfh(int index) { return m_f16[index]; }
    fpu::f32& sfs(int index) { return m_f32[index]; }
    fpu::f64& sfd(int index) { return m_f64[index]; }

    const fpu::f16 sfh(int index) const { return m_f16[index]; }
    const fpu::f32 sfs(int index) const { return m_f32[index]; }
    const fpu::f64 sfd(int index) const { return m_f64[index]; }

    const fpu::f16 sfh(void) const { return m_f16[0]; }
    const fpu::f32 sfs(void) const { return m_f32[0]; }
    const fpu::f64 sfd(void) const { return m_f64[0]; }
    const fpu::f128 sfq(void) const { return m_f128; };

    void operator=(const CGeneralRegister& r) {
        m_u128 = r.m_u128;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f16& r) {
        m_f16[0] = r;
        m_u16[1] = m_u16[2] = m_u16[3] = m_u16[4] = m_u16[5] = m_u16[6] = m_u16[7] = U16C(0);
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f32& r) {
        m_f32[0] = r;
        m_u32[1] = m_u32[2] = m_u32[3] = U32C(0);
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f64& r) {
        m_f64[0] = r;
        m_u64[1] = U64C(0);
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const fpu::f128& r) {
        m_f128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator=(const u128 r) {
        m_u128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i128 r) {
        m_i128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i64 r) {
        m_i128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u64 r) {
        m_u128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i32 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i32)0xffffffff);
        static_assert((i64)I64C(0x000000007fffffff) == (i64)(i32)0x7fffffff);
        m_i128 = (i64)r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u32 r) {
        static_assert((u64)U64C(0xffffffff) == (u64)(u32)0xffffffff);
        static_assert((u64)U64C(0x7fffffff) == (u64)(u32)0x7fffffff);
        m_u128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i16 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i16)0xffff);
        static_assert((i64)I64C(0x0000000000007fff) == (i64)(i16)0x7fff);
        m_i128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u16 r) {
        static_assert((u64)U64C(0xffff) == (u64)(u16)0xffff);
        static_assert((u64)U64C(0x7fff) == (u64)(u16)0x7fff);
        m_u128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const i8 r) {
        static_assert((i64)I64C(0xffffffffffffffff) == (i64)(i8)0xff);
        static_assert((i64)I64C(0x000000000000007f) == (i64)(i8)0x7f);
        m_i128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const u8 r) {
        static_assert((u64)U64C(0xff) == (u64)(u8)0xff);
        static_assert((u64)U64C(0x7f) == (u64)(u8)0x7f);
        m_i128 = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }
    void operator=(const link_info_t r) {
        link_info = r;
        LOG_DEBUG(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    CGeneralRegister operator & (const CGeneralRegister& r) const { return CGeneralRegister( m_u64[1] & r.m_u64[1], m_u64[0] & r.m_u64[0] ); }
    CGeneralRegister operator | (const CGeneralRegister& r) const { return CGeneralRegister( m_u64[1] | r.m_u64[1], m_u64[0] | r.m_u64[0] ); }
    CGeneralRegister operator ^ (const CGeneralRegister& r) const { return CGeneralRegister( m_u64[1] ^ r.m_u64[1], m_u64[0] ^ r.m_u64[0] ); }

    CGeneralRegister operator ~ (void) const { return CGeneralRegister(~m_u64[1], ~m_u64[0]); }

    void operator &= (const CGeneralRegister& r) { m_u64[0] &= r.m_u64[0]; m_u64[1] &= r.m_u64[1]; }
    void operator |= (const CGeneralRegister& r) { m_u64[0] |= r.m_u64[0]; m_u64[1] |= r.m_u64[1]; }
    void operator ^= (const CGeneralRegister& r) { m_u64[0] ^= r.m_u64[0]; m_u64[1] ^= r.m_u64[1]; }

    bool operator == (const CGeneralRegister& r) const { return m_u64[0] == r.m_u64[0] && m_u64[1] == r.m_u64[1]; }
    bool operator != (const CGeneralRegister& r) const { return m_u64[0] != r.m_u64[0] || m_u64[1] != r.m_u64[1]; }

    friend std::ostream& operator<<(std::ostream& out, const CGeneralRegister& f);

    unsigned get_bit(unsigned pos) const
    {
        if (pos < 64) {
           return (low() >>pos) & 1;
        } else if (pos < 128) {
           return (high() >> (pos-64)) & 1;
        } else {
           return 0;
        }
    }

};

static_assert(sizeof(CGeneralRegister) == bytes_per_vector);

template<> ALWAYS_INLINE i128      & CGeneralRegister::scalar<i128> (void)       { return m_i128; }
template<> ALWAYS_INLINE i128 const& CGeneralRegister::scalar<i128> (void) const { return m_i128; }
template<> ALWAYS_INLINE u128      & CGeneralRegister::scalar<u128> (void)       { return m_u128; }
template<> ALWAYS_INLINE u128 const& CGeneralRegister::scalar<u128> (void) const { return m_u128; }

template<> ALWAYS_INLINE                u8& CGeneralRegister::vector<u8               >(size_t index) { return m_u8[index]; }
template<> ALWAYS_INLINE               u16& CGeneralRegister::vector<u16              >(size_t index) { return m_u16[index]; }
template<> ALWAYS_INLINE               u32& CGeneralRegister::vector<u32              >(size_t index) { return m_u32[index]; }
template<> ALWAYS_INLINE               u64& CGeneralRegister::vector<u64              >(size_t index) { return m_u64[index]; }
template<> ALWAYS_INLINE                i8& CGeneralRegister::vector<i8               >(size_t index) { return m_i8[index]; }
template<> ALWAYS_INLINE               i16& CGeneralRegister::vector<i16              >(size_t index) { return m_i16[index]; }
template<> ALWAYS_INLINE               i32& CGeneralRegister::vector<i32              >(size_t index) { return m_i32[index]; }
template<> ALWAYS_INLINE               i64& CGeneralRegister::vector<i64              >(size_t index) { return m_i64[index]; }
template<> ALWAYS_INLINE              char& CGeneralRegister::vector<char             >(size_t index) { return m_char[index]; }
template<> ALWAYS_INLINE          fpu::f16& CGeneralRegister::vector<fpu::f16         >(size_t index) { return m_f16[index]; }
template<> ALWAYS_INLINE          fpu::f32& CGeneralRegister::vector<fpu::f32         >(size_t index) { return m_f32[index]; }
template<> ALWAYS_INLINE          fpu::f64& CGeneralRegister::vector<fpu::f64         >(size_t index) { return m_f64[index]; }
template<> ALWAYS_INLINE         fpu::f128& CGeneralRegister::vector<fpu::f128        >(size_t      ) { return m_f128; }
template<> ALWAYS_INLINE complex<fpu::f16>& CGeneralRegister::vector<complex<fpu::f16>>(size_t index) { return m_complex_f16[index]; }
template<> ALWAYS_INLINE complex<fpu::f32>& CGeneralRegister::vector<complex<fpu::f32>>(size_t index) { return m_complex_f32[index]; }
template<> ALWAYS_INLINE complex<fpu::f64>& CGeneralRegister::vector<complex<fpu::f64>>(size_t index) { return m_complex_f64[index]; }
template<> ALWAYS_INLINE       native::f16& CGeneralRegister::vector<native::f16      >(size_t index) { return m_native_f16[index]; }
template<> ALWAYS_INLINE       native::f32& CGeneralRegister::vector<native::f32      >(size_t index) { return m_native_f32[index]; }
template<> ALWAYS_INLINE       native::f64& CGeneralRegister::vector<native::f64      >(size_t index) { return m_native_f64[index]; }
template<> ALWAYS_INLINE      native::f128& CGeneralRegister::vector<native::f128     >(size_t      ) { return m_native_f128; }

template<> ALWAYS_INLINE                u8 const& CGeneralRegister::vector<u8               >(size_t index) const { return m_u8[index]; }
template<> ALWAYS_INLINE               u16 const& CGeneralRegister::vector<u16              >(size_t index) const { return m_u16[index]; }
template<> ALWAYS_INLINE               u32 const& CGeneralRegister::vector<u32              >(size_t index) const { return m_u32[index]; }
template<> ALWAYS_INLINE               u64 const& CGeneralRegister::vector<u64              >(size_t index) const { return m_u64[index]; }
template<> ALWAYS_INLINE                i8 const& CGeneralRegister::vector<i8               >(size_t index) const { return m_i8[index]; }
template<> ALWAYS_INLINE               i16 const& CGeneralRegister::vector<i16              >(size_t index) const { return m_i16[index]; }
template<> ALWAYS_INLINE               i32 const& CGeneralRegister::vector<i32              >(size_t index) const { return m_i32[index]; }
template<> ALWAYS_INLINE               i64 const& CGeneralRegister::vector<i64              >(size_t index) const { return m_i64[index]; }
template<> ALWAYS_INLINE              char const& CGeneralRegister::vector<char             >(size_t index) const { return m_char[index]; }
template<> ALWAYS_INLINE          fpu::f16 const& CGeneralRegister::vector<fpu::f16         >(size_t index) const { return m_f16[index]; }
template<> ALWAYS_INLINE          fpu::f32 const& CGeneralRegister::vector<fpu::f32         >(size_t index) const { return m_f32[index]; }
template<> ALWAYS_INLINE          fpu::f64 const& CGeneralRegister::vector<fpu::f64         >(size_t index) const { return m_f64[index]; }
template<> ALWAYS_INLINE         fpu::f128 const& CGeneralRegister::vector<fpu::f128        >(size_t      ) const { return m_f128; }
template<> ALWAYS_INLINE complex<fpu::f16> const& CGeneralRegister::vector<complex<fpu::f16>>(size_t index) const { return m_complex_f16[index]; }
template<> ALWAYS_INLINE complex<fpu::f32> const& CGeneralRegister::vector<complex<fpu::f32>>(size_t index) const { return m_complex_f32[index]; }
template<> ALWAYS_INLINE complex<fpu::f64> const& CGeneralRegister::vector<complex<fpu::f64>>(size_t index) const { return m_complex_f64[index]; }
template<> ALWAYS_INLINE       native::f16 const& CGeneralRegister::vector<native::f16      >(size_t index) const { return m_native_f16[index]; }
template<> ALWAYS_INLINE       native::f32 const& CGeneralRegister::vector<native::f32      >(size_t index) const { return m_native_f32[index]; }
template<> ALWAYS_INLINE       native::f64 const& CGeneralRegister::vector<native::f64      >(size_t index) const { return m_native_f64[index]; }
template<> ALWAYS_INLINE      native::f128 const& CGeneralRegister::vector<native::f128     >(size_t      ) const { return m_native_f128; }

inline CGeneralRegister
deposit(CGeneralRegister hi, CGeneralRegister lo, unsigned int shamt)
{
    u64 a0, a1;

    if (shamt < 64) {
       a1 = bitwise_select::func(hi.low(), lo.low(), util::makemask(shamt));
       a0 = lo.high();
    } else if (shamt > 64) {
       a1 = hi.low();
       a0 = bitwise_select::func(hi.high(), lo.high(), util::makemask(shamt - 64));
    } else {
       assert(shamt == 64);
       a1 = hi.m_u64[0];
       a0 = lo.m_u64[1];
    }
    return CGeneralRegister(a0, a1);
}

} //namespace postrisc
