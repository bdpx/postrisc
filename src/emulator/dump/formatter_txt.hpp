#pragma once

#include "formatter.hpp"

namespace postrisc {

class DumpFormatterTxt : public DumpFormatter {
public:
    DumpFormatterTxt(const Decoder& decoder_, std::ostream& out_)
        : DumpFormatter(decoder_, out_) {}
    ~DumpFormatterTxt(void) override {}
    void chapter(char const *) const override;
    void start_dump(void) const override;
    void finish_dump(void) const override;
    void dump_byte(u8 byte) const override;
    void dump_physical_address_ref(u64 address) const override;
    void dump_physical_address(u64 address) const override;
    void dump_virtual_address(u64 address) const override;
    void start_device(const std::string& name) const override;
    void finish_device(const std::string& name) const override;
    void start_page(u64 address, dump_page_flags flags) const override;
    void finish_page(u64 address) const override;
    void start_section(void) const override;
    void finish_section(void) const override;
};

} // namespace postrisc
