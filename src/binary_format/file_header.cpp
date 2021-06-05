#include "util/common.hpp"
#include "file_header.hpp"

/***********************************************************************
* binary file header format
***********************************************************************/

namespace postrisc {

ExeFileHeader::ExeFileHeader(size_t text, size_t rodata, size_t data)
{
    memset(this, 0, sizeof(*this));

    prefix        = exe_file_prefix;
    size_header   = sizeof(*this);
    fmt_version   = 1;
    abi_version   = 1;
    size_text     = text;
    size_rodata   = rodata;
    size_data     = data;
    size_stack    = 64 * 1024;
    size_heap     = 64 * 1024;

    pad_final.fill(0xff);
}

void ExeFileHeader::read(std::istream& in)
{
    if (!in.read(reinterpret_cast<char *>(this), sizeof(*this)))
        LOG_ABORT << "can't read file prefix";

    if (prefix != exe_file_prefix)
        LOG_ABORT << "invalid file prefix, not executable file";

    if (size_header != sizeof(*this))
        LOG_ABORT << "invalid file prefix size";
}

void ExeFileHeader::write(std::ostream& out) const
{
    out.write(reinterpret_cast<const char*>(this), sizeof(*this));
}

} // namespace
