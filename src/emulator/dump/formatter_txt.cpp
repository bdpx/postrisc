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

void DumpFormatterTxt::dump_byte(uint8_t byte) const
{
    if (std::isprint(byte)) {
        out << static_cast<char>(byte);
    } else {
        out << '.';
    }
}

void DumpFormatterTxt::dump_physical_address(uint64_t address) const
{
    out << fmt::hex(address) << fmt::endl;
}

void DumpFormatterTxt::dump_physical_address_ref(uint64_t address) const
{
    out << fmt::hex(address);
}

void DumpFormatterTxt::dump_virtual_address(uint64_t address) const
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

void DumpFormatterTxt::start_page(uint64_t UNUSED(address), dump_page_flags UNUSED(flags)) const
{
    out << fmt::endl;
}

void DumpFormatterTxt::finish_page(uint64_t UNUSED(address)) const
{
    out << fmt::endl;
}

} // namespace postrisc
