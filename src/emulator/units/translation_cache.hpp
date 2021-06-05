#pragma once

namespace postrisc {

class TranslationCacheEntry {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
public:
    TranslationCacheEntry() : virt_address(invalid_tag), phys_address(0) {}
    void reset() { virt_address = invalid_tag; }

public:
    static const uint64_t invalid_tag = ~UINT64_C(0);

public:
    uint64_t virt_address;
    uint64_t phys_address;
};

template<class Archive>
void
TranslationCacheEntry::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(virt_address);
    ar & BOOST_SERIALIZATION_NVP(phys_address);
}

template<size_t SIZE> class TranslationCache {
    friend class Core;
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
public:
    TranslationCache();

    void Flush();
    void Flush(uint64_t virt_address, unsigned bitsPerPageOffset);

private:
    std::array<TranslationCacheEntry, SIZE>     cache;
};

template<size_t SIZE>
template<class Archive>
void
TranslationCache<SIZE>::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(cache);
}

template<size_t SIZE>
TranslationCache<SIZE>::TranslationCache()
{
    Flush();
}

template<size_t SIZE>
void
TranslationCache<SIZE>::Flush()
{
    for (auto& x : cache) {
        x.reset();
    }
}

template<size_t SIZE>
void
TranslationCache<SIZE>::Flush(uint64_t virt_address, unsigned bitsPerPageOffset)
{
    uint64_t const vpn = virt_address >> bitsPerPageOffset;
    unsigned const idx = static_cast<unsigned>(vpn % cache.size());

    if (cache[idx].virt_address == vpn) {
        LOG_TRACE(LOG_PAGETABLE)
             << "TLB flush found: virt_address=" << fmt::hex(virt_address)
             << " phys_address=" << fmt::hex(cache[idx].phys_address);

        cache[idx].reset();
    }
}

} // namespace postrisc
