#include "util/common.hpp"
#include "DumpFormatter.hpp"

namespace postrisc {

#define HIDDEN_DIV_CLASS "hdv"

void DumpFormatter::start_dump(void) const
{
    if (!is_html()) return;

    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
        "<html lang=\"en\" xml:lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n";

    out <<
"<head>\n"
"<title>system dump</title>\n"
"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n"
"<meta http-equiv=\"content-language\" content=\"en\" />\n"
"<meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n"
"<meta name=\"language\" content=\"en\" />\n"
"<meta name=\"description\" content=\"Postrisc dump file\"/>\n"
"<meta name=\"viewport\" content=\"width=device-width\" />\n"
"<style type=\"text/css\">\n"
"    ." HIDDEN_DIV_CLASS " { clear:none; }\n"
"    label { cursor:pointer; }\n"
"    input { display:none; }\n"
"    input:checked + ." HIDDEN_DIV_CLASS " { display:none; }\n"
"</style>\n"
"</head>\n"
"<body>\n";

    //out << "<pre>" << fmt::endl;
#if 0
    out << "printable byte values:";
    for (unsigned i=0; i<256; i++) {
        if (i % 16 == 0) out << fmt::endl;
        out << "0x" << fmt::hex<u8>(i) << ' ';
        dump_byte(i);
        out << ' ';
    }
    out << fmt::endl << fmt::endl;
#endif
}

void DumpFormatter::finish_dump(void) const
{
    if (!is_html()) return;
    //out << "</pre>" << fmt::endl;
    out << "</body></html>" << fmt::endl;
}

void DumpFormatter::chapter(char const *name) const
{
    if (is_html()) {
        out << "<h1>" << name << "</h1>" << fmt::endl;
    } else {
        out << name << fmt::endl;
    }
}

void DumpFormatter::dump_utf8(unsigned codepoint) const
{
    if (codepoint <= 0x7f)
        out << static_cast<char>(codepoint);
    else if (codepoint <= 0x7ff) {
        out << static_cast<char>(0xc0 | ((codepoint >>  6) & 0x1f));
        out << static_cast<char>(0x80 | ((codepoint >>  0) & 0x3f));
    } else if (codepoint <= 0xffff) {
        out << static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f));
        out << static_cast<char>(0x80 | ((codepoint >>  6) & 0x3f));
        out << static_cast<char>(0x80 | ((codepoint >>  0) & 0x3f));
    } else {
        out << static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07));
        out << static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f));
        out << static_cast<char>(0x80 | ((codepoint >>  6) & 0x3f));
        out << static_cast<char>(0x80 | ((codepoint >>  0) & 0x3f));
    }
}

void DumpFormatter::dump_byte(u8 byte) const
{
    if (is_html()) {
        // html special chars
        switch (byte) {
        case  '<': out << "&lt;"; return;
        case  '>': out << "&gt;"; return;
        case  '&': out << "&amp;"; return;
        case '\"': out << "&quot;"; return;
        case '\'': out << "&apos;"; return;
        }
        // printable ASCII
        if (std::isprint(byte)) {
            out << static_cast<char>(byte);
            return;
        }
#if 0
        if (byte < 0x20) { // special
            return dump_utf8(0x0430 + byte); // cyrillic
            // out << '.';
            return;
        } else if (byte == 0x7f) { // special DEL
            out << '.';
            return;
        } else if (byte == 0xa0) { // latin 1 NBSP
            out << '.';
            return;
        } else if (byte == 0xad) { // latin 1 SHY
            out << '.';
            return;
        } else if (byte >= 0xa0) { // latin 1 printable
            return dump_utf8(byte);
        }
        else if (byte >= 0x80 && byte < 0xb0) {
            // greek 0x3b1 in place of latin 1 service symbols
            return dump_utf8(0x3b1 + byte - 0x80);
        }

        LOG_ERROR(LOG_PLATFORM) << "unhandled byte: " << +byte;
#endif
        out << '.';
        //if (byte < 0x80)
        //   out << byte;
        //if (byte < 0xb0)
        //   out << '&' << '#' << (0x100 + byte - 0x7f) << ';'; // latin extended-A //cyrillic
    } else {
        if (std::isprint(byte)) {
            out << static_cast<char>(byte);
        } else {
            out << '.';
        }
    }
}

void DumpFormatter::dump_physical_address(u64 address) const
{
    if (is_html()) {
        out << "<b>" << fmt::hex(address) << "</b>";
    } else {
        out << fmt::hex(address) << fmt::endl;
    }
}

void DumpFormatter::dump_physical_address_ref(u64 address) const
{
    if (is_html()) {
        out << "<a href=\"#phys_" << fmt::hex(address) << "\">";
        out << fmt::hex(address);
        out << "</a>";
    } else {
        out << fmt::hex(address);
    }
}

void DumpFormatter::dump_virtual_address(u64 address) const
{
    out << fmt::hex(address);
}

void DumpFormatter::start_device(const std::string& name) const
{
    if (is_html()) {
        out << "<p><b><label for=\"hd" << name << "\">";
        out << name;
        out << "</label></b></p>";

        out << "<div><input type=\"checkbox\" id=\"hd" << name
            << "\" checked=\"checked\"/>";
        out << "<div class=\"" HIDDEN_DIV_CLASS "\"><pre>" << fmt::endl;
    } else {
        out << name << fmt::endl;
    }
}

void DumpFormatter::finish_device(const std::string& UNUSED(name)) const
{
    if (is_html()) {
        out << "</pre></div></div>";
    }
    out << fmt::endl;
}

void DumpFormatter::start_page(u64 address, dump_page_flags flags) const
{
    if (is_html()) {
        out << "<p><b><label for=\"hd" << fmt::hex(address) << "\">";
        out << "<b id=\"phys_" << fmt::hex(address) << "\">" << fmt::hex(address) << "</b>";
        if (flags & dump_page_memory) {
            if (flags & dump_page_allocated) {
                out << " allocated";
            } else {
                out << " free";
            }
        } else {
            out << " mmio";
        }
        out << "</label></b></p>";
        out << "<div><input type=\"checkbox\" id=\"hd" << fmt::hex(address)
            << "\" checked=\"checked\"/>";
        out << "<div class=\"" HIDDEN_DIV_CLASS "\"><pre>";
    }
    out << fmt::endl;
}

void DumpFormatter::finish_page(u64 UNUSED(address)) const
{
    if (is_html()) {
        out << "</pre></div></div>";
    }
    out << fmt::endl;
}

void DumpFormatter::start_section(void) const
{
    if (is_html()) {
        out << "<div><pre>";
    }
}

void DumpFormatter::finish_section(void) const
{
    if (is_html()) {
        out << "</pre></div>" << fmt::endl;
    }
}

} // namespace postrisc

