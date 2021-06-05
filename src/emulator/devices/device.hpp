#pragma once

#include <map>
#include <functional>  // std::greater

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "arch/interruption.hpp"
#include "emulator/dump/formatter.hpp"

namespace postrisc {

enum {
    bits_per_absolute_address = 60,
    DEVICE_CONFIG_SPACE_SIZE = 4 * 1024,
};

using device_address_t = uint64_t;

const uint64_t PHYS_ADDRESS_MASK = (UINT64_C(1) << bits_per_absolute_address) - 1;
const uint64_t INVALID_PHYS_ADDRESS = ~ UINT64_C(0);

class TargetSystem;
class Core;
class PhysMemoryMapping;

class IMemoryOperation {
public:
    virtual ~IMemoryOperation() {}
    virtual size_t size(void) const = 0;
    virtual CStatus complex_operation(void * address) = 0;
};

/***********************************************************************
* the config space of any device should begin form standard header
* should be directly included in the final config space
* to be standard layout
************************************************************************/
template<typename T, size_t N>
using space_filler = std::array<util::default_initializer<T, ~T(0)>, N>;

class ConfigSpaceHeader {
friend class boost::serialization::access;
public:
    ConfigSpaceHeader(uint16_t device, uint32_t size);
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    uint16_t                   vendor_id;
    uint16_t                   device_id;
    uint16_t                   command;
    uint16_t                   status;
    uint32_t                   subclass;
    uint32_t                   bist;
    uint64_t                   bar0;
    uint64_t                   bar1;
    uint64_t                   bar2;
    uint32_t                   cardbus;
    uint32_t                   subsystem;
    uint32_t                   rom_base;
    uint32_t                   cap_pointer;
    uint32_t                   size;
    uint32_t                   pad;
    space_filler<uint64_t, 8>  reserved;
};

template<class Archive>
void ConfigSpaceHeader::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(vendor_id);
    ar & BOOST_SERIALIZATION_NVP(device_id);
}

STATIC_ASSERT(sizeof(ConfigSpaceHeader) == 128);
STATIC_ASSERT(std::is_standard_layout<ConfigSpaceHeader>::value);

/***********************************************************************
* parent class for all device types
************************************************************************/
class Device : public boost::intrusive_ref_counter<Device, boost::thread_safe_counter> {
friend class TargetSystem; // for access to reads/writes/etc counters
public:
    Device(TargetSystem * parent_, const std::string& name);
    virtual ~Device(void) = 0;
    // normal read, side effects are possible
    virtual CStatus read(device_address_t address, size_t len, void *bytes) = 0;
    // normal write
    virtual CStatus write(device_address_t address, size_t len, const void *bytes) = 0;
    // page fill, atomic RMO/CAS, etc
    virtual CStatus complex_operation(device_address_t address, IMemoryOperation& op);
    // read from device config space
    virtual CStatus read_config(device_address_t address, size_t len, void *bytes) = 0;
    // write to device config space
    virtual CStatus write_config(device_address_t address, size_t len, const void *bytes) = 0;
    // size of device internal address space
    virtual uint64_t size(void) const = 0;
    // size of device config space
    virtual uint64_t get_config_size(void) const = 0;
    // dump info about the device partial memory mapping,
    virtual void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const = 0;
    // dump the device config space
    virtual void dump_config(uint64_t address, const DumpFormatter& out) const = 0;
    // dump the device statistic (not memory/config mapped)
    virtual void dump_statistic(const DumpFormatter& out) const = 0;
    // return non-null only if device is the processor core
    virtual Core *get_bootstrap_core(void);

    Device(const Device&) = delete;
    Device& operator= (const Device&) = delete;

    const std::string& get_name(void) const { return m_name; }
    TargetSystem * get_target_system(void) const { return m_target_system; }

public:
    static const size_t minimal_size_alignment = 512;
    static void dump_page(uint64_t address, uint8_t const * base, size_t page_size, const DumpFormatter& out);

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

protected:
    static void dump_config_raw(device_address_t address, uint8_t const *config, size_t config_size, const DumpFormatter& out);
    CStatus read_config_raw(device_address_t address, void const *config, size_t config_size, size_t len, void *bytes);
    CStatus write_config_raw(device_address_t address, void *config, size_t config_size, size_t len, void const *bytes);

private:
    uint64_t                m_reads_count;
    uint64_t                m_writes_count;
    uint64_t                m_complex_ops_count;
    uint64_t                m_config_reads_count;
    uint64_t                m_config_writes_count;
    std::string             m_name;
    TargetSystem*           m_target_system;
};

template<class Archive>
void Device::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_reads_count);
    ar & BOOST_SERIALIZATION_NVP(m_writes_count);
    ar & BOOST_SERIALIZATION_NVP(m_complex_ops_count);
    ar & BOOST_SERIALIZATION_NVP(m_config_reads_count);
    ar & BOOST_SERIALIZATION_NVP(m_config_writes_count);
    ar & BOOST_SERIALIZATION_NVP(m_name);
}

/***********************************************************************
* physical memory map is divided into subranges,
* which individually mapped to internal device address ranges.
* each range of physical memory map is linked with only one device,
* but may map only part of internal device address range.
* for example, device with internal size 1000 bytes may be mapped via
* 2 chunks: 400 and 600 bytes,
* or even mapped parttially with 3 chunks: 300, 200, 400 bytes.
* better if chunks are not overlapped but this isn't required.
************************************************************************/
class PhysMemoryMapping {
friend class TargetSystem;
public:
    PhysMemoryMapping(uint64_t address, uint64_t size, uint64_t offset, boost::intrusive_ptr<Device> device)
        : m_address(address), m_size(size), m_offset(offset), m_device(device) {}
    // simple variant with monolith one-to-one mapping
    PhysMemoryMapping(uint64_t address, boost::intrusive_ptr<Device> device)
        : m_address(address), m_size(device->size()), m_offset(0), m_device(device) {}
    ~PhysMemoryMapping() {}

    uint64_t get_address(void) const { return m_address; }
    uint64_t get_size(void) const { return m_size; }
    uint64_t get_offset(void) const { return m_offset; }
    boost::intrusive_ptr<Device> get_device(void) const { return m_device; }

    bool is_memory(void) const { return m_page_bitmap.size() > 0; }

    const std::vector<bool>& get_page_bitmap(void) const { return m_page_bitmap; }
    void dump(uint8_t const *base, size_t device_size, const DumpFormatter& out) const;

private:
    uint64_t                      m_address;
    uint64_t                      m_size;
    uint64_t                      m_offset;
    boost::intrusive_ptr<Device>  m_device;
    std::vector<bool>             m_page_bitmap;
};

} // namespace postrisc
