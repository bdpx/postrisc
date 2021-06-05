#include <atomic>

#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

// we expect native support for 1/2/4/8/16-bytes atomics (without embedded mutexes)
// so we may treat any memory location as target for atomic operation
static_assert(sizeof(std::atomic<  u8>) == sizeof(u8));
static_assert(sizeof(std::atomic< u16>) == sizeof(u16));
static_assert(sizeof(std::atomic< u32>) == sizeof(u32));
static_assert(sizeof(std::atomic< u64>) == sizeof(u64));
static_assert(sizeof(std::atomic<u128>) == sizeof(u128));

Result Core::GetMemoryOrdering(const MemoryOrdering cmd_mo, std::memory_order& mo) const
{
    switch (cmd_mo) {
        case MemoryOrdering::RELAXED: mo = std::memory_order_relaxed; return Result::continue_execution;
        case MemoryOrdering::ACQUIRE: mo = std::memory_order_acquire; return Result::continue_execution;
        case MemoryOrdering::RELEASE: mo = std::memory_order_release; return Result::continue_execution;
        case MemoryOrdering::ACQ_REL: mo = std::memory_order_acq_rel; return Result::continue_execution;
        case MemoryOrdering::SEQ_CST: mo = std::memory_order_seq_cst; return Result::continue_execution;
    }
    return Result::illegal_instruction;
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(fence)
{
    switch (core.cmd.memory_order) {
       case std::memory_order_acquire: break;
       case std::memory_order_release: break;
       case std::memory_order_acq_rel: break;
       case std::memory_order_seq_cst: break;
       default:
           return Result::illegal_instruction;
    }

    std::atomic_thread_fence(core.cmd.memory_order);
    return FinalizeInstruction();
}

// all atomic ops do zero extension by default
template<typename T, typename STORE_T>
static ALWAYS_INLINE STORE_T FinalConversion(T value) { return static_cast<STORE_T>(value); }


/***************************************************************************
* atomic loads
***************************************************************************/
template<typename T>
class AtomicLoad : public MemoryOperation {
public:
    AtomicLoad(std::memory_order memory_order)
        : m_target(0), m_memory_order(memory_order) {}
    ~AtomicLoad() override {}
    Result complex_operation(void *address) override {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        const std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        m_target = r->load(m_memory_order);
        return Result::continue_execution;
    }
    T GetTarget(void) const { return m_target; }

private:
    T m_target;
    std::memory_order m_memory_order;
};

template<typename T>
Result Core::atomic_load(AddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    switch (cmd.memory_order) {
       case std::memory_order_relaxed: break;
       case std::memory_order_acquire: break;
       case std::memory_order_seq_cst: break;
       default:
           return Result::illegal_instruction;
    }

    const VirtAddress address(rb().base());
    LOG_DEBUG(LOG_STORE)
        << "read atomic " << sizeof(T) << " bytes, " << address;

    AtomicLoad<T> op(cmd.memory_order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    ra() = op.GetTarget();

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(amo_ld_u8)   { return core.atomic_load<u8  >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_ld_u16)  { return core.atomic_load<u16 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_ld_u32)  { return core.atomic_load<u32 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_ld_u64)  { return core.atomic_load<u64 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_ld_u128) { return core.atomic_load<u128>(as); }

/***************************************************************************
* atomic stores
***************************************************************************/
template<typename T>
class AtomicStore : public MemoryOperation {
public:
    AtomicStore(T value, std::memory_order memory_order)
        : m_value(value), m_memory_order(memory_order) {}
    ~AtomicStore() override {}
    Result complex_operation(void *address) override {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        r->store(m_value, m_memory_order);
        return Result::continue_execution;
    }
private:
    T m_value;
    std::memory_order m_memory_order;
};

template<typename T>
Result Core::atomic_store(AddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    switch (cmd.memory_order) {
       case std::memory_order_relaxed: break;
       case std::memory_order_release: break;
       case std::memory_order_seq_cst: break;
       default:
           return Result::illegal_instruction;
    }

    const VirtAddress address(rb().base());
    T const value = ra().scalar<T>();

    LOG_DEBUG(LOG_STORE)
        << "write atomic " << sizeof(T) << " bytes, " << address;

    AtomicStore<T> op(value, cmd.memory_order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(amo_st_u8)   { return core.atomic_store<u8  >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_st_u16)  { return core.atomic_store<u16 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_st_u32)  { return core.atomic_store<u32 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_st_u64)  { return core.atomic_store<u64 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_st_u128) { return core.atomic_store<u128>(as); }

/***************************************************************************
* CAS
***************************************************************************/
template<typename T>
class AtomicCompareAndSwap : public MemoryOperation {
public:
    AtomicCompareAndSwap(T expected, T desired, std::memory_order success_order)
        : m_expected(expected)
        , m_desired(desired)
        , m_success_order(success_order)
    {}

    ~AtomicCompareAndSwap() override {}

    Result complex_operation(void *address) override {
        std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        r->compare_exchange_strong(m_expected, m_desired, m_success_order);
        return Result::continue_execution;
    }

    T GetOldValue(void) const { return m_expected; }

private:
    T                   m_expected;
    T                   m_desired;
    std::memory_order   m_success_order;
};

template<typename T>
Result Core::compare_and_swap(AddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    switch (cmd.memory_order) {
       case std::memory_order_relaxed: break;
       case std::memory_order_acquire: break;
       case std::memory_order_release: break;
       case std::memory_order_acq_rel: break;
       case std::memory_order_seq_cst: break;
       default:
           return Result::illegal_instruction;
    }

    // CAS dst, base, expected, desired

    const VirtAddress address(rb().base());
    T expected = rc().scalar<T>();
    const T desired = rd().scalar<T>();

    AtomicCompareAndSwap<T> op(expected, desired, cmd.memory_order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    ra() = op.GetOldValue();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(amo_cas_u8)   { return core.compare_and_swap<u8  >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_cas_u16)  { return core.compare_and_swap<u16 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_cas_u32)  { return core.compare_and_swap<u32 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_cas_u64)  { return core.compare_and_swap<u64 >(as); }
DEFINE_INSN_IMPLEMENTATION(amo_cas_u128) { return core.compare_and_swap<u128>(as); }

/***************************************************************************
* RMW
***************************************************************************/
class fetch_exchange {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order)
    {
        T old_value = std::atomic_exchange_explicit<T>(&atomic, value, order);
        return old_value;
    }
};

// universal RMW via CAS
template<typename OP>
class fetch_common {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order)
    {
        T oldVal, newVal;
        do {
            oldVal = atomic.load(std::memory_order_acquire);
            newVal = OP::template func<T>(oldVal, value);
        } while (!atomic.compare_exchange_strong(oldVal, newVal, order));
        return oldVal;
    }
};

class fetch_add {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<add>::fetch_and_op_atomic<T>(atomic, value, order);
        } else {
            return std::atomic_fetch_add_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_sub {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<subtract>::fetch_and_op_atomic<T>(atomic, value, order);
        } else {
            return std::atomic_fetch_sub_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_and {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_and>::fetch_and_op_atomic<T>(atomic, value, order);
        } else {
            return std::atomic_fetch_and_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_or {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_or>::fetch_and_op_atomic<T>(atomic, value, order);
        } else {
            return std::atomic_fetch_or_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_xor {
public:
    template<typename T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_xor>::fetch_and_op_atomic<T>(atomic, value, order);
        } else {
            return std::atomic_fetch_xor_explicit<T>(&atomic, value, order);
        }
    }
};

template<typename T, typename OP>
class AtomicFetchAndOp : public MemoryOperation {
public:
    AtomicFetchAndOp(T value, std::memory_order success_order)
        : m_value(value)
        , m_success_order(success_order)
    {}

    ~AtomicFetchAndOp() override {}

    Result complex_operation(void *address) override
    {
        std::atomic<T> *r = reinterpret_cast<std::atomic<T>*>(address);
        m_value = OP::fetch_and_op_atomic(*r, m_value, m_success_order);
        return Result::continue_execution;
    }

    T GetOldValue(void) const { return m_value; }

private:
    T m_value;
    std::memory_order m_success_order;
};

template<typename T, typename STORE_T, typename OP>
Result Core::fetch_and_op_atomic(AddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    switch (cmd.memory_order) {
       case std::memory_order_relaxed: break;
       case std::memory_order_acquire: break;
       case std::memory_order_release: break;
       case std::memory_order_acq_rel: break;
       case std::memory_order_seq_cst: break;
       default:
           return Result::illegal_instruction;
    }

    const VirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    AtomicFetchAndOp<T,OP> op(value, cmd.memory_order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    ra() = FinalConversion<T, STORE_T>(op.GetOldValue());
    return FinalizeInstruction();
}

#define FETCH_AND_OPS(STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION(amo_swap_ ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_exchange    >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_add_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_add         >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_sub_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_sub         >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_and_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_and         >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_or_   ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_or          >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_xor_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_xor         >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_min_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_common<min> >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_max_  ## UTYPE) { return core.fetch_and_op_atomic<UTYPE, UTYPE, fetch_common<max> >(as); } \
    /* signed min/max - the only OP which use signed func, but store zero-extended result */ \
    DEFINE_INSN_IMPLEMENTATION(amo_min_  ## STYPE) { return core.fetch_and_op_atomic<STYPE, UTYPE, fetch_common<min> >(as); } \
    DEFINE_INSN_IMPLEMENTATION(amo_max_  ## STYPE) { return core.fetch_and_op_atomic<STYPE, UTYPE, fetch_common<max> >(as); } \

FETCH_AND_OPS(i8,   u8)
FETCH_AND_OPS(i16,  u16)
FETCH_AND_OPS(i32,  u32)
FETCH_AND_OPS(i64,  u64)
FETCH_AND_OPS(i128, u128)

} // namespace postrisc
