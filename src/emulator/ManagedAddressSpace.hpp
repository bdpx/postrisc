#pragma once

#include "AddressSpace.hpp"
#include "TranslationHelpers.hpp"
#include "Core.hpp"

namespace postrisc {

/***************************************************************************
* AddressSpace-derived specializations implement raw interfaces
* (single usermode app or whole system)
* this template class translates MMU interface:
*    fetch, read, write, complex_operation,
*    read_finalized, read_backing_store, write_backing_store, translate
* into raw memory interface:
*    read, write, complex_operation, translate
* usage:
*    IMPL : public ManagedAddressSpace<IMPL>
*
* DerivedT: should implement:
*    TranslateAddress
***************************************************************************/

template<typename DerivedT>
class ManagedAddressSpace : public AddressSpace {
public:
    ManagedAddressSpace(u32 bits_per_page_offset, Decoder& decoder)
        : AddressSpace(bits_per_page_offset, decoder) {}

    ~ManagedAddressSpace(void) override {};

    Result mmu_fetch_code(Core& core, const VirtAddress va, size_t len, void *data) override;

    Result mmu_read_u8  (Core& core, const VirtAddress va, u8& value) const override;
    Result mmu_read_u16 (Core& core, const VirtAddress va, u16& value) const override;
    Result mmu_read_u32 (Core& core, const VirtAddress va, u32& value) const override;
    Result mmu_read_u64 (Core& core, const VirtAddress va, u64& value) const override;
    Result mmu_read_u128(Core& core, const VirtAddress va, u128& value) const override;

    Result mmu_write_u8  (Core& core, const VirtAddress va, u8 value) override;
    Result mmu_write_u16 (Core& core, const VirtAddress va, u16 value) override;
    Result mmu_write_u32 (Core& core, const VirtAddress va, u32 value) override;
    Result mmu_write_u64 (Core& core, const VirtAddress va, u64 value) override;
    Result mmu_write_u128(Core& core, const VirtAddress va, u128 value) override;

    Result mmu_read_u32_finalized(Core& core, const VirtAddress va, u32& value) const override;
    Result mmu_read_u64_finalized(Core& core, const VirtAddress va, u64& value) const override;
    Result mmu_read_backing_store(Core& core, const VirtAddress va, size_t len, void *data) const override;
    Result mmu_write_backing_store(Core& core, const VirtAddress va, size_t len, void const *data) override;
    Result mmu_complex_operation(Core& core, const VirtAddress va, size_t len, u64 requested_rights,
                                  u64 unexpected_rights, MemoryOperation& op) override;
    std::tuple<Result, PhysAddress> mmu_translate(Core& core, const VirtAddress va) override;

private:
    template<typename TYPE> Result mmu_read_unaligned(Core& core, const VirtAddress va, TYPE& data) const;
    template<typename TYPE> Result mmu_write_unaligned(Core& core, const VirtAddress va, TYPE data);

    DerivedT *derived(void) { return static_cast<DerivedT*>(this); }
    const DerivedT *derived(void) const { return static_cast<const DerivedT*>(this); }
};

#define CHECK_ALIGNMENT_VAR(helper, address, size)                \
    if (unlikely((address.get_address() | size) & (size - 1))) {  \
        LOG_ERROR(LOG_EXCEPTION)                                  \
            << "unaligned address: " << address                   \
            << " size=" << size;                                  \
        return helper::unaligned_address();                       \
    }                                                             \

#define CHECK_ALIGNMENT(helper, address, size)                    \
    if constexpr (size > 1) {                                     \
        CHECK_ALIGNMENT_VAR(helper, address, size)                \
    }


/***************************************************************************
* MMU interface
***************************************************************************/
template<typename DerivedT>
Result
ManagedAddressSpace<DerivedT>::mmu_fetch_code(Core& core,
    const VirtAddress va, size_t len, void *data)
{
    LOG_DEBUG(LOG_ICACHE) << "fetch: " << va;
    CHECK_ALIGNMENT(InstructionHelper, va, sizeof(Bundle));

    auto [result, pa] =
        derived()->template TranslateAddress<InstructionHelper>(core, va,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);

    CHECK_RESULT(result);
    result = derived()->DerivedT::bus_read(pa, len, data);
    return result;
}

template<typename DerivedT>
template<typename TYPE>
Result
ManagedAddressSpace<DerivedT>::mmu_read_unaligned(Core& core, const VirtAddress va, TYPE& data) const
{
    LOG_DEBUG(LOG_EXCEPTION) << "unaligned addr=" << va;
    core.performance_monitor.report_event(PMC_UNALIGNED_READ);

    size_t const rest = va.get_address() & (sizeof(TYPE) - 1);
    assert(rest != 0);

    union {
       TYPE value;
       std::byte array[sizeof(TYPE)];
    } u;

    auto [result, pa] =
        derived()->template TranslateAddress<DataHelper>(core, va, VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);
    CHECK_RESULT(result);

    result = derived()->DerivedT::bus_read(pa, sizeof(TYPE) - rest, u.array);
    CHECK_RESULT(result);

    VirtAddress va_next(va.get_address() + sizeof(TYPE) - rest);
    auto [result_next, pa_next] =
        derived()->template TranslateAddress<DataHelper>(core, va_next, VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);
    CHECK_RESULT(result_next);

    result = derived()->DerivedT::bus_read(pa_next, rest, &u.array[sizeof(TYPE) - rest]);
    CHECK_RESULT(result);

    data = u.value;
    return Result::continue_execution;
}

#define MMU_READ_TYPED_HANDLER(TYPE)                                                     \
template<typename DerivedT>                                                              \
Result ManagedAddressSpace<DerivedT>::                                                   \
mmu_read_##TYPE(Core& core, const VirtAddress va, TYPE& data) const                      \
{                                                                                        \
    LOG_DEBUG(LOG_LOAD) << "read_" #TYPE ": "                                            \
        << get_decoder().get_address_info(va.get_address());                             \
                                                                                         \
    if constexpr (sizeof(TYPE) > 1) {                                                    \
        if (unlikely((va.get_address() & (sizeof(TYPE) - 1)) != 0)) {                    \
            return mmu_read_unaligned<TYPE>(core, va, data);                             \
        }                                                                                \
    }                                                                                    \
    auto [result, pa] =                                                                  \
        derived()->template TranslateAddress<DataHelper>(core, va,                       \
            VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);                                     \
    CHECK_RESULT(result);                                                                \
    result = derived()->DerivedT::bus_read_##TYPE(pa, data);                             \
    LOG_DEBUG(LOG_LOAD) << "value: " << fmt::data_logger(&data, sizeof(data));           \
    return result;                                                                       \
}

MMU_READ_TYPED_HANDLER(u8)
MMU_READ_TYPED_HANDLER(u16)
MMU_READ_TYPED_HANDLER(u32)
MMU_READ_TYPED_HANDLER(u64)
MMU_READ_TYPED_HANDLER(u128)

template<typename DerivedT>
template<typename TYPE>
Result
ManagedAddressSpace<DerivedT>::mmu_write_unaligned(Core& core, const VirtAddress va, TYPE data)
{
    LOG_DEBUG(LOG_EXCEPTION) << "unaligned addr=" << va;
    core.performance_monitor.report_event(PMC_UNALIGNED_WRITE);

    size_t const rest = va.get_address() & (sizeof(TYPE) - 1);
    assert(unlikely(rest != 0));

    union {
       TYPE value;
       std::byte array[sizeof(TYPE)];
    } u;
    u.value = data;

    auto [result, pa] =
        derived()->template TranslateAddress<DataHelper>(core, va,
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);
    CHECK_RESULT(result);

    result = derived()->DerivedT::bus_write(pa, sizeof(TYPE) - rest, u.array);
    CHECK_RESULT(result);

    VirtAddress va_next(va.get_address() + sizeof(TYPE) - rest);
    auto [result_next, pa_next] =
        derived()->template TranslateAddress<DataHelper>(core, va_next,
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);
    CHECK_RESULT(result_next);

    result = derived()->DerivedT::bus_write(pa_next, rest, &u.array[sizeof(TYPE) - rest]);
    return result;
}

#define MMU_WRITE_TYPED_HANDLER(TYPE)                                                    \
template<typename DerivedT>                                                              \
Result ManagedAddressSpace<DerivedT>::                                                   \
mmu_write_##TYPE(Core& core, const VirtAddress va, TYPE data)                            \
{                                                                                        \
    LOG_DEBUG(LOG_STORE) << "write_" #TYPE ": "                                          \
        << get_decoder().get_address_info(va.get_address());                             \
                                                                                         \
    if constexpr (sizeof(TYPE) > 1) {                                                    \
        if (unlikely((va.get_address() & (sizeof(TYPE) - 1)) != 0)) {                    \
            return mmu_write_unaligned<TYPE>(core, va, data);                            \
        }                                                                                \
    }                                                                                    \
    auto [result, pa] =                                                                  \
        derived()->template TranslateAddress<DataHelper>(core, va,                       \
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);   \
    CHECK_RESULT(result);                                                                \
    result = derived()->DerivedT::bus_write_##TYPE(pa, data);                            \
    LOG_DEBUG(LOG_STORE) << "value: " << fmt::data_logger(&data, sizeof(data));          \
    return result;                                                                       \
}

MMU_WRITE_TYPED_HANDLER(u8)
MMU_WRITE_TYPED_HANDLER(u16)
MMU_WRITE_TYPED_HANDLER(u32)
MMU_WRITE_TYPED_HANDLER(u64)
MMU_WRITE_TYPED_HANDLER(u128)

#define MMU_READ_FINALIZED_TYPED_HANDLER(TYPE)                                           \
template<typename DerivedT>                                                              \
Result                                                                                   \
ManagedAddressSpace<DerivedT>::mmu_read_##TYPE##_finalized(Core& core,                   \
    const VirtAddress va, TYPE& data) const                                              \
{                                                                                        \
    LOG_DEBUG(LOG_LOAD) << "read_" #TYPE "_finalized: "                                  \
        << get_decoder().get_address_info(va.get_address());                             \
    CHECK_ALIGNMENT(DataHelper, va, sizeof(TYPE));                                       \
    auto [result, pa] =                                                                  \
        derived()->template TranslateAddress<DataHelper>(core, va,                       \
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL,                                    \
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);  \
    CHECK_RESULT(result);                                                                \
    result = derived()->DerivedT::bus_read_##TYPE(pa, data);                             \
    LOG_DEBUG(LOG_LOAD) << "value: " << fmt::data_logger(&data, sizeof(data));           \
    return result;                                                                       \
}

MMU_READ_FINALIZED_TYPED_HANDLER(u32)
MMU_READ_FINALIZED_TYPED_HANDLER(u64)

template<typename DerivedT>
Result
ManagedAddressSpace<DerivedT>::mmu_read_backing_store(Core& core,
    const VirtAddress va, size_t len, void *data) const
{
    LOG_DEBUG(LOG_LOAD)
        << "read backstore: len=" << len
        << " addr=" << va;

    CHECK_ALIGNMENT_VAR(BackstoreHelper, va, len);

    auto [result, pa] =
        derived()->template TranslateAddress<BackstoreHelper>(core, va,
            VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE); // usual read is allowed

    CHECK_RESULT(result);
    result = derived()->DerivedT::bus_read(pa, len, data);
    return result;
}

template<typename DerivedT>
Result ManagedAddressSpace<DerivedT>::mmu_write_backing_store(
    Core& core, const VirtAddress va, size_t len, const void *data)
{
    LOG_DEBUG(LOG_STORE)
        << "write backstore: len=" << len
        << " addr=" << va;
    CHECK_ALIGNMENT_VAR(BackstoreHelper, va, len);

    // VM_PAGE_RIGHT_READ is allowed for backtrace reconstruction, so not listed in `unexpected_rights`
    auto [result, pa] =
        derived()->template TranslateAddress<BackstoreHelper>(core, va,
            VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE);

    CHECK_RESULT(result);
    result = derived()->DerivedT::bus_write(pa, len, data);
    return result;
}

template<typename DerivedT>
std::tuple<Result, PhysAddress>
ManagedAddressSpace<DerivedT>::mmu_translate(Core& core, const VirtAddress va)
{
    return derived()->template TranslateAddress<DataHelper>(core, va, 0, 0);
}

template<typename DerivedT>
Result ManagedAddressSpace<DerivedT>::mmu_complex_operation(
    Core& core, const VirtAddress va, size_t len, u64 requested_rights, u64 unexpected_rights, MemoryOperation& op)
{
    LOG_DEBUG(LOG_STORE)
        << "complex_operation: len=" << len
        << " addr=" << get_decoder().get_address_info(va.get_address());

    CHECK_ALIGNMENT_VAR(DataHelper, va, len);

    auto [result, pa] = derived()->template TranslateAddress<DataHelper>(core, va, requested_rights, unexpected_rights);

    CHECK_RESULT(result);
    result = derived()->DerivedT::bus_complex_operation(pa, len, op);
    return result;
}

} // namespace postrisc
