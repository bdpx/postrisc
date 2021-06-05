#include "util/common.hpp"
#include "arch/isa.hpp"
#include "program_section.hpp"

namespace postrisc {

CProgramSection::CProgramSection(u32 id, ESectionKind type)
    : global_offset(0)
    , m_Id(id)
    , m_Kind(type)
{
}

CProgramSection::~CProgramSection(void)
{
}

size_t
CProgramSection::size(void) const
{
    return image.size();
}

void
CProgramSection::clear(void)
{
    image.clear();
}

void
CProgramSection::reserve(size_t size)
{
    image.reserve(size);
}

char
CProgramSection::back(void) const
{
    return image.back();
}

void
CProgramSection::write(char val)
{
    image.push_back( val );
}

void
CProgramSection::write(u8 val)
{
    image.push_back( static_cast<char>(val) );
}

void
CProgramSection::write(u16 val)
{
    image.push_back( static_cast<char>(val >>  0) );
    image.push_back( static_cast<char>(val >>  8) );
}

void
CProgramSection::write(u32 val)
{
    image.push_back( static_cast<char>(val >>  0) );
    image.push_back( static_cast<char>(val >>  8) );
    image.push_back( static_cast<char>(val >> 16) );
    image.push_back( static_cast<char>(val >> 24) );
}

void
CProgramSection::write(u64 val)
{
    image.push_back( static_cast<char>(val >>  0) );
    image.push_back( static_cast<char>(val >>  8) );
    image.push_back( static_cast<char>(val >> 16) );
    image.push_back( static_cast<char>(val >> 24) );
    image.push_back( static_cast<char>(val >> 32) );
    image.push_back( static_cast<char>(val >> 40) );
    image.push_back( static_cast<char>(val >> 48) );
    image.push_back( static_cast<char>(val >> 56) );
}

void
CProgramSection::write(u128 val)
{
    // little endian!!!
    write( static_cast<u64>(val) );
    write( static_cast<u64>(val >> 64) );
}

void
CProgramSection::write(const CBundle & bundle)
{
    write( bundle.get_lo() );  //little endian!!!
    write( bundle.get_hi() );
}

void
CProgramSection::write(float x)
{
    write(std::bit_cast<u32>(x));
}

void
CProgramSection::write(double x)
{
    write(std::bit_cast<u64>(x));
}

void
CProgramSection::write(quadruple x)
{
    write(std::bit_cast<u128>(x));
}

void
CProgramSection::align(size_t alignment)
{
    u8 const filler = 0;
    while (image.size() & (alignment-1)) {
        write(filler);
    }
}

u64
CProgramSection::ComputeIpRelative(i64 ip, u64 offset) const
{
    return global_offset + offset - ip;
}

u64
CProgramSection::ComputeGlobalOffset(u64 offset) const
{
    return offset + global_offset;
}

u64
CProgramSection::ComputeNextIp(void) const
{
    return image.size() + bytes_per_bundle;
}

void
CProgramSection::write(std::ostream& out) const
{
    out.write( image.data(), image.size() );
}

/*************************************************************************
* this function print segment as hexadecimal for embedding in c.
*************************************************************************/
void
CProgramSection::print(const char name[], std::ostream& out) const
{
    const size_t nbytes = size();
    const size_t rowsize = 16;

    out << "\n/* " << name << " section "
        << std::dec << nbytes << " bytes */\n"
        << std::hex << std::setfill('0');

    for (size_t k = 0; k<nbytes; ++k) {
        if (k % rowsize == 0)
            out << ' ';

        out << "0x" << std::setw(2) << (255 & image[k]);
        out << ',';

        if (k % rowsize == (rowsize - 1))
            out << '\n';
        else
            out << ' ';
    }

    out << std::dec;
}

const char *
CProgramSection::GetString(size_t offset) const
{
    return image.data() + offset;
}

size_t
CProgramSection::GetNextGlobalOffset(void) const
{
    return global_offset + util::roundup(size(), bits_per_segment_offset);
}

} // namespace postrisc
