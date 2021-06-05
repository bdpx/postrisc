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

Result Core::atomic_fence(AddressSpace& as, std::memory_order order)
{
    UNREFERENCED_PARAMETER(as);
    std::atomic_thread_fence(order);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fence_a ) { return core.atomic_fence(as, std::memory_order_acquire); }
DEFINE_INSN_IMPLEMENTATION(fence_r ) { return core.atomic_fence(as, std::memory_order_release); }
DEFINE_INSN_IMPLEMENTATION(fence_ar) { return core.atomic_fence(as, std::memory_order_acq_rel); }
DEFINE_INSN_IMPLEMENTATION(fence_sc) { return core.atomic_fence(as, std::memory_order_seq_cst); }

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
Result Core::atomic_load(AddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const VirtAddress address(rb().base());
    LOG_DEBUG(LOG_STORE)
        << "read atomic " << sizeof(T) << " bytes, " << address;

    AtomicLoad<T> op(order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    ra() = op.GetTarget();

    return FinalizeInstruction();
}

#define DEFINE_ATOMIC_LOAD_OPS(TYPE) \
    DEFINE_INSN_IMPLEMENTATION(amx_ld_ ## TYPE) { return core.atomic_load<TYPE>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(amq_ld_ ## TYPE) { return core.atomic_load<TYPE>(as, std::memory_order_acquire); } \

DEFINE_ATOMIC_LOAD_OPS(u8)
DEFINE_ATOMIC_LOAD_OPS(u16)
DEFINE_ATOMIC_LOAD_OPS(u32)
DEFINE_ATOMIC_LOAD_OPS(u64)
DEFINE_ATOMIC_LOAD_OPS(u128)

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
Result Core::atomic_store(AddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const VirtAddress address(rb().base());
    T const value = ra().scalar<T>();

    LOG_DEBUG(LOG_STORE)
        << "write atomic " << sizeof(T) << " bytes, " << address;

    AtomicStore<T> op(value, order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

#define DEFINE_ATOMIC_STORE_OPS(TYPE) \
    DEFINE_INSN_IMPLEMENTATION(amx_st_ ## TYPE) { return core.atomic_store<TYPE>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(amr_st_ ## TYPE) { return core.atomic_store<TYPE>(as, std::memory_order_release); } \

DEFINE_ATOMIC_STORE_OPS(u8)
DEFINE_ATOMIC_STORE_OPS(u16)
DEFINE_ATOMIC_STORE_OPS(u32)
DEFINE_ATOMIC_STORE_OPS(u64)
DEFINE_ATOMIC_STORE_OPS(u128)

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
Result Core::compare_and_swap(AddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    // CAS dst, base, expected, desired

    const VirtAddress address(rb().base());
    T expected = rc().scalar<T>();
    const T desired = rd().scalar<T>();

    AtomicCompareAndSwap<T> op(expected, desired, order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    ra() = op.GetOldValue();
    return FinalizeInstruction();
}

#define DEFINE_CAS_OPS(TYPE) \
    DEFINE_INSN_IMPLEMENTATION(amx_cas_ ## TYPE) { return core.compare_and_swap<TYPE>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(amq_cas_ ## TYPE) { return core.compare_and_swap<TYPE>(as, std::memory_order_acquire); } \
    DEFINE_INSN_IMPLEMENTATION(amr_cas_ ## TYPE) { return core.compare_and_swap<TYPE>(as, std::memory_order_release); } \
    DEFINE_INSN_IMPLEMENTATION(amz_cas_ ## TYPE) { return core.compare_and_swap<TYPE>(as, std::memory_order_acq_rel); } \

DEFINE_CAS_OPS(u8)
DEFINE_CAS_OPS(u16)
DEFINE_CAS_OPS(u32)
DEFINE_CAS_OPS(u64)
DEFINE_CAS_OPS(u128)

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
Result Core::fetch_and_op_atomic(AddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const VirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    AtomicFetchAndOp<T,OP> op(value, order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    ra() = FinalConversion<T, STORE_T>(op.GetOldValue());
    return FinalizeInstruction();
}

#define DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE, STORE_TYPE) \
    DEFINE_INSN_IMPLEMENTATION(amx_ ## NAME ## _ ## TYPE) { return core.fetch_and_op_atomic<TYPE, STORE_TYPE, FUNC>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(amq_ ## NAME ## _ ## TYPE) { return core.fetch_and_op_atomic<TYPE, STORE_TYPE, FUNC>(as, std::memory_order_acquire); } \
    DEFINE_INSN_IMPLEMENTATION(amr_ ## NAME ## _ ## TYPE) { return core.fetch_and_op_atomic<TYPE, STORE_TYPE, FUNC>(as, std::memory_order_release); } \
    DEFINE_INSN_IMPLEMENTATION(amz_ ## NAME ## _ ## TYPE) { return core.fetch_and_op_atomic<TYPE, STORE_TYPE, FUNC>(as, std::memory_order_acq_rel); } \

#define DEFINE_FETCH_AND_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE ## 8,   u32) \
    DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE ## 16,  u16) \
    DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE ## 32,  u32) \
    DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE ## 64,  u64) \
    DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE ## 128, u128) \

DEFINE_FETCH_AND_OP_TYPES(swap,    fetch_exchange,    u)
DEFINE_FETCH_AND_OP_TYPES(ld_add,  fetch_add,         u)
DEFINE_FETCH_AND_OP_TYPES(ld_and,  fetch_and,         u)
DEFINE_FETCH_AND_OP_TYPES(ld_or,   fetch_or,          u)
DEFINE_FETCH_AND_OP_TYPES(ld_xor,  fetch_xor,         u)
DEFINE_FETCH_AND_OP_TYPES(ld_min,  fetch_common<min>, u)
DEFINE_FETCH_AND_OP_TYPES(ld_max,  fetch_common<max>, u)
DEFINE_FETCH_AND_OP_TYPES(ld_min,  fetch_common<min>, i) // SEXT
DEFINE_FETCH_AND_OP_TYPES(ld_max,  fetch_common<max>, i) // SEXT

/***************************************************************************
* store-op
***************************************************************************/
template<typename T, typename OP>
Result Core::store_op_atomic(AddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const VirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    AtomicFetchAndOp<T,OP> op(value, order);
    CHECK_RESULT_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

/*
#define DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(amx_ ## NAME ## _ ## TYPE) { return core.store_op_atomic<TYPE, FUNC>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(amr_ ## NAME ## _ ## TYPE) { return core.store_op_atomic<TYPE, FUNC>(as, std::memory_order_release); } \

#define DEFINE_STORE_AND_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE ##  8) \
    DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE ##  16) \
    DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE ##  32) \
    DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE ##  64) \
    DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE ##  128) \

DEFINE_STORE_AND_OP_TYPES(st_add,  fetch_add,         u)
DEFINE_STORE_AND_OP_TYPES(st_and,  fetch_and,         u)
DEFINE_STORE_AND_OP_TYPES(st_or,   fetch_or,          u)
DEFINE_STORE_AND_OP_TYPES(st_xor,  fetch_xor,         u)
DEFINE_STORE_AND_OP_TYPES(st_min,  fetch_common<min>, u)
DEFINE_STORE_AND_OP_TYPES(st_max,  fetch_common<max>, u)
DEFINE_STORE_AND_OP_TYPES(st_min,  fetch_common<min>, i) // SEXT
DEFINE_STORE_AND_OP_TYPES(st_max,  fetch_common<max>, i) // SEXT
*/

} // namespace postrisc
