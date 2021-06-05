#include "util/common.hpp"
#include "arch/isa.hpp"
#include "program_section.hpp"

namespace postrisc {

CProgramSection::CProgramSection(uint32_t id, ESectionKind type)
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
CProgramSection::write(uint8_t val)
{
    image.push_back( static_cast<char>(val) );
}

void
CProgramSection::write(uint16_t val)
{
    image.push_back( static_cast<char>(val >>  0) );
    image.push_back( static_cast<char>(val >>  8) );
}

void
CProgramSection::write(uint32_t val)
{
    image.push_back( static_cast<char>(val >>  0) );
    image.push_back( static_cast<char>(val >>  8) );
    image.push_back( static_cast<char>(val >> 16) );
    image.push_back( static_cast<char>(val >> 24) );
}

void
CProgramSection::write(uint64_t val)
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
CProgramSection::write(fpu::u128 val)
{
    write( val.low() );  //little endian!!!
    write( val.high() );
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
    write( util::change_bit_presentation<float, uint32_t>(x) );
}

void
CProgramSection::write(double x)
{
    write( util::change_bit_presentation<double, uint64_t>(x) );
}

void
CProgramSection::write(quadruple x)
{
    write( util::change_bit_presentation<quadruple, fpu::u128>(x) );
}

void
CProgramSection::align(size_t alignment)
{
    uint8_t const filler = 0;
    while (image.size() & (alignment-1)) {
        write(filler);
    }
}

uint64_t
CProgramSection::ComputeIpRelative(int64_t ip, uint64_t offset) const
{
    return global_offset + offset - ip;
}

uint64_t
CProgramSection::ComputeGlobalOffset(uint64_t offset) const
{
    return offset + global_offset;
}

uint64_t
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
