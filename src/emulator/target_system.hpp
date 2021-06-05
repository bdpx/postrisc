#pragma once

#include <atomic>
#include <future>

#include "devices/device.hpp"
#include "configuration.hpp"
#include "disassembler/decoder.hpp"

namespace postrisc {

class DeviceArray;
class TranslationUnit;
class CExeFileHeader;

using phys_address_t = uint64_t;
using phys_memory_map = std::map<phys_address_t, PhysMemoryMapping, std::greater<phys_address_t>>;

class TargetSystem {
friend class boost::serialization::access;
friend class emulator_test_fixture; // security hole for unittests

public:
    TargetSystem(const Decoder& decoder);
    ~TargetSystem(void);

    uint64_t size(void) const;
    CStatus read(phys_address_t address, size_t len, void* bytes) const;
    CStatus write(phys_address_t address, size_t len, const void* bytes) const;
    CStatus complex_operation(phys_address_t address, IMemoryOperation& op) const;
    void dump_mapping(const DumpFormatter& out) const;
    void dump_statistic(const DumpFormatter& out) const;
    Core *get_bootstrap_core(void) const;
    void activate_execution(void);
    CStatus post_interrupt(CStatus vector);

    CStatus add_memory_mapping(phys_address_t address, PhysMemoryMapping& range, bool is_memory);

    CStatus StartEmulation(EmulatorConfiguration const & configuration, std::istream & input, std::ostream & out);
    void WaitFinishEmulation(std::ostream & out);
    void Clear();
    CStatus Setup(EmulatorConfiguration const & configuration);

    phys_address_t alloc_page(void);
    CStatus free_page(phys_address_t phys_address, size_t pagesize);

    void Interactive(std::ostream & out);
    void JoinAllCores(void);

    void activate_execution(Core *core);
    CStatus activate_execution_handler(Core * core);
    const Decoder& GetDecoder(void) const { return m_Decoder; }
    unsigned get_page_size(void) const { return m_page_size; }
    void dump(std::ostream& out) const;

private:
    template<class Archive> void serialize(Archive & archive, const unsigned int version);

private:
    boost::intrusive_ptr<DeviceArray>      m_DeviceArray;
    phys_memory_map                        m_phys_memory_map;
    // non-serializable
    std::atomic<int>                       m_CoreNumber;
    std::vector<std::future<CStatus>>      m_Tasks;
    unsigned                               m_configuration_flags;
    unsigned                               m_bits_per_physical_address;
    unsigned                               m_page_size;
    size_t                                 m_NumAllocatedPages;
    const Decoder&                         m_Decoder;
};

template<class Archive>
void
TargetSystem::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_NVP(m_NumAllocatedPages);
}

} // namespace postrisc
