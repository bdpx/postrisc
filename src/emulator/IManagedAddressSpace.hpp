#pragma once

#include "IAddressSpace.hpp"
#include "translation_helpers.hpp"
#include "core.hpp"

namespace postrisc {

/***************************************************************************
* IAddressSpace-derived specializations implement raw interfaces
* (UsermodeOnly or FullSystem)
* this template class translates MMU interface:
*    fetch, read, write, complex_operation,
*    read_finalized, read_backing_store, write_backing_store, translate
* into raw memory interface:
*    read, write, complex_operation, translate
* usage:
*    IMPL : public IManagedAddressSpace<IMPL>
*
* DerivedT: should implement:
*    TranslateAddress
***************************************************************************/

template<class DerivedT>
class IManagedAddressSpace : public IAddressSpace {
public:
    IManagedAddressSpace(u32 bits_per_page_offset, Decoder& decoder)
        : IAddressSpace(bits_per_page_offset, decoder) {}

    ~IManagedAddressSpace(void) override {};

    CStatus mmu_fetch_code(Core& core, const CVirtAddress va, size_t len, void *data) override;

    CStatus mmu_read_u8  (Core& core, const CVirtAddress va, u8& value) const override;
    CStatus mmu_read_u16 (Core& core, const CVirtAddress va, u16& value) const override;
    CStatus mmu_read_u32 (Core& core, const CVirtAddress va, u32& value) const override;
    CStatus mmu_read_u64 (Core& core, const CVirtAddress va, u64& value) const override;
    CStatus mmu_read_u128(Core& core, const CVirtAddress va, u128& value) const override;

    CStatus mmu_write_u8  (Core& core, const CVirtAddress va, u8 value) override;
    CStatus mmu_write_u16 (Core& core, const CVirtAddress va, u16 value) override;
    CStatus mmu_write_u32 (Core& core, const CVirtAddress va, u32 value) override;
    CStatus mmu_write_u64 (Core& core, const CVirtAddress va, u64 value) override;
    CStatus mmu_write_u128(Core& core, const CVirtAddress va, u128 value) override;

    CStatus mmu_read_u32_finalized(Core& core, const CVirtAddress va, u32& value) const override;
    CStatus mmu_read_u64_finalized(Core& core, const CVirtAddress va, u64& value) const override;
    CStatus mmu_read_backing_store(Core& core, const CVirtAddress va, size_t len, void *data) const override;
    CStatus mmu_write_backing_store(Core& core, const CVirtAddress va, size_t len, void const *data) override;
    CStatus mmu_complex_operation(Core& core, const CVirtAddress va, size_t len, u64 requested_rights,
                                  u64 unexpected_rights, IMemoryOperation& op) override;
    std::tuple<CStatus, CPhysAddress> mmu_translate(Core& core, const CVirtAddress va) override;

private:
    template<class TYPE> CStatus mmu_read_unaligned(Core& core, const CVirtAddress va, TYPE& data) const;
    template<class TYPE> CStatus mmu_write_unaligned(Core& core, const CVirtAddress va, TYPE data);

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
template<class DerivedT>
CStatus
IManagedAddressSpace<DerivedT>::mmu_fetch_code(Core& core,
    const CVirtAddress va, size_t len, void *data)
{
    LOG_DEBUG(LOG_ICACHE) << "fetch: " << va;
    CHECK_ALIGNMENT(CInstructionHelper, va, sizeof(CBundle));

    auto [status, pa] =
        derived()->template TranslateAddress<CInstructionHelper>(core, va,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);

    CHECK_STATUS(status);
    status = derived()->DerivedT::bus_read(pa, len, data);
    return status;
}

template<class DerivedT>
template<class TYPE>
CStatus
IManagedAddressSpace<DerivedT>::mmu_read_unaligned(Core& core, const CVirtAddress va, TYPE& data) const
{
    LOG_DEBUG(LOG_EXCEPTION) << "unaligned addr=" << va;
    core.performance_monitor.report_event(PMC_UNALIGNED_READ);

    size_t const rest = va.get_address() & (sizeof(TYPE) - 1);
    assert(rest != 0);

    union {
       TYPE value;
       std::byte array[sizeof(TYPE)];
    } u;

    auto [status, pa] =
        derived()->template TranslateAddress<CDataHelper>(core, va, VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);
    CHECK_STATUS(status);

    status = derived()->DerivedT::bus_read(pa, sizeof(TYPE) - rest, u.array);
    CHECK_STATUS(status);

    CVirtAddress va_next(va.get_address() + sizeof(TYPE) - rest);
    auto [status_next, pa_next] =
        derived()->template TranslateAddress<CDataHelper>(core, va_next, VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);
    CHECK_STATUS(status_next);

    status = derived()->DerivedT::bus_read(pa_next, rest, &u.array[sizeof(TYPE) - rest]);
    CHECK_STATUS(status);

    data = u.value;
    return CStatus::continue_execution;
}

#define MMU_READ_TYPED_HANDLER(TYPE)                                                     \
template<class DerivedT>                                                                 \
CStatus IManagedAddressSpace<DerivedT>::                                                 \
mmu_read_##TYPE(Core& core, const CVirtAddress va, TYPE& data) const                     \
{                                                                                        \
    LOG_DEBUG(LOG_LOAD) << "read_" #TYPE ": "                                            \
        << get_decoder().get_address_info(va.get_address());                             \
                                                                                         \
    if constexpr (sizeof(TYPE) > 1) {                                                    \
        if (unlikely((va.get_address() & (sizeof(TYPE) - 1)) != 0)) {                    \
            return mmu_read_unaligned<TYPE>(core, va, data);                             \
        }                                                                                \
    }                                                                                    \
    auto [status, pa] =                                                                  \
        derived()->template TranslateAddress<CDataHelper>(core, va,                      \
            VM_PAGE_RIGHT_READ, VM_PAGE_RIGHT_NONE);                                     \
    CHECK_STATUS(status);                                                                \
    status = derived()->DerivedT::bus_read_##TYPE(pa, data);                             \
    LOG_DEBUG(LOG_LOAD) << "value: " << fmt::data_logger(&data, sizeof(data));           \
    return status;                                                                       \
}

MMU_READ_TYPED_HANDLER(u8)
MMU_READ_TYPED_HANDLER(u16)
MMU_READ_TYPED_HANDLER(u32)
MMU_READ_TYPED_HANDLER(u64)
MMU_READ_TYPED_HANDLER(u128)

template<class DerivedT>
template<class TYPE>
CStatus
IManagedAddressSpace<DerivedT>::mmu_write_unaligned(Core& core, const CVirtAddress va, TYPE data)
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

    auto [status, pa] =
        derived()->template TranslateAddress<CDataHelper>(core, va,
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);
    CHECK_STATUS(status);

    status = derived()->DerivedT::bus_write(pa, sizeof(TYPE) - rest, u.array);
    CHECK_STATUS(status);

    CVirtAddress va_next(va.get_address() + sizeof(TYPE) - rest);
    auto [status_next, pa_next] =
        derived()->template TranslateAddress<CDataHelper>(core, va_next,
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);
    CHECK_STATUS(status_next);

    status = derived()->DerivedT::bus_write(pa_next, rest, &u.array[sizeof(TYPE) - rest]);
    return status;
}

#define MMU_WRITE_TYPED_HANDLER(TYPE)                                                    \
template<class DerivedT>                                                                 \
CStatus IManagedAddressSpace<DerivedT>::                                                 \
mmu_write_##TYPE(Core& core, const CVirtAddress va, TYPE data)                           \
{                                                                                        \
    LOG_DEBUG(LOG_STORE) << "write_" #TYPE ": "                                          \
        << get_decoder().get_address_info(va.get_address());                             \
                                                                                         \
    if constexpr (sizeof(TYPE) > 1) {                                                    \
        if (unlikely((va.get_address() & (sizeof(TYPE) - 1)) != 0)) {                    \
            return mmu_write_unaligned<TYPE>(core, va, data);                            \
        }                                                                                \
    }                                                                                    \
    auto [status, pa] =                                                                  \
        derived()->template TranslateAddress<CDataHelper>(core, va,                      \
            VM_PAGE_RIGHT_WRITE, VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE);   \
    CHECK_STATUS(status);                                                                \
    status = derived()->DerivedT::bus_write_##TYPE(pa, data);                            \
    LOG_DEBUG(LOG_STORE) << "value: " << fmt::data_logger(&data, sizeof(data));          \
    return status;                                                                       \
}

MMU_WRITE_TYPED_HANDLER(u8)
MMU_WRITE_TYPED_HANDLER(u16)
MMU_WRITE_TYPED_HANDLER(u32)
MMU_WRITE_TYPED_HANDLER(u64)
MMU_WRITE_TYPED_HANDLER(u128)

#define MMU_READ_FINALIZED_TYPED_HANDLER(TYPE)                                           \
template<class DerivedT>                                                                 \
CStatus                                                                                  \
IManagedAddressSpace<DerivedT>::mmu_read_##TYPE##_finalized(Core& core,                  \
    const CVirtAddress va, TYPE& data) const                                             \
{                                                                                        \
    LOG_DEBUG(LOG_LOAD) << "read_" #TYPE "_finalized: "                                  \
        << get_decoder().get_address_info(va.get_address());                             \
    CHECK_ALIGNMENT(CDataHelper, va, sizeof(TYPE));                                      \
    auto [status, pa] =                                                                  \
        derived()->template TranslateAddress<CDataHelper>(core, va,                      \
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL,                                    \
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);  \
    CHECK_STATUS(status);                                                                \
    status = derived()->DerivedT::bus_read_##TYPE(pa, data);                             \
    LOG_DEBUG(LOG_LOAD) << "value: " << fmt::data_logger(&data, sizeof(data));           \
    return status;                                                                       \
}

MMU_READ_FINALIZED_TYPED_HANDLER(u32)
MMU_READ_FINALIZED_TYPED_HANDLER(u64)

template<class DerivedT>
CStatus
IManagedAddressSpace<DerivedT>::mmu_read_backing_store(Core& core,
    const CVirtAddress va, size_t len, void *data) const
{
    LOG_DEBUG(LOG_LOAD)
        << "read backstore: len=" << len
        << " addr=" << va;

    CHECK_ALIGNMENT_VAR(CBackstoreHelper, va, len);

    auto [status, pa] =
        derived()->template TranslateAddress<CBackstoreHelper>(core, va,
            VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE); // usual read is allowed

    CHECK_STATUS(status);
    status = derived()->DerivedT::bus_read(pa, len, data);
    return status;
}

template<class DerivedT>
CStatus
IManagedAddressSpace<DerivedT>::mmu_write_backing_store(Core& core,
    const CVirtAddress va, size_t len, const void *data)
{
    LOG_DEBUG(LOG_STORE)
        << "write backstore: len=" << len
        << " addr=" << va;
    CHECK_ALIGNMENT_VAR(CBackstoreHelper, va, len);

    // VM_PAGE_RIGHT_READ is allowed for backtrace reconstruction
    // so not listed in `unexpected_rights`
    auto [status, pa] =
        derived()->template TranslateAddress<CBackstoreHelper>(core, va,
            VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE);

    CHECK_STATUS(status);
    status = derived()->DerivedT::bus_write(pa, len, data);
    return status;
}

template<class DerivedT>
std::tuple<CStatus, CPhysAddress>
IManagedAddressSpace<DerivedT>::mmu_translate(Core& core, const CVirtAddress va)
{
    return derived()->template TranslateAddress<CDataHelper>(core, va, 0, 0);
}

template<class DerivedT>
CStatus
IManagedAddressSpace<DerivedT>::mmu_complex_operation(Core& core,
    const CVirtAddress va, size_t len, u64 requested_rights,
    u64 unexpected_rights, IMemoryOperation& op)
{
    LOG_DEBUG(LOG_STORE)
        << "complex_operation: len=" << len
        << " addr=" << get_decoder().get_address_info(va.get_address());

    CHECK_ALIGNMENT_VAR(CDataHelper, va, len);

    auto [status, pa] =
        derived()->template TranslateAddress<CDataHelper>(core, va,
            requested_rights,
            unexpected_rights);

    CHECK_STATUS(status);
    status = derived()->DerivedT::bus_complex_operation(pa, len, op);
    return status;
}

} // namespace postrisc
