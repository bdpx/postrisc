#pragma once

#include "util/common.hpp"
#include "disassembler/decoder.hpp"

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
    DumpFormatter(const Decoder& decoder_, std::ostream& out_) : decoder(decoder_), out(out_) {}

    virtual ~DumpFormatter(void) {}
    virtual void chapter(char const *) const = 0;
    virtual void start_dump(void) const = 0;
    virtual void finish_dump(void) const = 0;
    virtual void dump_byte(u8 byte) const = 0;
    virtual void dump_physical_address_ref(u64 address) const = 0;
    virtual void dump_physical_address(u64 address) const = 0;
    virtual void dump_virtual_address(u64 address) const = 0;
    virtual void start_device(const std::string& name) const = 0;
    virtual void finish_device(const std::string& name) const = 0;
    virtual void start_page(u64 address, dump_page_flags flags) const = 0;
    virtual void finish_page(u64 address) const = 0;
    virtual void start_section(void) const = 0;
    virtual void finish_section(void) const = 0;

    template <class T>
    const DumpFormatter& operator<< (const T &x) const {
        out << x;
        return *this;
    }
    template <class T>
    const DumpFormatter& operator<< (const T&& x) const {
        out << x;
        return *this;
    }
    // manipulators like std::endl
    const DumpFormatter& operator << (std::ostream& (*manip)(std::ostream&)) const
    {
        out << manip;
        return *this;
    }

    const Decoder& get_decoder(void) const { return decoder; }
    std::ostream& get_stream(void) const { return out; }

protected:
    const Decoder&  decoder;
    std::ostream&   out;
};

} // namespace postrisc
