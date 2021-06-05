#pragma once

#include <atomic>

namespace postrisc {

// we expect native support for 4-8-16-bytes atomics
// (means without embedded mutexes)
// so we may treat any memory location as target for atomic operation
STATIC_ASSERT(sizeof(std::atomic<uint16_t>) == sizeof(uint16_t));
STATIC_ASSERT(sizeof(std::atomic<uint32_t>) == sizeof(uint32_t));
STATIC_ASSERT(sizeof(std::atomic<uint64_t>) == sizeof(uint64_t));
STATIC_ASSERT(sizeof(std::atomic<__uint128_t>) == sizeof(__uint128_t));

CStatus
Core::TranslateLoadMemoryOrder(const EMemoryOrdering memorder, std::memory_order & stdorder)
{
    switch (memorder) {
        case order_relaxed:          stdorder = std::memory_order_relaxed; break;
        case order_acquire:          stdorder = std::memory_order_acquire; break;
        //case order_release:          stdorder = std::memory_order_release; break;
        case order_acquire_release:  stdorder = std::memory_order_acq_rel; break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "invalid memory order " << memorder;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::TranslateStoreMemoryOrder(const EMemoryOrdering memorder, std::memory_order & stdorder)
{
    switch (memorder) {
        case order_relaxed:          stdorder = std::memory_order_relaxed; break;
        //case order_acquire:          stdorder = std::memory_order_acquire; break;
        case order_release:          stdorder = std::memory_order_release; break;
        case order_acquire_release:  stdorder = std::memory_order_acq_rel; break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "invalid memory order " << memorder;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::TranslateMemoryOrder(const EMemoryOrdering memorder, std::memory_order & success_order, std::memory_order & fail_order)
{
    switch (memorder) {
        case order_relaxed:
            success_order = std::memory_order_relaxed;
            fail_order = std::memory_order_relaxed;
            break;
        case order_acquire:
            success_order = std::memory_order_acquire;
            fail_order = std::memory_order_acquire;
            break;
        case order_release:
            success_order = std::memory_order_release;
            fail_order = std::memory_order_relaxed;
            break;
        case order_acquire_release:
            success_order = std::memory_order_acq_rel;
            fail_order = std::memory_order_acquire;
            break;
        default:
            LOG_ERROR(LOG_EXCEPTION) << "invalid memory order " << memorder;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

/***************************************************************************
* atomic load
***************************************************************************/
template<class T>
class CAtomicLoad : public IMemoryOperation
{
public:
    CAtomicLoad(std::memory_order _memory_order)
        : memory_order(_memory_order)
        , target(0) {}
    ~CAtomicLoad() override {}
    size_t size(void) const override { return sizeof(T); }
    CStatus complex_operation(void * address) override
    {
        const std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        target = r->load(memory_order);
        return CStatus(CStatus::continue_execution);
    }
    T GetTarget(void) const { return target; }

private:
    std::memory_order memory_order;
    T target;
};

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::read_atomic(const CMD cmd)
{
    STATIC_ASSERT(sizeof(std::atomic<T>) == sizeof(T));
    GET_WRA;
    GET_RRB;

    const uint64_t address = b.base();

    const EMemoryOrdering memorder = cmd.GetMemoryOrdering();
    std::memory_order memory_order;
    CHECK_STATUS(TranslateLoadMemoryOrder(memorder, memory_order));

    LOG_TRACE(LOG_STORE)
        << "read atomic " << sizeof(T) << " bytes, "
        << fmt::hex(address);

    CAtomicLoad<T> op(memory_order);
    CHECK_STATUS_ADDRESS(address,
        mmu_complex_operation(address, VM_PAGE_RIGHT_READ, op));
    a = op.GetTarget();

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1a)  { return read_atomic<uint8_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld2a)  { return read_atomic<uint16_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld4a)  { return read_atomic<uint32_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld8a)  { return read_atomic<uint64_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld16a) { return read_atomic<__uint128_t>(cmd); }

/***************************************************************************
* atomic store
***************************************************************************/
template<class T>
class CAtomicStore : public IMemoryOperation {
public:
    CAtomicStore(std::memory_order _memory_order, T _value)
        : memory_order(_memory_order)
        , value(_value)
    {}
    ~CAtomicStore() override {}
    size_t size(void) const override { return sizeof(T); }
    CStatus complex_operation(void * address) override
    {
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        r->store(value, memory_order);
        return CStatus(CStatus::continue_execution);
    }

private:
    std::memory_order memory_order;
    T value;
};

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::write_atomic(const CMD cmd)
{
    GET_RRA;
    GET_RRB;

    const uint64_t address = b.base();
    const T value = static_cast<T>(a);

    const EMemoryOrdering memorder = cmd.GetMemoryOrdering();
    std::memory_order memory_order;
    CHECK_STATUS(TranslateStoreMemoryOrder(memorder, memory_order));

#if defined(__unix__) || defined(__GNUC__)
    STATIC_ASSERT(sizeof(std::atomic<T>) == sizeof(T));
#endif

    LOG_TRACE(LOG_STORE)
        << "write atomic " << sizeof(T) << " bytes, "
        << fmt::hex(address);

    CAtomicStore<T> op(memory_order, value);
    CHECK_STATUS_ADDRESS(address,
        mmu_complex_operation(address, VM_PAGE_RIGHT_WRITE, op));

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1a)  { return write_atomic<uint8_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(st2a)  { return write_atomic<uint16_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(st4a)  { return write_atomic<uint32_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(st8a)  { return write_atomic<uint64_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(st16a) { return write_atomic<__uint128_t>(cmd); }


/***************************************************************************
* RMW
***************************************************************************/
class fetch_exchange {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
         T old_value = std::atomic_exchange_explicit<T>(&atomic, value, memory_order);
         return old_value;
    }
};

class fetch_add {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
         T old_value = std::atomic_fetch_add_explicit<T>(&atomic, value, memory_order);
         return old_value;
    }
};

class fetch_and {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
         T old_value = std::atomic_fetch_and_explicit<T>(&atomic, value, memory_order);
         return old_value;
    }
};

class fetch_or {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
         T old_value = std::atomic_fetch_or_explicit<T>(&atomic, value, memory_order);
         return old_value;
    }
};

class fetch_xor {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T> & atomic, T value, std::memory_order memory_order, std::memory_order UNUSED(fail_order))
    {
         T old_value = std::atomic_fetch_xor_explicit<T>(&atomic, value, memory_order);
         return old_value;
    }
};

template<class T, class OP>
class CAtomicFetchAndOp : public IMemoryOperation {
public:
    CAtomicFetchAndOp(T _value, std::memory_order _success_order, std::memory_order _fail_order)
        : value(_value)
        , success_order(_success_order)
        , fail_order(_fail_order)
    {}

    ~CAtomicFetchAndOp() override {}

    size_t size(void) const override { return sizeof(T); }
    CStatus complex_operation(void * address) override
    {
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        value = OP::fetch_and_op_atomic(*r, value, success_order, fail_order);
        return CStatus(CStatus::continue_execution);
    }

    T GetOldValue(void) const { return value; }

private:
    T                   value;
    std::memory_order   success_order;
    std::memory_order   fail_order;
};

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::fetch_and_op_atomic(const CMD cmd)
{
    STATIC_ASSERT(sizeof(std::atomic<T>) == sizeof(T));

    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister & oldvalue = a;
    const uint64_t address = b.base();

    LOG_TRACE(LOG_LOAD | LOG_STORE)
        << "RMW atomic " << sizeof(T) << " bytes, "
        << fmt::hex(address);

    const T value = c;

    const EMemoryOrdering memorder = cmd.GetMemoryOrdering();

    std::memory_order success_order;
    std::memory_order fail_order;
    CHECK_STATUS(TranslateMemoryOrder(memorder, success_order, fail_order));

    CAtomicFetchAndOp<T,OP> op(value, success_order, fail_order);
    CHECK_STATUS_ADDRESS(address,
        mmu_complex_operation(address, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE, op));

    oldvalue = op.GetOldValue();
    return FinalizeInstruction();
}

// atomic fetch-op
DEFINE_INSN_IMPLEMENTATION(amoswap4) { return fetch_and_op_atomic<uint32_t, fetch_exchange>(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoswap8) { return fetch_and_op_atomic<uint64_t, fetch_exchange>(cmd); }

DEFINE_INSN_IMPLEMENTATION(amoadd4)  { return fetch_and_op_atomic<uint32_t, fetch_add>(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoadd8)  { return fetch_and_op_atomic<uint64_t, fetch_add>(cmd); }

DEFINE_INSN_IMPLEMENTATION(amoand4)  { return fetch_and_op_atomic<uint32_t, fetch_and>(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoor4)   { return fetch_and_op_atomic<uint32_t, fetch_or >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoxor4)  { return fetch_and_op_atomic<uint32_t, fetch_xor>(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoand8)  { return fetch_and_op_atomic<uint64_t, fetch_and>(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoor8)   { return fetch_and_op_atomic<uint64_t, fetch_or >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amoxor8)  { return fetch_and_op_atomic<uint64_t, fetch_xor>(cmd); }

/***************************************************************************
* non-standard RMW via CAS
***************************************************************************/
template<class OP>
class fetch_common {
public:
    template<class T>
    static T
    fetch_and_op_atomic(std::atomic<T>& atomic, T value, std::memory_order success_order, std::memory_order fail_order)
    {
        T oldVal, newVal;

        do {
            oldVal = std::atomic_load_explicit(&atomic, success_order);
            newVal = OP::func(oldVal, value);
        } while (!std::atomic_compare_exchange_weak_explicit(&atomic, &oldVal, newVal, success_order, fail_order));

        return oldVal;
    }
};

DEFINE_INSN_IMPLEMENTATION(amomin4)  { return fetch_and_op_atomic<int32_t, fetch_common<min> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amomin8)  { return fetch_and_op_atomic<int64_t, fetch_common<min> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amomax4)  { return fetch_and_op_atomic<int32_t, fetch_common<max> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amomax8)  { return fetch_and_op_atomic<int64_t, fetch_common<max> >(cmd); }

DEFINE_INSN_IMPLEMENTATION(amominu4) { return fetch_and_op_atomic<uint32_t, fetch_common<min> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amominu8) { return fetch_and_op_atomic<uint64_t, fetch_common<min> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amomaxu4) { return fetch_and_op_atomic<uint32_t, fetch_common<max> >(cmd); }
DEFINE_INSN_IMPLEMENTATION(amomaxu8) { return fetch_and_op_atomic<uint64_t, fetch_common<max> >(cmd); }


/***************************************************************************
* CAS
***************************************************************************/
template<class T>
class CAtomicCompareAndSwap : public IMemoryOperation {
public:
    CAtomicCompareAndSwap(T _expected, T _desired, std::memory_order _success_order, std::memory_order _fail_order)
        : expected(_expected)
        , desired(_desired)
        , success_order(_success_order)
        , fail_order(_fail_order) {}

    ~CAtomicCompareAndSwap() override {}

    size_t size(void) const override { return sizeof(T); }
    CStatus complex_operation(void * address) override
    {
        std::atomic<T> * r = reinterpret_cast<std::atomic<T> *>(address);
        std::atomic_compare_exchange_weak_explicit(r, &expected, desired, success_order, fail_order);
        return CStatus(CStatus::continue_execution);
    }

    T GetOldValue(void) const { return expected; }

private:
    T                   expected;
    T                   desired;
    std::memory_order   success_order;
    std::memory_order   fail_order;
};

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::compare_and_swap(const CMD cmd)
{
    STATIC_ASSERT(sizeof(std::atomic<T>) == sizeof(T));
    GET_WRA;
    GET_RRB;
    GET_RRC;

    CGeneralRegister & dst = a;
    T expected = static_cast<T>(dst);
    const uint64_t address = b;

    const T desired = (T)c;

    const EMemoryOrdering memorder = cmd.GetMemoryOrdering();

    std::memory_order success_order;
    std::memory_order fail_order;
    CHECK_STATUS(TranslateMemoryOrder(memorder, success_order, fail_order));
    CAtomicCompareAndSwap op(expected, desired, success_order, fail_order);
    CHECK_STATUS_ADDRESS(address,
        mmu_complex_operation(address, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE, op));
    dst = op.GetOldValue();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(cas4)  {  return compare_and_swap<   uint32_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(cas8)  {  return compare_and_swap<   uint64_t>(cmd);  }
DEFINE_INSN_IMPLEMENTATION(cas16) {  return compare_and_swap<__uint128_t>(cmd);  }

} // namespace postrisc
