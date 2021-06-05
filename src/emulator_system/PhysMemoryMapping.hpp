#pragma once

#include <map>
#include <functional>  // std::greater

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "emulator/Result.hpp"
#include "emulator/Device.hpp"
#include "emulator/DumpFormatter.hpp"
#include "emulator/AddressSpace.hpp"   // for MemoryOperation

namespace postrisc {

/***********************************************************************
* physical memory map is divided into subranges,
* which individually mapped to internal device address ranges.
* each range of physical memory map is linked with only one device,
************************************************************************/
class PhysMemoryMapping {
friend class TargetSystem;
public:
    PhysMemoryMapping(PhysAddress address, size_t page_size, boost::intrusive_ptr<Device> device)
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
    PhysAddress                   m_address;
    u64                           m_size;
    size_t                        m_page_size;
    boost::intrusive_ptr<Device>  m_device;
    std::vector<bool>             m_page_bitmap;
};

} // namespace postrisc
