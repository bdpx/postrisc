#pragma once

#include <atomic>
#include <future>

#include "emulator/IManagedAddressSpace.hpp"
#include "emulator/translation_unit.hpp"
#include "emulator/device.hpp"
#include "emulator/core.hpp"
#include "devices/video.hpp"
#include "config/emulator_configuration.hpp"
#include "disassembler/decoder.hpp"

namespace postrisc {

class DeviceArray;
class TranslationUnit;
class CExeFileHeader;

using phys_memory_map = std::map<u64, PhysMemoryMapping, std::greater<u64>>;

class TargetSystem final : public IManagedAddressSpace<TargetSystem> {
friend class boost::serialization::access;
friend class emulator_test_fixture; // security hole for unittests

public:
    TargetSystem(Decoder& decoder);
    ~TargetSystem(void) override;

    u64 size(void) const;

    STANDARD_ADDRESS_SPACE_INTERFACE
    // ovverride mmu_fetch_code for using core i-cache
    CStatus mmu_fetch_code(Core &core, const CVirtAddress va, size_t len, void *data) override;

    void dump_mapping(const DumpFormatter& out) const;
    void dump_statistic(const DumpFormatter& out) const;
    Core *get_bootstrap_core(void) const;
    void FlushTranslationBuffer(void);
    void FlushTranslationBuffer(u64 address);
    void activate_execution(void);
    CStatus broadcast_interrupt(CStatus vector);
    void RunMemoryTests(std::ostream & os);
    CStatus dump_virt_map(const TranslationUnit &mmu, const DumpFormatter& out);

    CStatus add_memory_mapping(const CPhysAddress pa, PhysMemoryMapping& range, bool is_memory);

    CStatus StartEmulation(EmulatorConfiguration const & configuration,
                           std::istream & input, std::ostream & out);
    void WaitFinishEmulation(std::ostream & out);
    void Clear();
    CStatus Setup(EmulatorConfiguration const & configuration);

    CPhysAddress alloc_page(void);
    CStatus free_page(CPhysAddress phys_address, size_t pagesize);

    void Interactive(std::ostream & out);
    void JoinAllCores(void);

    void activate_execution(Core *core);
    CStatus activate_execution_handler(Core * core);
    void dump(const TranslationUnit &mmu, std::ostream& out);

    boost::intrusive_ptr<device_video> GetVideoDevice(void) { return m_video; }

    const TranslationUnit& FindMMU(void) const { return get_bootstrap_core()->GetMMU(); }

    template<class helper>
    std::tuple<CStatus, CPhysAddress> TranslateAddress(Core &core,
        const CVirtAddress va, u64 requested_rights, u64 unexpected_rights);

private:
    template<class Archive> void serialize(Archive & archive, const unsigned int version);

private:
    CStatus InsertPage(const TranslationUnit &mmu, u64 virt_address, const CPhysAddress pa, u64 attributes);
    CStatus VirtualMap(const TranslationUnit &mmu, u64 va_base, const CPhysAddress pa, u64 size, u64 attributes);
    CStatus VirtualAllocate(const TranslationUnit &mmu, u64 base,
                            u64 size, u64 attributes);
    CStatus VirtualProtect(const TranslationUnit &mmu, u64 base,
                           u64 size, u64 attributes);
    CStatus VirtualFree(const TranslationUnit &mmu, u64 base, u64 size);
    CStatus LoadSegment(Core& core, u64 base, size_t size,
                        const u8 *data, u64 final_rights);
    CStatus LoadModule(Core& core, const u64 vbase,
                       std::istream & input, CExeFileHeader & header);
    CStatus PrepareThread(Core& core, const u64 module_base,
                          const CExeFileHeader &header);

    CStatus dump_virt_map_sub_dir(const TranslationUnit &mmu, const DumpFormatter& out,
                                  const CPhysAddress pta, u64 base, u64 & used,
                                  u64 & extra_used, int level);

private:
    boost::intrusive_ptr<DeviceArray>      m_DeviceArray;
    boost::intrusive_ptr<device_video>     m_video;
    phys_memory_map                        m_phys_memory_map;
    // non-serializable
    std::atomic<int>                       m_CoreNumber;
    std::vector<std::future<CStatus>>      m_Tasks;
    unsigned                               m_configuration_flags;
    unsigned                               m_bits_per_physical_address;
    size_t                                 m_NumAllocatedPages;
};

template<class Archive>
void
TargetSystem::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_NVP(m_NumAllocatedPages);
}

template<class helper>
ALWAYS_INLINE
std::tuple<CStatus, CPhysAddress>
TargetSystem::TranslateAddress(Core &core, const CVirtAddress va,
    u64 requested_rights, u64 unexpected_rights)
{
    // Whole system translation is based on per-core TLB settings
    return core.TranslateAddress<helper>(*this, va, requested_rights, unexpected_rights);
}

#define FIND_DEVICE(pa, len, OFFSET)                                           \
    u64 const address = pa.get_address();                                      \
    /* Find the device with the base address */                                \
    /* closest to but less than or equal address */                            \
    auto it = m_phys_memory_map.lower_bound(address);                          \
    /* at least null device should be presented, plus CoreArray */             \
    assert(!m_phys_memory_map.empty());                                        \
    if (unlikely(it == m_phys_memory_map.end())) {                             \
        return CStatus::bus_unimplemented_address;                             \
    }                                                                          \
    /* address should be after device base address */                          \
    assert(it->first <= address);                                              \
    if (unlikely(address + len > it->first + it->second.get_size())) {         \
        LOG_ERROR(LOG_EXCEPTION | LOG_PLATFORM)                                \
            << "unimplemented address: " << fmt::hex(address)                  \
            << " size=" << len;                                                \
        return CStatus::bus_unimplemented_address;                             \
    }                                                                          \
    const CDeviceOffset device_offset(address - it->first, OFFSET);            \
    auto& mapping = it->second;


#define SYSTEM_READ_TYPED_HANDLER(TYPE)                                        \
ALWAYS_INLINE CStatus                                                          \
TargetSystem::bus_read_##TYPE(const CPhysAddress pa, TYPE& data)               \
{                                                                              \
    FIND_DEVICE(pa, sizeof(TYPE), it->second.get_offset())                     \
    mapping.m_device->m_reads_count++;                                         \
    return mapping.m_device->read_##TYPE(device_offset, data);                 \
}

SYSTEM_READ_TYPED_HANDLER(u8)
SYSTEM_READ_TYPED_HANDLER(u16)
SYSTEM_READ_TYPED_HANDLER(u32)
SYSTEM_READ_TYPED_HANDLER(u64)
SYSTEM_READ_TYPED_HANDLER(u128)

ALWAYS_INLINE CStatus
TargetSystem::bus_read(const CPhysAddress pa, size_t len, void *bytes)
{
    FIND_DEVICE(pa, len, it->second.get_offset())
    mapping.m_device->m_reads_count++;
    return mapping.m_device->read(device_offset, len, bytes);
}

#define SYSTEM_WRITE_TYPED_HANDLER(TYPE)                                       \
ALWAYS_INLINE CStatus                                                          \
TargetSystem::bus_write_##TYPE(const CPhysAddress pa, TYPE data)               \
{                                                                              \
    FIND_DEVICE(pa, sizeof(TYPE), it->second.get_offset())                     \
    mapping.m_device->m_writes_count++;                                        \
    return mapping.m_device->write_##TYPE(device_offset, data);                \
}

SYSTEM_WRITE_TYPED_HANDLER(u8)
SYSTEM_WRITE_TYPED_HANDLER(u16)
SYSTEM_WRITE_TYPED_HANDLER(u32)
SYSTEM_WRITE_TYPED_HANDLER(u64)
SYSTEM_WRITE_TYPED_HANDLER(u128)

ALWAYS_INLINE CStatus
TargetSystem::bus_write(const CPhysAddress pa, size_t len, const void *bytes)
{
    FIND_DEVICE(pa, len, it->second.get_offset())
    mapping.m_device->m_writes_count++;
    return mapping.m_device->write(device_offset, len, bytes);
}

ALWAYS_INLINE CStatus
TargetSystem::bus_complex_operation(const CPhysAddress pa, size_t len, IMemoryOperation& op)
{
    FIND_DEVICE(pa, len, it->second.get_offset())
    mapping.m_device->m_complex_ops_count++;
    return mapping.m_device->complex_operation(device_offset, len, op);
}

} // namespace postrisc
