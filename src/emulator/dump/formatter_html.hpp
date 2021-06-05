#pragma once

#include "formatter.hpp"

namespace postrisc {

class DumpFormatterHtml : public DumpFormatter {
public:
    DumpFormatterHtml(std::ostream& out_)
        : DumpFormatter(out_) {}
    ~DumpFormatterHtml(void) override {}
    void chapter(char const *) const override;
    void start_dump(void) const override;
    void finish_dump(void) const override;
    void dump_byte(uint8_t byte) const override;
    void dump_physical_address_ref(uint64_t address) const override;
    void dump_physical_address(uint64_t address) const override;
    void dump_virtual_address(uint64_t address) const override;
    void start_device(const std::string& name) const override;
    void finish_device(const std::string& name) const override;
    void start_page(uint64_t address, dump_page_flags flags) const override;
    void finish_page(uint64_t address) const override;

private:
    void dump_utf8(unsigned codepoint) const;
};

} // namespace postrisc
