#include "PhysMemoryMapping.hpp"
#include "util/common.hpp"

namespace postrisc {

PhysMemoryMapping::~PhysMemoryMapping()
{
}

void
PhysMemoryMapping::dump(const std::byte *base, size_t device_size, const DumpFormatter& out) const
{
    u64 address = get_address();
    u64 size    = get_size();
    u64 offset  = 0;

    const size_t page_size = m_page_size;

    for (u64 page_address = address, iii = 0; page_address < address + size && offset < device_size; ) {
        if (page_address == address || page_address % page_size == 0) {
            dump_page_flags flags = dump_page_mmio;
            if (!m_page_bitmap.empty()) {
                flags |= dump_page_memory;
                if (m_page_bitmap.at(iii / page_size)) {
                    flags |= dump_page_allocated;
                }
            }
            out.start_page(page_address, flags);
        }
        const size_t page_rest = page_size - page_address % page_size;
        const size_t nbytes = std::min(device_size - offset, page_rest);
        out.dump_page(page_address, base + offset, nbytes);
        offset += page_rest;
        page_address += page_rest;
        iii += page_rest;
        out.finish_page(page_address);
    }
}

} // namespace postrisc
