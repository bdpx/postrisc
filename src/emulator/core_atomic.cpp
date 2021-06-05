#include <atomic>

#include "util/common.hpp"
#include "config/emulator_configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"

namespace postrisc {

// we expect native support for 1-2-4-8-16-bytes atomics
// (means without embedded mutexes)
// so we may treat any memory location as target for atomic operation
static_assert(sizeof(std::atomic<  u8>) == sizeof(u8));
static_assert(sizeof(std::atomic< u16>) == sizeof(u16));
static_assert(sizeof(std::atomic< u32>) == sizeof(u32));
static_assert(sizeof(std::atomic< u64>) == sizeof(u64));
static_assert(sizeof(std::atomic<u128>) == sizeof(u128));

static std::memory_order GetCorrespondingFailOrder(std::memory_order memory_order)
{
    switch (memory_order) {
        case std::memory_order_relaxed: return std::memory_order_relaxed;
        case std::memory_order_acquire: return std::memory_order_acquire;
        case std::memory_order_release: return std::memory_order_relaxed;
        case std::memory_order_acq_rel: return std::memory_order_acquire;
        case std::memory_order_seq_cst: return std::memory_order_seq_cst;
        default:
            LOG_ERROR(LOG_EXCEPTION)
                << "invalid memory order without correponding failing order " << (unsigned)memory_order;
            assert(0);
            return std::memory_order_relaxed;
    }
}

DEFINE_INSN_IMPLEMENTATION(fence)
{
    UNREFERENCED_PARAMETER(as);
    std::memory_order order;
    switch (core.cmd.memory_order) {
        case EMemoryOrder::acquire: order = std::memory_order_acquire; break;
        case EMemoryOrder::release: order = std::memory_order_release; break;
        case EMemoryOrder::acq_rel: order = std::memory_order_acq_rel; break;
        case EMemoryOrder::seq_cst: order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE)
                << "invalid atomic fence memory order " << (unsigned)core.cmd.memory_order;
            return CStatus::illegal_instruction;
    }

    std::atomic_thread_fence(order);
    return FinalizeInstruction();
}

/***************************************************************************
* atomic loads
***************************************************************************/
template<class T>
class CAtomicLoad : public IMemoryOperation {
public:
    CAtomicLoad(std::memory_order _memory_order)
        : target(0), memory_order(_memory_order) {}
    ~CAtomicLoad() override {}
    CStatus complex_operation(void * address) override
    {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        const std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        target = r->load(memory_order);
        return CStatus::continue_execution;
    }
    T GetTarget(void) const { return target; }

private:
    T target;
    std::memory_order memory_order;
};

template<class T>
CStatus
Core::atomic_load(IAddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    std::memory_order order;
    switch (cmd.memory_order) {
        case EMemoryOrder::relaxed: order = std::memory_order_relaxed; break;
        case EMemoryOrder::acquire: order = std::memory_order_acquire; break;
        case EMemoryOrder::seq_cst: order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE)
                << "invalid atomic load memory order " << (unsigned)cmd.memory_order;
            return CStatus::illegal_instruction;
    }

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

// load atomic
DEFINE_INSN_IMPLEMENTATION(ldab) { return core.atomic_load<  u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldah) { return core.atomic_load< u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldaw) { return core.atomic_load< u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldad) { return core.atomic_load< u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldaq) { return core.atomic_load<u128>(as); }

/***************************************************************************
* atomic stores
***************************************************************************/
template<class T>
class CAtomicStore : public IMemoryOperation {
public:
    CAtomicStore(T _value, std::memory_order _memory_order)
        : value(_value), memory_order(_memory_order) {}
    ~CAtomicStore() override {}
    CStatus complex_operation(void * address) override {
        static_assert(sizeof(std::atomic<T>) == sizeof(T));
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        r->store(value, memory_order);
        return CStatus::continue_execution;
    }
private:
    T value;
    std::memory_order memory_order;
};

template<class T>
CStatus
Core::atomic_store(IAddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    std::memory_order order;
    switch (cmd.memory_order) {
        case EMemoryOrder::relaxed: order = std::memory_order_relaxed; break;
        case EMemoryOrder::release: order = std::memory_order_release; break;
        case EMemoryOrder::seq_cst: order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE)
                << "invalid atomic store memory order " << (unsigned)cmd.memory_order;
            return CStatus::illegal_instruction;
    }


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

DEFINE_INSN_IMPLEMENTATION(stab) { return core.atomic_store<  u8>(as); }
DEFINE_INSN_IMPLEMENTATION(stah) { return core.atomic_store< u16>(as); }
DEFINE_INSN_IMPLEMENTATION(staw) { return core.atomic_store< u32>(as); }
DEFINE_INSN_IMPLEMENTATION(stad) { return core.atomic_store< u64>(as); }
DEFINE_INSN_IMPLEMENTATION(staq) { return core.atomic_store<u128>(as); }

/***************************************************************************
* RMW
***************************************************************************/
class fetch_exchange {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order)
    {
        T old_value = std::atomic_exchange_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

class fetch_add {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order)
    {
        T old_value = std::atomic_fetch_add_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

class fetch_and {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order)
    {
        T old_value = std::atomic_fetch_and_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

class fetch_or {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order)
    {
        T old_value = std::atomic_fetch_or_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

class fetch_xor {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order)
    {
        T old_value = std::atomic_fetch_xor_explicit<T>(&atomic, value, memory_order);
        return old_value;
    }
};

// non-standard RMW via CAS
template<class OP>
class fetch_common {
public:
    template<class T>
    static T fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order memory_order)
    {
        std::memory_order fail_order = GetCorrespondingFailOrder(memory_order);
        T oldVal, newVal;
        do {
            oldVal = std::atomic_load_explicit(&atomic, memory_order);
            newVal = OP::func(oldVal, value);
        } while (!std::atomic_compare_exchange_weak_explicit(&atomic, &oldVal, newVal,
                                                             memory_order, fail_order));
        return oldVal;
    }
};

template<class T, class OP>
class CAtomicFetchAndOp : public IMemoryOperation {
public:
    CAtomicFetchAndOp(T _value, std::memory_order _success_order)
        : value(_value)
        , success_order(_success_order)
    {}

    ~CAtomicFetchAndOp() override {}

    CStatus complex_operation(void * address) override
    {
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        value = OP::fetch_and_op_atomic(*r, value, success_order);
        return CStatus::continue_execution;
    }

    T GetOldValue(void) const { return value; }

private:
    T                   value;
    std::memory_order   success_order;
};

template<class T, class OP>
CStatus
Core::fetch_and_op_atomic(IAddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    std::memory_order success_order;
    switch (cmd.memory_order) {
        case EMemoryOrder::relaxed: success_order = std::memory_order_relaxed; break;
        case EMemoryOrder::acquire: success_order = std::memory_order_acquire; break;
        case EMemoryOrder::release: success_order = std::memory_order_release; break;
        case EMemoryOrder::acq_rel: success_order = std::memory_order_acq_rel; break;
        case EMemoryOrder::seq_cst: success_order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE) << "invalid RMW memory order" << (unsigned)cmd.memory_order;
            return CStatus::illegal_instruction;
    }

    CGeneralRegister & oldvalue = ra();
    const CVirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    CAtomicFetchAndOp<T,OP> op(value, success_order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    oldvalue = op.GetOldValue();
    return FinalizeInstruction();
}

#define DEFINE_LOAD_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME ## b) { return core.fetch_and_op_atomic< TYPE ##  8, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## h) { return core.fetch_and_op_atomic< TYPE ## 16, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## w) { return core.fetch_and_op_atomic< TYPE ## 32, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## d) { return core.fetch_and_op_atomic< TYPE ## 64, FUNC>(as); } \

DEFINE_LOAD_OP_TYPES(ldadd,  fetch_add,         u)
DEFINE_LOAD_OP_TYPES(ldand,  fetch_and,         u)
DEFINE_LOAD_OP_TYPES(ldor,   fetch_or,          u)
DEFINE_LOAD_OP_TYPES(ldxor,  fetch_xor,         u)
DEFINE_LOAD_OP_TYPES(ldsmin, fetch_common<min>, i)
DEFINE_LOAD_OP_TYPES(ldsmax, fetch_common<max>, i)
DEFINE_LOAD_OP_TYPES(ldumin, fetch_common<min>, u)
DEFINE_LOAD_OP_TYPES(ldumax, fetch_common<max>, u)
DEFINE_LOAD_OP_TYPES(swap,   fetch_exchange,    u)

DEFINE_INSN_IMPLEMENTATION(swapq) { return core.fetch_and_op_atomic<u128, fetch_exchange>(as); }

/***************************************************************************
* store-op
***************************************************************************/
template<class T, class OP> CStatus Core::store_op_atomic(IAddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    std::memory_order success_order;
    switch (cmd.memory_order) {
        case EMemoryOrder::relaxed: success_order = std::memory_order_relaxed; break;
        case EMemoryOrder::acquire: success_order = std::memory_order_acquire; break;
        case EMemoryOrder::release: success_order = std::memory_order_release; break;
        case EMemoryOrder::acq_rel: success_order = std::memory_order_acq_rel; break;
        case EMemoryOrder::seq_cst: success_order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE) << "invalid RMW memory order" << (unsigned)cmd.memory_order;
            return CStatus::illegal_instruction;
    }

    const CVirtAddress address(rb().base());

    LOG_DEBUG(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, " << address;

    const T value = rc().scalar<T>();
    CAtomicFetchAndOp<T,OP> op(value, success_order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));

    return FinalizeInstruction();
}

#define DEFINE_STORE_OP_TYPES(NAME, FUNC, TYPE) \
    DEFINE_INSN_IMPLEMENTATION(NAME ## b) { return core.store_op_atomic< TYPE ##  8, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## h) { return core.store_op_atomic< TYPE ## 16, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## w) { return core.store_op_atomic< TYPE ## 32, FUNC>(as); } \
    DEFINE_INSN_IMPLEMENTATION(NAME ## d) { return core.store_op_atomic< TYPE ## 64, FUNC>(as); } \

DEFINE_STORE_OP_TYPES(stadd,  fetch_add,         u)
DEFINE_STORE_OP_TYPES(stand,  fetch_and,         u)
DEFINE_STORE_OP_TYPES(stor,   fetch_or,          u)
DEFINE_STORE_OP_TYPES(stxor,  fetch_xor,         u)
DEFINE_STORE_OP_TYPES(stsmin, fetch_common<min>, i)
DEFINE_STORE_OP_TYPES(stsmax, fetch_common<max>, i)
DEFINE_STORE_OP_TYPES(stumin, fetch_common<min>, u)
DEFINE_STORE_OP_TYPES(stumax, fetch_common<max>, u)

/***************************************************************************
* CAS
***************************************************************************/
template<class T>
class CAtomicCompareAndSwap : public IMemoryOperation {
public:
    CAtomicCompareAndSwap(T _expected, T _desired, std::memory_order _memory_order)
        : expected(_expected)
        , desired(_desired)
        , memory_order(_memory_order)
    {}

    ~CAtomicCompareAndSwap() override {}

    CStatus complex_operation(void * address) override
    {
        std::memory_order fail_order = GetCorrespondingFailOrder(memory_order);
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        std::atomic_compare_exchange_weak_explicit(r, &expected, desired, memory_order, fail_order);
        return CStatus::continue_execution;
    }

    T GetOldValue(void) const { return expected; }

private:
    T                   expected;
    T                   desired;
    std::memory_order   memory_order;
};

template<class T>
CStatus
Core::compare_and_swap(IAddressSpace& as)
{
    static_assert(sizeof(std::atomic<T>) == sizeof(T));

    std::memory_order success_order;
    switch (cmd.memory_order) {
        case EMemoryOrder::relaxed: success_order = std::memory_order_relaxed; break;
        case EMemoryOrder::acquire: success_order = std::memory_order_acquire; break;
        case EMemoryOrder::release: success_order = std::memory_order_release; break;
        case EMemoryOrder::acq_rel: success_order = std::memory_order_acq_rel; break;
        case EMemoryOrder::seq_cst: success_order = std::memory_order_seq_cst; break;
        default:
            LOG_ERROR(LOG_LOAD | LOG_STORE) << "invalid CAS memory order" << (unsigned)cmd.memory_order;
            return CStatus::illegal_instruction;
    }

    CGeneralRegister & dst = ra();
    T expected = dst.scalar<T>();
    const CVirtAddress address(rb().base());
    const T desired = rc().scalar<T>();
    CAtomicCompareAndSwap op(expected, desired, success_order);
    CHECK_STATUS_ADDRESS(*this, address,
        as.mmu_complex_operation(*this, address, sizeof(T),
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL, op));
    dst = op.GetOldValue();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(casb) { return core.compare_and_swap<  u8>(as); }
DEFINE_INSN_IMPLEMENTATION(cash) { return core.compare_and_swap< u16>(as); }
DEFINE_INSN_IMPLEMENTATION(casw) { return core.compare_and_swap< u32>(as); }
DEFINE_INSN_IMPLEMENTATION(casd) { return core.compare_and_swap< u64>(as); }
DEFINE_INSN_IMPLEMENTATION(casq) { return core.compare_and_swap<u128>(as); }

} // namespace postrisc
