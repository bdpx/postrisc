#include <atomic>

#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"

namespace postrisc {

// we expect native support for 1/2/4/8/16-bytes atomics (without embedded mutexes)
// so we may treat any memory location as target for atomic operation
static_assert(sizeof(std::atomic<  u8>) == sizeof(u8));
static_assert(sizeof(std::atomic< u16>) == sizeof(u16));
static_assert(sizeof(std::atomic< u32>) == sizeof(u32));
static_assert(sizeof(std::atomic< u64>) == sizeof(u64));
static_assert(sizeof(std::atomic<u128>) == sizeof(u128));

CStatus Core::atomic_fence(IAddressSpace& as, std::memory_order order)
{
    UNREFERENCED_PARAMETER(as);
    std::atomic_thread_fence(order);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(fencea)  { return core.atomic_fence(as, std::memory_order_acquire); }
DEFINE_INSN_IMPLEMENTATION(fencer)  { return core.atomic_fence(as, std::memory_order_release); }
DEFINE_INSN_IMPLEMENTATION(fencear) { return core.atomic_fence(as, std::memory_order_acq_rel); }
DEFINE_INSN_IMPLEMENTATION(fencesc) { return core.atomic_fence(as, std::memory_order_seq_cst); }

/***************************************************************************
* atomic loads
***************************************************************************/
template<class T>
class CAtomicLoad : public IMemoryOperation {
public:
    CAtomicLoad(std::memory_order memory_order)
        : m_target(0), m_memory_order(memory_order) {}
    ~CAtomicLoad() override {}
    CStatus complex_operation(void *address) override {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        const std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        m_target = r->load(m_memory_order);
        return CStatus::continue_execution;
    }
    T GetTarget(void) const { return m_target; }

private:
    T m_target;
    std::memory_order m_memory_order;
};

template<class T>
CStatus
Core::atomic_load(IAddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const CVirtAddress address(rb().base());
    LOG_DEBUG(LOG_STORE)
        << "read atomic " << sizeof(T) << " bytes, " << address;

    CAtomicLoad<T> op(order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    ra() = op.GetTarget();

    return FinalizeInstruction();
}

#define DEFINE_LOAD_OPS(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME     ) { return core.atomic_load<TYPE>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## a) { return core.atomic_load<TYPE>(as, std::memory_order_acquire); } \

DEFINE_LOAD_OPS(ldab, u8)
DEFINE_LOAD_OPS(ldah, u16)
DEFINE_LOAD_OPS(ldaw, u32)
DEFINE_LOAD_OPS(ldad, u64)
DEFINE_LOAD_OPS(ldaq, u128)

/***************************************************************************
* atomic stores
***************************************************************************/
template<class T>
class CAtomicStore : public IMemoryOperation {
public:
    CAtomicStore(T value, std::memory_order memory_order)
        : m_value(value), m_memory_order(memory_order) {}
    ~CAtomicStore() override {}
    CStatus complex_operation(void *address) override {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        r->store(m_value, m_memory_order);
        return CStatus::continue_execution;
    }
private:
    T m_value;
    std::memory_order m_memory_order;
};

template<class T>
CStatus
Core::atomic_store(IAddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const CVirtAddress address(rb().base());
    T const value = ra().scalar<T>();

    LOG_DEBUG(LOG_STORE)
        << "write atomic " << sizeof(T) << " bytes, " << address;

    CAtomicStore<T> op(value, order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

#define DEFINE_STORE_OPS(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME     ) { return core.atomic_store<TYPE>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## r) { return core.atomic_store<TYPE>(as, std::memory_order_release); } \

DEFINE_STORE_OPS(stab, u8)
DEFINE_STORE_OPS(stah, u16)
DEFINE_STORE_OPS(staw, u32)
DEFINE_STORE_OPS(stad, u64)
DEFINE_STORE_OPS(staq, u128)

/***************************************************************************
* CAS
***************************************************************************/
template<class T>
class CAtomicCompareAndSwap : public IMemoryOperation {
public:
    CAtomicCompareAndSwap(T expected, T desired, std::memory_order success_order, std::memory_order fail_order)
        : m_expected(expected)
        , m_desired(desired)
        , m_success_order(success_order)
        , m_fail_order(fail_order)
    {}

    ~CAtomicCompareAndSwap() override {}

    CStatus complex_operation(void *address) override {
        std::atomic<T> *r = reinterpret_cast<std::atomic<T> *>(address);
        std::atomic_compare_exchange_weak_explicit(r, &m_expected, m_desired, m_success_order, m_fail_order);
        return CStatus::continue_execution;
    }

    T GetOldValue(void) const { return m_expected; }

private:
    T                   m_expected;
    T                   m_desired;
    std::memory_order   m_success_order;
    std::memory_order   m_fail_order;
};

template<class T>
CStatus
Core::compare_and_swap(IAddressSpace& as, std::memory_order order, std::memory_order fail_order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    CGeneralRegister& dst = ra();
    T expected = dst.scalar<T>();
    const CVirtAddress address(rb().base());
    const T desired = rc().scalar<T>();
    CAtomicCompareAndSwap op(expected, desired, order, fail_order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    dst = op.GetOldValue();
    return FinalizeInstruction();
}

#define DEFINE_CAS_OPS(NAME, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME      ) { return core.compare_and_swap<TYPE>(as, std::memory_order_relaxed, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ##  a) { return core.compare_and_swap<TYPE>(as, std::memory_order_acquire, std::memory_order_acquire); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ##  r) { return core.compare_and_swap<TYPE>(as, std::memory_order_release, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## ar) { return core.compare_and_swap<TYPE>(as, std::memory_order_acq_rel, std::memory_order_acquire); } \

DEFINE_CAS_OPS(casb, u8)
DEFINE_CAS_OPS(cash, u16)
DEFINE_CAS_OPS(casw, u32)
DEFINE_CAS_OPS(casd, u64)
DEFINE_CAS_OPS(casq, u128)

/***************************************************************************
* RMW
***************************************************************************/
class fetch_exchange {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
        T old_value = std::atomic_exchange_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

// universal RMW via CAS
template<class OP>
class fetch_common {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order memory_order, std::memory_order fail_order)
    {
        T oldVal, newVal;
        do {
            oldVal = std::atomic_load_explicit(&atomic, memory_order);
            newVal = OP::template func<T>(oldVal, value);
        } while (!std::atomic_compare_exchange_weak_explicit(&atomic, &oldVal, newVal, memory_order, fail_order));
        return oldVal;
    }
};

class fetch_add {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order, std::memory_order fail_order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<add>::fetch_and_op_atomic<T>(atomic, value, order, fail_order);
        } else {
            return std::atomic_fetch_add_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_and {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order, std::memory_order fail_order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_and>::fetch_and_op_atomic<T>(atomic, value, order, fail_order);
        } else {
            return std::atomic_fetch_and_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_or {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order, std::memory_order fail_order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_or>::fetch_and_op_atomic<T>(atomic, value, order, fail_order);
        } else {
            return std::atomic_fetch_or_explicit<T>(&atomic, value, order);
        }
    }
};

class fetch_xor {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order order, std::memory_order fail_order) {
        if constexpr (std::is_same_v<T, u128>) {
            return fetch_common<bitwise_xor>::fetch_and_op_atomic<T>(atomic, value, order, fail_order);
        } else {
            return std::atomic_fetch_xor_explicit<T>(&atomic, value, order);
        }
    }
};

template<class T, class OP>
class CAtomicFetchAndOp : public IMemoryOperation {
public:
    CAtomicFetchAndOp(T value, std::memory_order success_order, std::memory_order fail_order)
        : m_value(value)
        , m_success_order(success_order)
        , m_fail_order(fail_order)
    {}

    ~CAtomicFetchAndOp() override {}

    CStatus complex_operation(void *address) override
    {
        std::atomic<T> *r = reinterpret_cast<std::atomic<T>*>(address);
        m_value = OP::fetch_and_op_atomic(*r, m_value, m_success_order, m_fail_order);
        return CStatus::continue_execution;
    }

    T GetOldValue(void) const { return m_value; }

private:
    T m_value;
    std::memory_order m_success_order;
    std::memory_order m_fail_order;
};

template<class T, class OP>
CStatus
Core::fetch_and_op_atomic(IAddressSpace& as, std::memory_order order, std::memory_order fail_order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    CGeneralRegister& oldvalue = ra();
    const CVirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    CAtomicFetchAndOp<T,OP> op(value, order, fail_order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    oldvalue = op.GetOldValue();
    return FinalizeInstruction();
}

#define DEFINE_FETCH_AND_OP_OPS(NAME, FUNC, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME      ) { return core.fetch_and_op_atomic<TYPE, FUNC>(as, std::memory_order_relaxed, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ##  a) { return core.fetch_and_op_atomic<TYPE, FUNC>(as, std::memory_order_acquire, std::memory_order_acquire); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ##  r) { return core.fetch_and_op_atomic<TYPE, FUNC>(as, std::memory_order_release, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## ar) { return core.fetch_and_op_atomic<TYPE, FUNC>(as, std::memory_order_acq_rel, std::memory_order_acquire); } \

#define DEFINE_FETCH_AND_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_FETCH_AND_OP_OPS(NAME ## b, FUNC, TYPE ## 8) \
    DEFINE_FETCH_AND_OP_OPS(NAME ## h, FUNC, TYPE ## 16) \
    DEFINE_FETCH_AND_OP_OPS(NAME ## w, FUNC, TYPE ## 32) \
    DEFINE_FETCH_AND_OP_OPS(NAME ## d, FUNC, TYPE ## 64) \
    DEFINE_FETCH_AND_OP_OPS(NAME ## q, FUNC, TYPE ## 128) \

DEFINE_FETCH_AND_OP_TYPES(swap,   fetch_exchange,    u)
DEFINE_FETCH_AND_OP_TYPES(ldadd,  fetch_add,         u)
DEFINE_FETCH_AND_OP_TYPES(ldand,  fetch_and,         u)
DEFINE_FETCH_AND_OP_TYPES(ldor,   fetch_or,          u)
DEFINE_FETCH_AND_OP_TYPES(ldxor,  fetch_xor,         u)
DEFINE_FETCH_AND_OP_TYPES(ldsmin, fetch_common<min>, i)
DEFINE_FETCH_AND_OP_TYPES(ldsmax, fetch_common<max>, i)
DEFINE_FETCH_AND_OP_TYPES(ldumin, fetch_common<min>, u)
DEFINE_FETCH_AND_OP_TYPES(ldumax, fetch_common<max>, u)

/***************************************************************************
* store-op
***************************************************************************/
template<class T, class OP>
CStatus Core::store_op_atomic(IAddressSpace& as, std::memory_order order)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    const CVirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    CAtomicFetchAndOp<T,OP> op(value, order, std::memory_order_relaxed);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

#define DEFINE_STORE_AND_OP_MODES(NAME, FUNC, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME     ) { return core.store_op_atomic<TYPE, FUNC>(as, std::memory_order_relaxed); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## r) { return core.store_op_atomic<TYPE, FUNC>(as, std::memory_order_release); } \

#define DEFINE_STORE_AND_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_STORE_AND_OP_MODES(NAME ## b, FUNC, TYPE ##  8) \
    DEFINE_STORE_AND_OP_MODES(NAME ## h, FUNC, TYPE ##  16) \
    DEFINE_STORE_AND_OP_MODES(NAME ## w, FUNC, TYPE ##  32) \
    DEFINE_STORE_AND_OP_MODES(NAME ## d, FUNC, TYPE ##  64) \
    DEFINE_STORE_AND_OP_MODES(NAME ## q, FUNC, TYPE ##  128) \

DEFINE_STORE_AND_OP_TYPES(stadd,  fetch_add,         u)
DEFINE_STORE_AND_OP_TYPES(stand,  fetch_and,         u)
DEFINE_STORE_AND_OP_TYPES(stor,   fetch_or,          u)
DEFINE_STORE_AND_OP_TYPES(stxor,  fetch_xor,         u)
DEFINE_STORE_AND_OP_TYPES(stsmin, fetch_common<min>, i)
DEFINE_STORE_AND_OP_TYPES(stsmax, fetch_common<max>, i)
DEFINE_STORE_AND_OP_TYPES(stumin, fetch_common<min>, u)
DEFINE_STORE_AND_OP_TYPES(stumax, fetch_common<max>, u)

} // namespace postrisc
