#include "fmt.hpp"

namespace postrisc {
namespace fmt {

std::ostream& flush(std::ostream& out)
{
    // out << std::flush;
    return out;
}

std::ostream& endl(std::ostream& out)
{
    return out << '\n';
}

void hexdump(u8 const *data, size_t len, uintptr_t offset, std::ostream& out)
{
    static size_t const width = 64;

    for (size_t base = 0; base < len; base += width, offset += width) {
        out << fmt::hex(offset) << ' ';
        for (size_t i = 0; i < width; ++i) {
            if (8*i % width == 0) out << ' ';
            if (base + i < len)
                out << fmt::hex(data[base + i]);
            else
                out << ' ' << ' ';
        }
        out << ' ' << ' ';
        for (size_t i = 0; i < width; ++i) {
            if (base + i >= len)
                break;
            u8 const c = data[base + i];
            out << (char)(c < 32 || c > 126 ? '.' : c);
        }
        out << std::endl;
    }
}

template<typename T>
inline void data_logger::dump_value(std::ostream& out) const
{
    T value = *reinterpret_cast<T const*>(data);
    out << fmt::hex(value) << ' ' << '\'';
    for (size_t i = 0; i < sizeof(value); i++) {
        char c = static_cast<char>(value & 0xff);
        out << (std::isprint(c) ? c : '.');
        value >>= 8;
    }
    out << '\'';
}

template<>
inline void data_logger::dump_value<u8>(std::ostream& out) const
{
    u8 value = *reinterpret_cast<u8 const*>(data);
    out << fmt::hex(value) << ' ' << '\'';
    char c = static_cast<char>(value & 0xff);
    out << (std::isprint(c) ? c : '.');
    out << '\'';
}

std::ostream& operator<<(std::ostream& out, const data_logger& r)
{
    switch (r.len) {
        case 1: r.dump_value< u8>(out);  break;
        case 2: r.dump_value<u16>(out);  break;
        case 4: r.dump_value<u32>(out);  break;
        case 8: r.dump_value<u64>(out);  break;
    }
    return out;
}

} // namespace fmt
} // namespace postrisc
