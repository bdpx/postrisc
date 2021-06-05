#pragma once

#include "arch/instructions.hpp"
#include "fpu/float.hpp"

namespace postrisc {

class alignas(16) CGeneralRegister : public fpu::u128
{
public:
    constexpr explicit CGeneralRegister() : fpu::u128(0, 0) {}
    constexpr explicit CGeneralRegister(uint64_t low) : fpu::u128(0, low) {}
    constexpr explicit CGeneralRegister(uint64_t hi, uint64_t low) : fpu::u128(hi, low) {}

#define PACK(TYPE, A, B) static_cast<TYPE>((static_cast<TYPE>(A) << (4/*halftype*/ * sizeof(TYPE))) | static_cast<TYPE>(B))

    constexpr explicit CGeneralRegister(
        uint32_t v3, uint32_t v2, uint32_t v1, uint32_t v0)
        : CGeneralRegister(PACK(uint64_t, v3, v2),
                           PACK(uint64_t, v1, v0))
    {}

    constexpr explicit CGeneralRegister(
         uint16_t v7, uint16_t v6, uint16_t v5, uint16_t v4,
         uint16_t v3, uint16_t v2, uint16_t v1, uint16_t v0)
        : CGeneralRegister(PACK(uint32_t, v7, v6),
                           PACK(uint32_t, v5, v4),
                           PACK(uint32_t, v3, v2),
                           PACK(uint32_t, v1, v0))
    {}

    constexpr explicit CGeneralRegister(
        uint8_t v15, uint8_t v14, uint8_t v13, uint8_t v12,
        uint8_t v11, uint8_t v10, uint8_t v9, uint8_t v8,
        uint8_t v7, uint8_t v6, uint8_t v5, uint8_t v4,
        uint8_t v3, uint8_t v2, uint8_t v1, uint8_t v0)
        : CGeneralRegister(
                 PACK(uint16_t, v15, v14),
                 PACK(uint16_t, v13, v12),
                 PACK(uint16_t, v11, v10),
                 PACK(uint16_t,  v9,  v8),
                 PACK(uint16_t,  v7,  v6),
                 PACK(uint16_t,  v5,  v4),
                 PACK(uint16_t,  v3,  v2),
                 PACK(uint16_t,  v1,  v0)
         )
    {}
#undef PACK

    const uint64_t & base(void) const { return low(); }
    uint64_t & base(void) { return low(); }

    operator uint64_t (void) const { return low(); }
    operator uint32_t (void) const { return low(); }
    operator uint16_t (void) const { return low(); }
    operator uint8_t  (void) const { return low(); }

    operator int64_t (void) const { return low(); }
    operator int32_t (void) const { return low(); }
    operator int16_t (void) const { return low(); }
    operator int8_t  (void) const { return low(); }

    operator __uint128_t () const { return (__uint128_t)vector<uint64_t>(0) | (((__uint128_t)vector<uint64_t>(1)) << 64); }

    operator uint64_t & (void) { return m1; }

    bool IsAllBitClear(void) const { return (m1 | m0) == UINT64_C(0); }
    bool IsAllBitSet(void) const { return (m1 & m0) == ~UINT64_C(0); }

    template<class T> constexpr static size_t size(void) { return bytes_per_vector / sizeof(T); }
    // reinterpret as array
    template<class T> T * vector(void) { return reinterpret_cast<T*> ( this ); }
    template<class T> T & vector(size_t index) { return vector<T>()[index]; }
    // reinterpret as const array
    template<class T> const T * vector(void) const { return reinterpret_cast<const T*> ( this ); }
    template<class T> const T & vector(size_t index) const { return vector<T>()[index]; }
    // first element
    template<class T> T & scalar(void) { return vector<T>(0); }
    template<class T> const T & scalar(void) const { return vector<T>(0); }

    fpu::f32 & sfs(int index) { return vector<fpu::f32>()[index]; }
    fpu::f64 & sfd(int index) { return vector<fpu::f64>()[index]; }

    const fpu::f32 & sfs(int index) const { return vector<fpu::f32>()[index]; }
    const fpu::f64 & sfd(int index) const { return vector<fpu::f64>()[index]; }

    fpu::f32 & sfs(void) { return scalar<fpu::f32>(); }
    fpu::f64 & sfd(void) { return scalar<fpu::f64>(); }
    fpu::f128 & sfq(void) { return scalar<fpu::f128>(); };

    const fpu::f32 & sfs(void) const { return scalar<fpu::f32>(); }
    const fpu::f64 & sfd(void) const { return scalar<fpu::f64>(); }
    const fpu::f128 & sfq(void) const { return scalar<fpu::f128>(); };

    void operator = (const fpu::f32 & r)
    {
        sfs(0) = r;
        sfs(1) =
        sfs(2) =
        sfs(3) = fpu::f32(UINT32_C(0));
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const fpu::f64 & r)
    {
        sfd(0) = r;
        sfd(1) = fpu::f64(UINT64_C(0));
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const fpu::f128 & r)
    {
        sfq() = r;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const int64_t r)
    {
        vector<int64_t>(0) = r;
        vector<int64_t>(1) = 0;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const uint64_t r)
    {
        vector<uint64_t>(0) = r;
        vector<uint64_t>(1) = 0;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const int32_t r)
    {
        vector<uint64_t>(0) = r;
        vector<uint64_t>(1) = 0;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const uint32_t r)
    {
        vector<uint64_t>(0) = r;
        vector<uint64_t>(1) = 0;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const fpu::u128 & r)
    {
        m0 = r.m0;
        m1 = r.m1;
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    void operator = (const __uint128_t r)
    {
        vector<uint64_t>(0) = static_cast<uint64_t>(r);
        vector<uint64_t>(1) = static_cast<uint64_t>(r >> 64);
        LOG_TRACE(LOG_REGISTER_DATA) << " >> " << (*this);
    }

    uint64_t operator ++ (void)
    {
        return ++m1;
    }

    uint64_t operator -- (void)
    {
        return --m1;
    }

    CGeneralRegister operator & (const CGeneralRegister & r ) const {  return CGeneralRegister( m0 & r.m0, m1 & r.m1 );  }
    CGeneralRegister operator | (const CGeneralRegister & r ) const {  return CGeneralRegister( m0 | r.m0, m1 | r.m1 );  }
    CGeneralRegister operator ^ (const CGeneralRegister & r ) const {  return CGeneralRegister( m0 ^ r.m0, m1 ^ r.m1 );  }

    friend std::ostream & operator << (std::ostream & out, const CGeneralRegister & f);

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

} //namespace postrisc
