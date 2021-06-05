#pragma once

#include <atomic>
#include <future>

#include "emulator/ManagedAddressSpace.hpp"
#include "emulator/TranslationUnit.hpp"
#include "emulator/Device.hpp"
#include "emulator/Core.hpp"
#include "DeviceVideo.hpp"
#include "config/configuration.hpp"
#include "disassembler/Decoder.hpp"

namespace postrisc {

class DeviceArray;
class TranslationUnit;
class ExeFileHeader;

using phys_memory_map = std::map<u64, PhysMemoryMapping, std::greater<u64>>;

class TargetSystem final : public ManagedAddressSpace<TargetSystem> {
friend class boost::serialization::access;
friend class emulator_test_fixture; // security hole for unittests

public:
    TargetSystem(Decoder& decoder);
    ~TargetSystem(void) override;

    u64 size(void) const;

    STANDARD_ADDRESS_SPACE_INTERFACE
    // ovverride mmu_fetch_code for using core i-cache
    Result mmu_fetch_code(Core &core, const VirtAddress va, size_t len, void *data) override;
    void dump_mapping(const DumpFormatter& out) const;
    void dump_statistic(const DumpFormatter& out) const;
    Core *get_bootstrap_core(void) const;
    void FlushTranslationBuffer(void);
    void FlushTranslationBuffer(u64 address);
    void activate_execution(void);
    Result broadcast_interrupt(Result vector);
    void RunMemoryTests(std::ostream& os);
    Result dump_virt_map(const TranslationUnit &mmu, const DumpFormatter& out);
    Result add_memory_mapping(const PhysAddress pa, PhysMemoryMapping& range, bool is_memory);
    Result StartEmulation(const Configuration& configuration, std::istream& input, std::ostream& out);
    void WaitFinishEmulation(std::ostream& out);
    void Clear();
    Result Setup(const Configuration& configuration);
    PhysAddress alloc_page(void);
    Result free_page(PhysAddress phys_address, size_t pagesize);
    void Interactive(std::ostream& out);
    void JoinAllCores(void);
    void activate_execution(Core *core);
    Result activate_execution_handler(Core *core);
    void dump(const TranslationUnit &mmu, std::ostream& out);
    boost::intrusive_ptr<DeviceVideo> GetVideoDevice(void) { return m_video; }
    const TranslationUnit& FindMMU(void) const { return get_bootstrap_core()->GetMMU(); }
    template<typename helper>
    std::tuple<Result, PhysAddress> TranslateAddress(Core &core,
        const VirtAddress va, u64 requested_rights, u64 unexpected_rights) const;

private:
    template<typename Archive> void serialize(Archive& archive, const unsigned int version);

private:
    Result InsertPage(const TranslationUnit &mmu, u64 virt_address, const PhysAddress pa, u64 attributes);
    Result VirtualMap(const TranslationUnit &mmu, const VirtAddress va_base, const PhysAddress pa, u64 size, u64 attributes);
    Result VirtualAllocate(const TranslationUnit &mmu, u64 base, u64 size, u64 attributes);
    Result VirtualProtect(const TranslationUnit &mmu, u64 base, u64 size, u64 attributes);
    Result VirtualFree(const TranslationUnit &mmu, u64 base, u64 size);
    Result LoadSegment(Core& core, u64 base, size_t size, const u8 *data, u64 final_rights);
    Result LoadModule(Core& core, const u64 vbase, std::istream& input, ExeFileHeader& header);
    Result PrepareThread(Core& core, const u64 module_base, const ExeFileHeader& header);
    Result dump_virt_map_sub_dir(const TranslationUnit &mmu, const DumpFormatter& out, const PhysAddress pta, u64 base, u64& used, u64& extra_used, int level);

private:
    boost::intrusive_ptr<DeviceArray>      m_DeviceArray;
    boost::intrusive_ptr<DeviceVideo>      m_video;
    phys_memory_map                        m_phys_memory_map;
    // non-serializable
    std::atomic<int>                       m_CoreNumber;
    std::vector<std::future<Result>>      m_Tasks;
    unsigned                               m_configuration_flags;
    unsigned                               m_bits_per_physical_address;
    size_t                                 m_NumAllocatedPages;
};

template<typename Archive>
void
TargetSystem::serialize(Archive& ar, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_NVP(m_NumAllocatedPages);
}

template<typename helper>
ALWAYS_INLINE
std::tuple<Result, PhysAddress>
TargetSystem::TranslateAddress(Core &core, const VirtAddress va, u64 requested_rights, u64 unexpected_rights) const
{
    // Whole system translation is based on per-core TLB settings
    return core.TranslateAddress<helper>(*this, va, requested_rights, unexpected_rights);
}

#define FIND_DEVICE(pa, len)                                                   \
    u64 const address = pa.get_address();                                      \
    /* Find the device with the base address */                                \
    /* closest to but less than or equal address */                            \
    auto it = m_phys_memory_map.lower_bound(address);                          \
    /* at least null device should be presented, plus CoreArray */             \
    assert(!m_phys_memory_map.empty());                                        \
    if (unlikely(it == m_phys_memory_map.end())) {                             \
        LOG_ERROR(LOG_EXCEPTION | LOG_PLATFORM)                                \
            << "no device at : " << fmt::hex(address)                          \
            << " size=" << len;                                                \
        return Result::bus_unimplemented_address;                              \
    }                                                                          \
    /* address should be after device base address */                          \
    assert(it->first <= address);                                              \
    if (unlikely(address + len > it->first + it->second.get_size())) {         \
        LOG_ERROR(LOG_EXCEPTION | LOG_PLATFORM)                                \
            << "unimplemented address: " << fmt::hex(address)                  \
            << " size=" << len                                                 \
            << " start=" << fmt::hex(it->first)                                \
            << " size=" << it->second.get_size() << std::endl;                 \
        return Result::bus_unimplemented_address;                              \
    }                                                                          \
    const DeviceOffset device_offset(address - it->first);                     \
    auto& mapping = it->second;


#define SYSTEM_READ_TYPED_HANDLER(TYPE)                                        \
ALWAYS_INLINE Result                                                           \
TargetSystem::bus_read_##TYPE(const PhysAddress pa, TYPE& data) const          \
{                                                                              \
    FIND_DEVICE(pa, sizeof(TYPE))                                              \
    mapping.m_device->m_reads_count++;                                         \
    return mapping.m_device->read_##TYPE(device_offset, data);                 \
}

SYSTEM_READ_TYPED_HANDLER(u8)
SYSTEM_READ_TYPED_HANDLER(u16)
SYSTEM_READ_TYPED_HANDLER(u32)
SYSTEM_READ_TYPED_HANDLER(u64)
SYSTEM_READ_TYPED_HANDLER(u128)

ALWAYS_INLINE Result
TargetSystem::bus_read(const PhysAddress pa, size_t len, void *bytes) const
{
    FIND_DEVICE(pa, len)
    mapping.m_device->m_reads_count++;
    return mapping.m_device->read(device_offset, len, bytes);
}

#define SYSTEM_WRITE_TYPED_HANDLER(TYPE)                                       \
ALWAYS_INLINE Result                                                           \
TargetSystem::bus_write_##TYPE(const PhysAddress pa, TYPE data)                \
{                                                                              \
    FIND_DEVICE(pa, sizeof(TYPE))                                              \
    mapping.m_device->m_writes_count++;                                        \
    return mapping.m_device->write_##TYPE(device_offset, data);                \
}

SYSTEM_WRITE_TYPED_HANDLER(u8)
SYSTEM_WRITE_TYPED_HANDLER(u16)
SYSTEM_WRITE_TYPED_HANDLER(u32)
SYSTEM_WRITE_TYPED_HANDLER(u64)
SYSTEM_WRITE_TYPED_HANDLER(u128)

ALWAYS_INLINE Result
TargetSystem::bus_write(const PhysAddress pa, size_t len, const void *bytes)
{
    FIND_DEVICE(pa, len)
    mapping.m_device->m_writes_count++;
    return mapping.m_device->write(device_offset, len, bytes);
}

ALWAYS_INLINE Result
TargetSystem::bus_complex_operation(const PhysAddress pa, size_t len, MemoryOperation& op)
{
    FIND_DEVICE(pa, len)
    mapping.m_device->m_complex_ops_count++;
    return mapping.m_device->complex_operation(device_offset, len, op);
}

} // namespace postrisc
