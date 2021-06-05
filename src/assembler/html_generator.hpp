#pragma once

namespace postrisc {

class Decoder;

int arch_html(const Decoder& decoder, std::ostream & out);

void xhtml_header(std::ostream& out, const char *title);

namespace llvm {
int TableGenGenerateEncoding(std::ostream & out);
} // namespace llvm

} // namespace postrisc
