#pragma once

#include "performance_monitor.hpp" // for performance events

namespace postrisc {

/***************************************************************************
* exception helpers: code/data/backstore
***************************************************************************/
class CInstructionHelper {
public:
    static CStatus invalid_address      () { return CStatus::instruction_invalid_address; }
    static CStatus unaligned_address    () { return CStatus::instruction_unaligned_address; }
    static CStatus unimplemented_address() { return CStatus::instruction_unimplemented_address; }
    static CStatus unsupported_reference() { return CStatus::instruction_unsupported_reference; }
    static CStatus tlb_miss             () { return CStatus::instruction_tlb_miss; }
    static CStatus page_not_present     () { return CStatus::instruction_page_not_present; }
    static CStatus access_right         () { return CStatus::instruction_access_right; }
    static CStatus dirty_bit            () { return CStatus::instruction_dirty_bit; }
    static CStatus access_bit           () { return CStatus::instruction_access_bit; }
    static CStatus debug                () { return CStatus::instruction_debug; }

    static const monitor_event translation_hit  = PMC_INSTRUCTION_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_INSTRUCTION_TRANSLATION_MISS;
};

class CDataHelper {
public:
    static CStatus invalid_address      () { return CStatus::data_invalid_address; }
    static CStatus unaligned_address    () { return CStatus::data_unaligned_address; }
    static CStatus unimplemented_address() { return CStatus::data_unimplemented_address; }
    static CStatus unsupported_reference() { return CStatus::data_unsupported_reference; }
    static CStatus tlb_miss             () { return CStatus::data_tlb_miss; }
    static CStatus page_not_present     () { return CStatus::data_page_not_present; }
    static CStatus access_right         () { return CStatus::data_access_right; }
    static CStatus dirty_bit            () { return CStatus::data_dirty_bit; }
    static CStatus access_bit           () { return CStatus::data_access_bit; }
    static CStatus debug                () { return CStatus::data_debug; }

    static const monitor_event translation_hit  = PMC_DATA_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_DATA_TRANSLATION_MISS;
};

class CBackstoreHelper {
public:
    static CStatus invalid_address      () { return CStatus::backstore_invalid_address; }
    static CStatus unaligned_address    () { return CStatus::backstore_unaligned_address; }
    static CStatus unimplemented_address() { return CStatus::backstore_unimplemented_address; }
    static CStatus unsupported_reference() { return CStatus::backstore_unsupported_reference; }
    static CStatus tlb_miss             () { return CStatus::backstore_tlb_miss; }
    static CStatus page_not_present     () { return CStatus::backstore_page_not_present; }
    static CStatus access_right         () { return CStatus::backstore_access_right; }
    static CStatus dirty_bit            () { return CStatus::backstore_dirty_bit; }
    static CStatus access_bit           () { return CStatus::backstore_access_bit; }
    static CStatus debug                () { return CStatus::backstore_debug; }

    static const monitor_event translation_hit  = PMC_BACKSTORE_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_BACKSTORE_TRANSLATION_MISS;
};

} // namespace postrisc
