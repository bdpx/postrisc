#pragma once

#include <map>
#include <functional>  // std::greater

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "interruption.hpp"
#include "dump/formatter.hpp"
#include "IAddressSpace.hpp"   // for IMemoryOperation

namespace postrisc {

/***********************************************************************
* The device is modelled as continuous range of internal addresses.
* The mapping several parts of device is supported.
* The whole range may be mapped to several different physical addresses.
* Each part may be mapped to different physical address,
* but not overlapped with other devices and/or other parts of the same device.
* CDeviceOffset represents device-internal offsets after mapping resolution,
* started from device begin.
*
* virt_addr ==MMU==> phys_addr ==chipset==> device_offset
*
************************************************************************/
class CDeviceOffset {
public:
    explicit CDeviceOffset(u64 offset) : m_offset(offset) {}
    friend std::ostream& operator<<(std::ostream& out, const CDeviceOffset& r) {
        return out << fmt::hex(r.m_offset);
    }
    u64 get_offset(void) const { return m_offset; }
private:
    u64 m_offset;
};

class Core;
class PhysMemoryMapping;

/***********************************************************************
* the config space of any device should begin form standard header
* should be directly included in the final config space
* to be standard layout
************************************************************************/
enum {
    DEVICE_CONFIG_SPACE_SIZE = 4 * 1024,
};

template<typename T, size_t N>
using space_filler = std::array<util::default_initializer<T, ~T(0)>, N>;

class ConfigSpaceHeader {
friend class boost::serialization::access;
public:
    ConfigSpaceHeader(u16 device, u32 size);
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    u16                   vendor_id = U16C(404);
    u16                   device_id;
    u16                   command = 0;
    u16                   status = 0;
    u32                   subclass = 0;
    u32                   bist = 0;
    u64                   bar0 = ~U64C(0);
    u64                   bar1 = ~U64C(0);
    u64                   bar2 = ~U64C(0);
    u32                   cardbus = 0;
    u32                   subsystem = 0;
    u32                   rom_base = 0xffffffff;
    u32                   cap_pointer = 0;
    u32                   size;
    u32                   pad = 0;
    space_filler<u64, 8>  reserved;
};

static_assert(sizeof(ConfigSpaceHeader) == 128);
static_assert(std::is_standard_layout<ConfigSpaceHeader>::value);

template<class Archive>
void
ConfigSpaceHeader::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(vendor_id);
    ar & BOOST_SERIALIZATION_NVP(device_id);
}

/***********************************************************************
* base class for all device types
************************************************************************/
class Device : public boost::intrusive_ref_counter<Device, boost::thread_safe_counter> {
friend class TargetSystem; // for access to reads/writes/etc counters
public:
    Device(const std::string& name);
    virtual ~Device(void) = 0;
    // normal read, side effects are possible
    virtual CStatus read_u8(const CDeviceOffset address, u8& value) const = 0;
    virtual CStatus read_u16(const CDeviceOffset address, u16& value) const = 0;
    virtual CStatus read_u32(const CDeviceOffset address, u32& value) const = 0;
    virtual CStatus read_u64(const CDeviceOffset address, u64& value) const = 0;
    virtual CStatus read_u128(const CDeviceOffset address, u128& value) const = 0;
    virtual CStatus read(const CDeviceOffset address, size_t len, void *bytes) const = 0;
    // normal write
    virtual CStatus write_u8(const CDeviceOffset address, u8 value) = 0;
    virtual CStatus write_u16(const CDeviceOffset address, u16 value) = 0;
    virtual CStatus write_u32(const CDeviceOffset address, u32 value) = 0;
    virtual CStatus write_u64(const CDeviceOffset address, u64 value) = 0;
    virtual CStatus write_u128(const CDeviceOffset address, u128 value) = 0;
    virtual CStatus write(const CDeviceOffset address, size_t len, const void *bytes) = 0;
    // page fill, atomic RMO/CAS, etc
    virtual CStatus complex_operation(const CDeviceOffset address, size_t len, IMemoryOperation& op) = 0;
    // read from device config space
    virtual CStatus read_config(const CDeviceOffset address, size_t len, void *bytes) const = 0;
    // write to device config space
    virtual CStatus write_config(const CDeviceOffset address, size_t len, const void *bytes) = 0;
    // size of device internal address space
    virtual u64 size(void) const = 0;
    // size of device config space
    virtual u64 get_config_size(void) const = 0;
    // dump info about the device partial memory mapping,
    virtual void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const = 0;
    // dump the device config space
    virtual void dump_config(const CPhysAddress pa, const DumpFormatter& out) const = 0;
    // dump the device statistic (not memory/config mapped)
    virtual void dump_statistic(const DumpFormatter& out) const = 0;
    // return non-null only if device is the processor core
    virtual Core *get_bootstrap_core(void);

    Device(const Device&) = delete;
    Device& operator= (const Device&) = delete;

    const std::string& get_name(void) const { return m_name; }

public:
    static const size_t minimal_size_alignment = 512;
    static void dump_page(u64 address, const std::byte *base, size_t page_size, const DumpFormatter& out);

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

protected:
    static void dump_config_raw(const CPhysAddress pa, const std::byte *config, size_t config_size, const DumpFormatter& out);
    CStatus read_config_raw(const CDeviceOffset address, void const *config, size_t config_size, size_t len, void *bytes) const;
    CStatus write_config_raw(const CDeviceOffset address, void *config, size_t config_size, size_t len, void const *bytes);

private:
    mutable u64        m_reads_count = 0;
    mutable u64        m_config_reads_count = 0;
    u64                m_writes_count = 0;
    u64                m_complex_ops_count = 0;
    u64                m_config_writes_count = 0;
    std::string        m_name;
};

/***************************************************************************
* for derived classes
***************************************************************************/
#define STANDARD_DEVICE_INTERFACE                                                                     \
    CStatus read_u8(const CDeviceOffset address, u8& value) const override;                           \
    CStatus read_u16(const CDeviceOffset address, u16& value) const override;                         \
    CStatus read_u32(const CDeviceOffset address, u32& value) const override;                         \
    CStatus read_u64(const CDeviceOffset address, u64& value) const override;                         \
    CStatus read_u128(const CDeviceOffset address, u128& value) const override;                       \
    CStatus read(const CDeviceOffset address, size_t len, void *bytes) const override;                \
    CStatus write_u8(const CDeviceOffset address, u8 value) override;                                 \
    CStatus write_u16(const CDeviceOffset address, u16 value) override;                               \
    CStatus write_u32(const CDeviceOffset address, u32 value) override;                               \
    CStatus write_u64(const CDeviceOffset address, u64 value) override;                               \
    CStatus write_u128(const CDeviceOffset address, u128 value) override;                             \
    CStatus write(const CDeviceOffset address, size_t len, const void* bytes) override;               \
    CStatus read_config(const CDeviceOffset addr, size_t len, void *bytes) const override;            \
    CStatus write_config(const CDeviceOffset addr, size_t len, const void *bytes) override;           \
    CStatus complex_operation(const CDeviceOffset addr, size_t len, IMemoryOperation& op) override;   \
    u64 size(void) const override;                                                                    \
    u64 get_config_size(void) const override;                                                         \
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;     \
    void dump_config(const CPhysAddress pa, const DumpFormatter& out) const override;                 \
    void dump_statistic(const DumpFormatter& out) const override;                                     \

/***************************************************************************
* Device templates/inlines
***************************************************************************/
template<class Archive>
void
Device::serialize(Archive & ar, const unsigned int UNUSED(version))
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
************************************************************************/
class PhysMemoryMapping {
friend class TargetSystem;
public:
    PhysMemoryMapping(CPhysAddress address, size_t page_size, boost::intrusive_ptr<Device> device)
        : m_address(address)
        , m_size(device->size())
        , m_page_size(page_size)
        , m_device(device)
    {}

    ALWAYS_INLINE PhysMemoryMapping(const PhysMemoryMapping&) = default;
    ~PhysMemoryMapping();

    u64 get_address(void) const { return m_address.get_address(); }
    u64 get_size(void) const { return m_size; }
    boost::intrusive_ptr<Device> get_device(void) const { return m_device; }

    bool is_memory(void) const { return m_page_bitmap.size() > 0; }

    const std::vector<bool>& get_page_bitmap(void) const { return m_page_bitmap; }
    void dump(const std::byte *base, size_t device_size, const DumpFormatter& out) const;

private:
    CPhysAddress                  m_address;
    u64                           m_size;
    size_t                        m_page_size;
    boost::intrusive_ptr<Device>  m_device;
    std::vector<bool>             m_page_bitmap;
};

} // namespace postrisc
