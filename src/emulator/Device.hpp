#pragma once

#include <map>
#include <functional>  // std::greater

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "Result.hpp"
#include "DumpFormatter.hpp"
#include "AddressSpace.hpp"   // for MemoryOperation

namespace postrisc {

/***********************************************************************
* The device is modelled as continuous range of internal addresses.
* The mapping several parts of device is supported.
* The whole range may be mapped to several different physical addresses.
* Each part may be mapped to different physical address,
* but not overlapped with other devices and/or other parts of the same device.
* DeviceOffset represents device-internal offsets after mapping resolution,
* started from device begin.
*
* VirtAddress ==MMU==> PhysAddress ==chipset==> Device+DeviceOffset
*
************************************************************************/
class DeviceOffset {
public:
    explicit DeviceOffset(u64 offset) : m_offset(offset) {}
    friend std::ostream& operator<<(std::ostream& out, const DeviceOffset& r) {
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
public:
    ConfigSpaceHeader(u16 device, u32 size);
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);
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

template<typename Archive>
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
public:
    Device(const std::string& name);
    virtual ~Device(void) = 0;
    // normal read, side effects are possible
    virtual Result read_u8(const DeviceOffset address, u8& value) const = 0;
    virtual Result read_u16(const DeviceOffset address, u16& value) const = 0;
    virtual Result read_u32(const DeviceOffset address, u32& value) const = 0;
    virtual Result read_u64(const DeviceOffset address, u64& value) const = 0;
    virtual Result read_u128(const DeviceOffset address, u128& value) const = 0;
    virtual Result read(const DeviceOffset address, size_t len, void *bytes) const = 0;
    // normal write
    virtual Result write_u8(const DeviceOffset address, u8 value) = 0;
    virtual Result write_u16(const DeviceOffset address, u16 value) = 0;
    virtual Result write_u32(const DeviceOffset address, u32 value) = 0;
    virtual Result write_u64(const DeviceOffset address, u64 value) = 0;
    virtual Result write_u128(const DeviceOffset address, u128 value) = 0;
    virtual Result write(const DeviceOffset address, size_t len, const void *bytes) = 0;
    // page fill, atomic RMO/CAS, etc
    virtual Result complex_operation(const DeviceOffset address, size_t len, MemoryOperation& op) = 0;
    // read from device config space
    virtual Result read_config(const DeviceOffset address, size_t len, void *bytes) const = 0;
    // write to device config space
    virtual Result write_config(const DeviceOffset address, size_t len, const void *bytes) = 0;
    // size of device internal address space
    virtual u64 size(void) const = 0;
    // size of device config space
    virtual u64 config_size(void) const = 0;
    // dump info about the device partial memory mapping,
    virtual void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const = 0;
    // dump the device config space
    virtual void dump_config(const PhysAddress pa, const DumpFormatter& out) const = 0;
    // dump the device statistic (not memory/config mapped)
    virtual void dump_statistic(const DumpFormatter& out) const = 0;
    // return non-null only if device is the processor core
    virtual Core *get_bootstrap_core(void);

    Device(const Device&) = delete;
    Device& operator= (const Device&) = delete;

    const std::string& get_name(void) const { return m_name; }

    static const size_t minimal_size_alignment = 512;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

    void read() const { m_reads_count++; }
    void config_read() const { m_config_reads_count++; }
    void write() { m_writes_count++; }
    void config_write() { m_config_writes_count++; }
    void complex_op() { m_complex_ops_count++; }

protected:
    static void dump_config_raw(const PhysAddress pa, const std::byte *config, size_t config_size, const DumpFormatter& out);
    Result read_config_raw(const DeviceOffset address, void const *config, size_t config_size, size_t len, void *bytes) const;
    Result write_config_raw(const DeviceOffset address, void *config, size_t config_size, size_t len, void const *bytes);

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
#define STANDARD_DEVICE_INTERFACE                                                                   \
    Result read_u8(const DeviceOffset address, u8& value) const override;                           \
    Result read_u16(const DeviceOffset address, u16& value) const override;                         \
    Result read_u32(const DeviceOffset address, u32& value) const override;                         \
    Result read_u64(const DeviceOffset address, u64& value) const override;                         \
    Result read_u128(const DeviceOffset address, u128& value) const override;                       \
    Result read(const DeviceOffset address, size_t len, void *bytes) const override;                \
    Result write_u8(const DeviceOffset address, u8 value) override;                                 \
    Result write_u16(const DeviceOffset address, u16 value) override;                               \
    Result write_u32(const DeviceOffset address, u32 value) override;                               \
    Result write_u64(const DeviceOffset address, u64 value) override;                               \
    Result write_u128(const DeviceOffset address, u128 value) override;                             \
    Result write(const DeviceOffset address, size_t len, const void* bytes) override;               \
    Result read_config(const DeviceOffset addr, size_t len, void *bytes) const override;            \
    Result write_config(const DeviceOffset addr, size_t len, const void *bytes) override;           \
    Result complex_operation(const DeviceOffset addr, size_t len, MemoryOperation& op) override;    \
    u64 size(void) const override;                                                                  \
    u64 config_size(void) const override;                                                           \
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;   \
    void dump_config(const PhysAddress pa, const DumpFormatter& out) const override;                \
    void dump_statistic(const DumpFormatter& out) const override;                                   \

/***************************************************************************
* Device templates/inlines
***************************************************************************/
template<typename Archive>
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

} // namespace postrisc
