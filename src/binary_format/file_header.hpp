#pragma once

namespace postrisc {

/***************************************************************************
* file header for executables (must be POD)
***************************************************************************/
class CExeFileHeader {
public:
    uint64_t    prefix;         // standard portable prefix for executable file

    uint8_t     fmt_version;    // format version
    uint8_t     abi_version;    // ABI version
    uint16_t    size_header;    // this prefix size for loader check
    uint32_t    pad1;

    uint64_t    entry_point;    // text-relative address

    uint64_t    size_text;      // text section (execute-only)
    uint64_t    size_rodata;    // rodata section (read-only)
    uint64_t    size_data;      // data section (read-write) in file
    uint64_t    size_data_bss;  // data section (read-write) zeros
    uint64_t    size_tls;       // thread local storage (read-write) in file
    uint64_t    size_tls_bss;   // thread local storage (read-write) zeros
    uint64_t    size_stack;     // main thread stack size
    uint64_t    size_heap;      // main heap initial size

    std::array<uint64_t, 5+16>  pad2;    // padding to 256 bytes (reserved)

private:
    static const uint64_t exe_file_prefix = UINT64_C(0x6373697274736f70);

public:
    CExeFileHeader() {}
    CExeFileHeader(size_t text, size_t rodata, size_t data);
    void read(std::istream & in);
    void write(std::ostream & out) const;
};

} // namespace postrisc
