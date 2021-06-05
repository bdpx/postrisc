#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "Result.hpp"
#include "disassembler/Decoder.hpp"

namespace postrisc {

/***************************************************************************
* typed wrapper for physical address
***************************************************************************/
class PhysAddress {
friend class boost::serialization::access;
template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)) {
    ar & BOOST_SERIALIZATION_NVP(m_address);
}
public:
    explicit PhysAddress(void) : m_address(0) {}
    explicit PhysAddress(u64 address) : m_address(address) {}

    friend std::ostream& operator<<(std::ostream& out, const PhysAddress& r) {
        return out << fmt::hex(r.m_address);
    }
    u64 get_address(void) const { return m_address; }
    PhysAddress rounddown_pow2(u64 size) const {
        return PhysAddress(util::rounddown_pow2<u64>(m_address, size));
    }

private:
    u64 m_address;
};

/***************************************************************************
* typed wrapper for virtual address
***************************************************************************/
class VirtAddress {
friend class boost::serialization::access;
template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)) {
    ar & BOOST_SERIALIZATION_NVP(m_address);
}
public:
    explicit VirtAddress(void) : m_address(0) {}
    explicit VirtAddress(u64 address) : m_address(address) {}
    friend std::ostream& operator<<(std::ostream& out, const VirtAddress& r) {
        return out << fmt::hex(r.m_address);
    }
    u64 get_address(void) const { return m_address; }
    VirtAddress rounddown_pow2(u64 size) const {
        return VirtAddress(util::rounddown_pow2<u64>(m_address, size));
    }
private:
    u64 m_address;
};

// interface for complex memory operations (not simple read/write)
class MemoryOperation {
public:
    virtual ~MemoryOperation() {};
    virtual Result complex_operation(void *address) = 0;
};

class Core;

/***************************************************************************
* interface to emulate address space
***************************************************************************/
class AddressSpace
// boost::intrusive_ref_counter<AddressSpace, boost::thread_safe_counter>
{
public:
    AddressSpace(u32 bits_per_page_offset, Decoder& decoder)
        : m_decoder(decoder)
        , m_page_bits(bits_per_page_offset)
    {}

    virtual ~AddressSpace(void) {}

    // raw physical interface
    virtual Result bus_read_u8  (const PhysAddress address, u8& value) const = 0;
    virtual Result bus_read_u16 (const PhysAddress address, u16& value) const = 0;
    virtual Result bus_read_u32 (const PhysAddress address, u32& value) const = 0;
    virtual Result bus_read_u64 (const PhysAddress address, u64& value) const = 0;
    virtual Result bus_read_u128(const PhysAddress address, u128& value) const = 0;
    virtual Result bus_read     (const PhysAddress address, size_t len, void *data) const = 0;

    virtual Result bus_write_u8  (const PhysAddress address, u8 value) = 0;
    virtual Result bus_write_u16 (const PhysAddress address, u16 value) = 0;
    virtual Result bus_write_u32 (const PhysAddress address, u32 value) = 0;
    virtual Result bus_write_u64 (const PhysAddress address, u64 value) = 0;
    virtual Result bus_write_u128(const PhysAddress address, u128 value) = 0;
    virtual Result bus_write     (const PhysAddress address, size_t len, const void *data) = 0;

    virtual Result bus_complex_operation(const PhysAddress address, size_t len, MemoryOperation& op) = 0;

    // MMU virt-mm interface.
    // AddressSpace may be implemented as whole-system or single-app:
    // for the whole-system we need call back per-core TLB,
    // so we pass core parameter to all mmu_* functions
    virtual Result mmu_fetch_code(Core& core, const VirtAddress va, size_t len, void *data) = 0;

    virtual Result mmu_read_u8  (Core& core, const VirtAddress va, u8& value) const = 0;
    virtual Result mmu_read_u16 (Core& core, const VirtAddress va, u16& value) const = 0;
    virtual Result mmu_read_u32 (Core& core, const VirtAddress va, u32& value) const = 0;
    virtual Result mmu_read_u64 (Core& core, const VirtAddress va, u64& value) const = 0;
    virtual Result mmu_read_u128(Core& core, const VirtAddress va, u128& value) const = 0;

    virtual Result mmu_write_u8  (Core& core, const VirtAddress va, u8 value) = 0;
    virtual Result mmu_write_u16 (Core& core, const VirtAddress va, u16 value) = 0;
    virtual Result mmu_write_u32 (Core& core, const VirtAddress va, u32 value) = 0;
    virtual Result mmu_write_u64 (Core& core, const VirtAddress va, u64 value) = 0;
    virtual Result mmu_write_u128(Core& core, const VirtAddress va, u128 value) = 0;

    virtual Result mmu_read_u32_finalized(Core& core, const VirtAddress va, u32& value) const = 0;
    virtual Result mmu_read_u64_finalized(Core& core, const VirtAddress va, u64& value) const = 0;

    virtual Result mmu_read_backing_store(Core& core, const VirtAddress va, size_t len, void *data) const = 0;
    virtual Result mmu_write_backing_store(Core& core, const VirtAddress va, size_t len, void const *data) = 0;
    virtual Result mmu_complex_operation(Core& core, const VirtAddress va, size_t len,
        u64 requested_rights, u64 unexpected_rights, MemoryOperation& op) = 0;
    virtual std::tuple<Result, PhysAddress> mmu_translate(Core& core, const VirtAddress va) = 0;
    // TODO: TLB/cache interfaces

    virtual int DoSyscall(unsigned UNUSED(code), u64 & UNUSED(result),
                  u64 UNUSED(r1), u64 UNUSED(r2), u64 UNUSED(r3),
                  u64 UNUSED(r4), u64 UNUSED(r5), u64 UNUSED(r6)) { return -1; }

    // helpers for handling typed overloads to virtual funcs
    Result mmu_read(Core& core, const VirtAddress va,   u8& value) const { return mmu_read_u8  (core, va, value); }
    Result mmu_read(Core& core, const VirtAddress va,  u16& value) const { return mmu_read_u16 (core, va, value); }
    Result mmu_read(Core& core, const VirtAddress va,  u32& value) const { return mmu_read_u32 (core, va, value); }
    Result mmu_read(Core& core, const VirtAddress va,  u64& value) const { return mmu_read_u64 (core, va, value); }
    Result mmu_read(Core& core, const VirtAddress va, u128& value) const { return mmu_read_u128(core, va, value); }

    Result mmu_read(Core& core, const VirtAddress va,   i8& value) const { return mmu_read_u8  (core, va, reinterpret_cast<u8  &>(value)); }
    Result mmu_read(Core& core, const VirtAddress va,  i16& value) const { return mmu_read_u16 (core, va, reinterpret_cast<u16 &>(value)); }
    Result mmu_read(Core& core, const VirtAddress va,  i32& value) const { return mmu_read_u32 (core, va, reinterpret_cast<u32 &>(value)); }
    Result mmu_read(Core& core, const VirtAddress va,  i64& value) const { return mmu_read_u64 (core, va, reinterpret_cast<u64 &>(value)); }
    Result mmu_read(Core& core, const VirtAddress va, i128& value) const { return mmu_read_u128(core, va, reinterpret_cast<u128&>(value)); }

    Result mmu_write(Core& core, const VirtAddress va,   u8 value) { return mmu_write_u8  (core, va, value); }
    Result mmu_write(Core& core, const VirtAddress va,  u16 value) { return mmu_write_u16 (core, va, value); }
    Result mmu_write(Core& core, const VirtAddress va,  u32 value) { return mmu_write_u32 (core, va, value); }
    Result mmu_write(Core& core, const VirtAddress va,  u64 value) { return mmu_write_u64 (core, va, value); }
    Result mmu_write(Core& core, const VirtAddress va, u128 value) { return mmu_write_u128(core, va, value); }

    Result mmu_write(Core& core, const VirtAddress va,   i8 value) { return mmu_write_u8  (core, va,   (u8)value); }
    Result mmu_write(Core& core, const VirtAddress va,  i16 value) { return mmu_write_u16 (core, va,  (u16)value); }
    Result mmu_write(Core& core, const VirtAddress va,  i32 value) { return mmu_write_u32 (core, va,  (u32)value); }
    Result mmu_write(Core& core, const VirtAddress va,  i64 value) { return mmu_write_u64 (core, va,  (u64)value); }
    Result mmu_write(Core& core, const VirtAddress va, i128 value) { return mmu_write_u128(core, va, (u128)value); }

    // misc
    size_t get_page_size(void) const { return U64C(1) << m_page_bits; }
    void set_bits_per_page_offset(u32 bits_per_page_offset) { m_page_bits = bits_per_page_offset; }

    Decoder& get_decoder(void) const { return m_decoder; }

    unsigned get_page_bits(void) const { return m_page_bits; }
    u64 get_page_number(u64 address) const { return address >> m_page_bits; }

private:
    Decoder&   m_decoder; // for address/names database
    unsigned   m_page_bits;
};

/***************************************************************************
* insert this into derived class
***************************************************************************/
#define STANDARD_ADDRESS_SPACE_INTERFACE                                                                 \
    Result bus_read_u8(const PhysAddress address, u8& value) const override;                             \
    Result bus_read_u16(const PhysAddress address, u16& value) const override;                           \
    Result bus_read_u32(const PhysAddress address, u32& value) const override;                           \
    Result bus_read_u64(const PhysAddress address, u64& value) const override;                           \
    Result bus_read_u128(const PhysAddress address, u128& value) const override;                         \
    Result bus_read(const PhysAddress address, size_t len, void* bytes) const override;                  \
    Result bus_write_u8(const PhysAddress address, u8 value) override;                                   \
    Result bus_write_u16(const PhysAddress address, u16 value) override;                                 \
    Result bus_write_u32(const PhysAddress address, u32 value) override;                                 \
    Result bus_write_u64(const PhysAddress address, u64 value) override;                                 \
    Result bus_write_u128(const PhysAddress address, u128 value) override;                               \
    Result bus_write(const PhysAddress address, size_t len, const void* bytes) override;                 \
    Result bus_complex_operation(const PhysAddress address, size_t len, MemoryOperation& op) override;   \

/***************************************************************************
* various consts
***************************************************************************/
enum {
    bits_per_absolute_address = 60,
};

const u64 PHYS_ADDRESS_MASK = util::makemask<u64>(bits_per_absolute_address);
const u64 INVALID_PHYS_ADDRESS = (~static_cast<u64>(0));

/***************************************************************************
* const to use with mmu interface (rights)
***************************************************************************/
enum : u64 {
    // low bits (minimal supported 12-bit page offset)
    VM_PAGE_FLAG_VALID                 = U64C(0x0000000000000001),   // translation is valid, page is reserved
    VM_PAGE_FLAG_SUPERPAGE             = U64C(0x0000000000000002),   // stop walking, super-page found
    VM_PAGE_FLAG_PRESENTED             = U64C(0x0000000000000004),   // page is committed
    VM_PAGE_FLAG_ACCESS                = U64C(0x0000000000000008),   // some access occurs
    VM_PAGE_FLAG_DIRTY                 = U64C(0x0000000000000010),   // writes occurs
    VM_PAGE_FLAG_GLOBAL                = U64C(0x0000000000000020),   // global page (not flushed on context switch)
    // rights
    VM_PAGE_RIGHT_READ                 = U64C(0x0000000000000040),   // can read data
    VM_PAGE_RIGHT_WRITE                = U64C(0x0000000000000080),   // can write data
    VM_PAGE_RIGHT_EXECUTE              = U64C(0x0000000000000100),   // can execute code
    VM_PAGE_RIGHT_BACKING_STORE        = U64C(0x0000000000000200),   // can backstore registers
    VM_PAGE_RIGHT_FINAL                = U64C(0x0000000000000400),   // finalized page (not changed protection)
    VM_PAGE_RIGHT_PROMOTE              = U64C(0x0000000000000800),   // kernel rights promotion

    // high bits (in unimplemented physical address bits)
    VM_PAGE_ATTRIBUTE_CACHEABLE        = U64C(0x1000000000000000),   // enable caching
    VM_PAGE_ATTRIBUTE_WRITE_COALESCING = U64C(0x2000000000000000),   // enable write combining
    VM_PAGE_ATTRIBUTE_WRITE_THROUGH    = U64C(0x4000000000000000),   // write mode
    VM_PAGE_ATTRIBUTE_RESIDENT         = U64C(0x8000000000000000),   // reserved

    VM_PAGE_RIGHT_MASK =
          VM_PAGE_RIGHT_READ
        | VM_PAGE_RIGHT_WRITE
        | VM_PAGE_RIGHT_EXECUTE
        | VM_PAGE_RIGHT_BACKING_STORE
        | VM_PAGE_RIGHT_FINAL
        | VM_PAGE_RIGHT_PROMOTE,

    VM_PAGE_RIGHT_NONE = U64C(0x0),

    VM_PAGE_ATTRIBUTES_MASK =
          VM_PAGE_ATTRIBUTE_CACHEABLE
        | VM_PAGE_ATTRIBUTE_WRITE_COALESCING
        | VM_PAGE_ATTRIBUTE_WRITE_THROUGH
        | VM_PAGE_ATTRIBUTE_RESIDENT,

    VM_PAGE_FLAGS_ALL =
          VM_PAGE_FLAG_VALID
        | VM_PAGE_FLAG_SUPERPAGE
        | VM_PAGE_FLAG_ACCESS
        | VM_PAGE_FLAG_DIRTY
        | VM_PAGE_FLAG_GLOBAL
        | VM_PAGE_FLAG_PRESENTED
        | VM_PAGE_RIGHT_MASK
        | VM_PAGE_ATTRIBUTES_MASK,

    VM_PAGE_PHYS_ADDRESS_MASK = PHYS_ADDRESS_MASK & ~VM_PAGE_FLAGS_ALL,  // U64C(0x0fffffffffffc000)
};

} // namespace postrisc
