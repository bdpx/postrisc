#pragma once

#include "util/common.hpp"
#include "disassembler/Decoder.hpp"

namespace postrisc {

/***************************************************************************
* flags for disassembling mods
***************************************************************************/
enum dump_page_flags {
    dump_page_mmio       = 0,
    dump_page_memory     = 1,
    dump_page_allocated  = 2,
};
DEFINE_ENUM_FLAG_OPERATOR_SET(dump_page_flags)

class DumpFormatter {
public:
    DumpFormatter(const Decoder& decoder_, std::ostream& out_, bool html = true) : m_html(html), decoder(decoder_), out(out_) {}

    void chapter(char const *title) const;
    void start_dump(void) const;
    void finish_dump(void) const;
    void dump_byte(u8 byte) const;
    void dump_utf8(unsigned codepoint) const;
    void dump_physical_address_ref(u64 address) const;
    void dump_physical_address(u64 address) const;
    void dump_virtual_address(u64 address) const;
    void start_device(const std::string& name) const;
    void finish_device(const std::string& name) const;
    void start_page(u64 address, dump_page_flags flags) const;
    void finish_page(u64 address) const;
    void start_section(void) const;
    void finish_section(void) const;

    bool is_html(void) const { return m_html; }

    template <typename T>
    const DumpFormatter& operator<<(const T &x) const {
        out << x;
        return *this;
    }
    template <typename T>
    const DumpFormatter& operator<<(const T&& x) const {
        out << x;
        return *this;
    }
    // manipulators like std::endl
    const DumpFormatter& operator<<(std::ostream& (*manip)(std::ostream&)) const
    {
        out << manip;
        return *this;
    }

    const Decoder& get_decoder(void) const { return decoder; }
    std::ostream& get_stream(void) const { return out; }

protected:
    bool m_html = false;
    const Decoder&  decoder;
    std::ostream&   out;
};

} // namespace postrisc
