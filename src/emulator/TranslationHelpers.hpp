#pragma once

#include "PerformanceMonitor.hpp"

namespace postrisc {

/***************************************************************************
* exception helpers: code/data/backstore
***************************************************************************/
class InstructionHelper {
public:
    static Result invalid_address      () { return Result::instruction_invalid_address; }
    static Result unaligned_address    () { return Result::instruction_unaligned_address; }
    static Result unimplemented_address() { return Result::instruction_unimplemented_address; }
    static Result unsupported_reference() { return Result::instruction_unsupported_reference; }
    static Result tlb_miss             () { return Result::instruction_tlb_miss; }
    static Result page_not_present     () { return Result::instruction_page_not_present; }
    static Result access_right         () { return Result::instruction_access_right; }
    static Result dirty_bit            () { return Result::instruction_dirty_bit; }
    static Result access_bit           () { return Result::instruction_access_bit; }
    static Result debug                () { return Result::instruction_debug; }

    static const monitor_event translation_hit  = PMC_INSTRUCTION_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_INSTRUCTION_TRANSLATION_MISS;
};

class DataHelper {
public:
    static Result invalid_address      () { return Result::data_invalid_address; }
    static Result unaligned_address    () { return Result::data_unaligned_address; }
    static Result unimplemented_address() { return Result::data_unimplemented_address; }
    static Result unsupported_reference() { return Result::data_unsupported_reference; }
    static Result tlb_miss             () { return Result::data_tlb_miss; }
    static Result page_not_present     () { return Result::data_page_not_present; }
    static Result access_right         () { return Result::data_access_right; }
    static Result dirty_bit            () { return Result::data_dirty_bit; }
    static Result access_bit           () { return Result::data_access_bit; }
    static Result debug                () { return Result::data_debug; }

    static const monitor_event translation_hit  = PMC_DATA_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_DATA_TRANSLATION_MISS;
};

class BackstoreHelper {
public:
    static Result invalid_address      () { return Result::backstore_invalid_address; }
    static Result unaligned_address    () { return Result::backstore_unaligned_address; }
    static Result unimplemented_address() { return Result::backstore_unimplemented_address; }
    static Result unsupported_reference() { return Result::backstore_unsupported_reference; }
    static Result tlb_miss             () { return Result::backstore_tlb_miss; }
    static Result page_not_present     () { return Result::backstore_page_not_present; }
    static Result access_right         () { return Result::backstore_access_right; }
    static Result dirty_bit            () { return Result::backstore_dirty_bit; }
    static Result access_bit           () { return Result::backstore_access_bit; }
    static Result debug                () { return Result::backstore_debug; }

    static const monitor_event translation_hit  = PMC_BACKSTORE_TRANSLATION_HIT;
    static const monitor_event translation_miss = PMC_BACKSTORE_TRANSLATION_MISS;
};

} // namespace postrisc
