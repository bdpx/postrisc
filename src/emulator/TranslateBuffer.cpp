#include "util/common.hpp"
#include "arch/isa.hpp"
#include "Result.hpp"
#include "Utils.hpp"
#include "Core.hpp"
#include "TranslateBuffer.hpp"

namespace postrisc {

const unsigned not_found = (unsigned)~0;

/***************************************************************************
* this function insert new tlb data
***************************************************************************/
Result TranslateBuffer::insert_translate_buffer(tlb_ppn_t gpr, u64 ifa)
{
    // check page size
    unsigned insert_ps = 14;
    if (insert_ps > bits_per_maxpageoffset) {
        LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "invalid page size: " << insert_ps;
        return Result::invalid_parameter;
    }

    u64 insert_mask = ~util::makemask<u64>(insert_ps);

    u64 insert_vp = ifa;
    if ((insert_vp & insert_mask) != insert_vp) {
        LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "invalid vpa alignment: " << fmt::hex(insert_vp);
        return Result::invalid_parameter;
    }

    u64 insert_pp = gpr.address & ~util::makemask<u64>(bits_per_minpageoffset);
    if ((insert_pp & insert_mask) != insert_pp) {
        LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "invalid ppa alignment: " << fmt::hex(insert_pp);
        return Result::invalid_parameter;
    }

    if ((insert_pp & util::makemask<u64>(bits_per_phys_address)) != insert_pp) {
        LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "unimplemented ppa bits: " << fmt::hex(insert_pp);
        return Result::invalid_parameter;
    }

    // invalidate all pages that map into new possible superpage
    // search for overlapped pages and clean its from tlb
    size_t cand = not_found;
    for (size_t i=0; i<entry.size(); i++) {

        if (entry[i].ppn.u.v == 0) {
           if (cand == not_found)
              cand = i;
           continue;
        }

        u64 tlb_vpa = entry[i].vpn.get_vpa();
        assert((tlb_vpa & util::makemask<u64>(bits_per_vpn)) == tlb_vpa);

        unsigned tlb_ps = entry[i].vpn.get_page_size();
        assert(tlb_ps >= bits_per_minpageoffset);
        assert(tlb_ps <= bits_per_maxpageoffset);

        u64 tlb_pagemask = ~util::makemask<u64>(tlb_ps);
        assert((tlb_vpa & tlb_pagemask) == tlb_vpa);

        u64 cmp_mask = tlb_pagemask & insert_mask;

        if ((tlb_vpa & cmp_mask) == (insert_vp & cmp_mask)) {
           LOG_DEBUG(LOG_ITLB | LOG_DTLB | LOG_EMULATOR) << "cleanup overlapped tlb entry: " << i;
           entry[i].ppn.u.v = 0;
           entry[i].ppn.u.f = 0;
           if (cand == not_found)
             cand = i;
        }
    }

    // free entries not found, so find some other random entry
    if (cand == not_found) {
        LOG_DEBUG(LOG_ITLB | LOG_DTLB | LOG_EMULATOR) << "find valid non-fixed tlb entry for overwrite.";
        size_t i;
        for (i=insert_head; i<entry.size(); i++) {
            if (entry[i].ppn.u.f == 0)
               break;
        }
        if (i == entry.size()) {
            for (i=0; i<insert_head; i++)
            {
                if (!entry[i].ppn.fixed())
                    break;
            }
        }

        if (i == insert_head) {
            LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "all tlb entries are fixed";
            return Result::illegal_instruction;
        }

        LOG_DEBUG(LOG_ITLB | LOG_DTLB | LOG_EMULATOR) << "cleanup valid tlb entry: " << cand;
        cand = i;
        insert_head = static_cast<unsigned>(i + 1);
    }

    // write new entry
    entry[cand].ppn = gpr;
    entry[cand].vpn.address = ifa;

    return Result::continue_execution;
}


/***************************************************************************
* this function translate virtual addresses for data
***************************************************************************/
Result TranslateBuffer::translate_address(u64 address, unsigned int rights, unsigned int clevel) const
{
    LOG_DEBUG(LOG_ITLB | LOG_DTLB) << "address=" << fmt::hex(address);

    const u64 vpn = address & util::makemask<u64>(bits_per_vpn);

    // search tlb
    for (unsigned tlbnum=0; tlbnum < entry.size(); tlbnum++) {
        // skip nonvalid entries
        if (!entry[tlbnum].ppn.valid())
           continue;

        // variable pagesize
        const unsigned pagesize = entry[tlbnum].vpn.get_page_size();
        const u64 pagemask = ~util::makemask<u64>(pagesize);

        const u64 tlbvpn = entry[tlbnum].vpn.get_vpa();
        assert((tlbvpn & pagemask) == tlbvpn);

        if ((tlbvpn & pagemask) != (vpn & pagemask))
           continue;

        if (entry[tlbnum].ppn.u.p == 0) {
            LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "data page not present: " << fmt::hex(address);
            return Result::data_page_not_present;
        }

        const unsigned page_rights = rights;

        if ((rights & page_rights) != rights) {
            LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "data page permission fault: " << fmt::hex(address);
            return Result::data_access_right;
        }

        // all is right
        u64 paddr = entry[tlbnum].ppn.get_ppa();
        assert((paddr & pagemask) == paddr);
        paddr = paddr | (address & ~pagemask);
        return Result::continue_execution;
    }

    if (clevel > 0) {
        LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "nested tlb fault: " << fmt::hex(address);
        return Result::data_tlb_miss;
    }

    //if (!pta.valid())
    //{
    //    LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "data page miss fault: " << fmt::hex(address);
    //    return Result::data_tlb_miss;
    //}

    // vpt search
    LOG_ERROR(LOG_ITLB | LOG_DTLB | LOG_EXCEPTION) << "vpt tlb alternate miss fault";
    return Result::data_tlb_miss;
}

} // namespace postrisc
