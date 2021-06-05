#include "util/common.hpp"
#include "formatter_txt.hpp"

namespace postrisc {

void DumpFormatterTxt::start_dump(void) const
{
}

void DumpFormatterTxt::finish_dump(void) const
{
}

void DumpFormatterTxt::chapter(char const *name) const
{
    out << name << fmt::endl;
}

void DumpFormatterTxt::dump_byte(u8 byte) const
{
    if (std::isprint(byte)) {
        out << static_cast<char>(byte);
    } else {
        out << '.';
    }
}

void DumpFormatterTxt::dump_physical_address(u64 address) const
{
    out << fmt::hex(address) << fmt::endl;
}

void DumpFormatterTxt::dump_physical_address_ref(u64 address) const
{
    out << fmt::hex(address);
}

void DumpFormatterTxt::dump_virtual_address(u64 address) const
{
    out << fmt::hex(address);
}

void DumpFormatterTxt::start_device(const std::string& name) const
{
    out << name << fmt::endl;
}

void DumpFormatterTxt::finish_device(const std::string& UNUSED(name)) const
{
    out << fmt::endl;
}

void DumpFormatterTxt::start_page(u64 UNUSED(address), dump_page_flags UNUSED(flags)) const
{
    out << fmt::endl;
}

void DumpFormatterTxt::finish_page(u64 UNUSED(address)) const
{
    out << fmt::endl;
}

void DumpFormatterTxt::start_section(void) const
{
}

void DumpFormatterTxt::finish_section(void) const
{
}


} // namespace postrisc
