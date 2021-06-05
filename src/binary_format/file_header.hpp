#pragma once

namespace postrisc {

/***************************************************************************
* file header for executables (must be POD)
***************************************************************************/
class ExeFileHeader {
public:
    u64    prefix;         // standard portable prefix for executable file

    u8     fmt_version;    // format version
    u8     abi_version;    // ABI version
    u16    size_header;    // this prefix size for loader check
    u32    pad1;

    u64    entry_point;    // text-relative address

    u64    size_text;      // text section (execute-only)
    u64    size_rodata;    // rodata section (read-only)
    u64    size_data;      // data section (read-write) in file
    u64    size_data_bss;  // data section (read-write) zeros
    u64    size_tls;       // thread local storage (read-write) in file
    u64    size_tls_bss;   // thread local storage (read-write) zeros
    u64    size_stack;     // main thread stack size
    u64    size_heap;      // main heap initial size

    std::array<u8, 512 - 88>  pad_final;

private:
    static const u64 exe_file_prefix = U64C(0x6373697274736f70);

public:
    ExeFileHeader() {}
    ExeFileHeader(size_t text, size_t rodata, size_t data);
    void read(std::istream& in);
    void write(std::ostream& out) const;
};

static_assert(sizeof(ExeFileHeader) == 512);

} // namespace postrisc
