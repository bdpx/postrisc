#include "ValuePrinter.hpp"

namespace postrisc {
namespace fpu {

std::ostream& operator<<(std::ostream& out, const ValuePrinter<bool>& v) { return out << v.v; }

std::ostream& operator<<(std::ostream& out, const ValuePrinter<i16>& v) { return out << fmt::hex(static_cast<u16>(v.v & 0xFFFF)) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u16>& v) { return out << fmt::hex(static_cast<u16>(v.v & 0xFFFF)) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i32>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u32>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i64>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u64>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }

using postrisc::operator<<;
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i128>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u128>& v) { return out << fmt::hex(v.v) << ' ' << v.v; }

std::ostream& operator<<(std::ostream& out, const ValuePrinter<f16>& v)
{
    // FIXME: std::numeric_limits<f16::F_HARD>::max_digits10 == 0
    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_frac = v.v.get_fraction();

    out << (a_sign ? '-' : '+')
        << fmt::hex<typename f16::F_UINT, (f16::EXP_SIZE + 3) / 4>(a_exp)
        << '.'
        << fmt::hex<typename f16::F_UINT, (f16::FRAC_SIZE + 3) / 4>(a_frac)
        << ' ' << std::setprecision(6) << v.v.hard();

    ValuePrinter<f16>::writeClassify(out, v.v);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ValuePrinter<f32>& v)
{
    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_frac = v.v.get_fraction();

    out << (a_sign ? '-' : '+')
        << fmt::hex<typename f32::F_UINT, (f32::EXP_SIZE + 3) / 4>(a_exp)
        << '.'
        << fmt::hex<typename f32::F_UINT, (f32::FRAC_SIZE + 3) / 4>(a_frac)
        << ' '
        << std::setprecision(12) << v.v.hard();

    ValuePrinter<f32>::writeClassify(out, v.v);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ValuePrinter<f64>& v)
{
    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_frac = v.v.get_fraction();

    out << (a_sign ? '-' : '+')
        << fmt::hex<typename f64::F_UINT, (f64::EXP_SIZE + 3) / 4>(a_exp)
        << '.'
        << fmt::hex<typename f64::F_UINT, (f64::FRAC_SIZE + 3) / 4>(a_frac)
        << ' '
        << std::setprecision(18) << v.v.hard();

    ValuePrinter<f64>::writeClassify(out, v.v);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ValuePrinter<f128>& v)
{
    using postrisc::operator<<;

    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_frac = v.v.get_fraction();

    out << (a_sign ? '-' : '+')
        << fmt::hex<u64, (f128::EXP_SIZE + 3) / 4>(a_exp)
        << '.'
        << fmt::hex<u64, ((f128::FRAC_SIZE - 64) + 3) / 4>((u64)(a_frac >> 64))
        << '_'
        << fmt::hex((u64)(a_frac))
        << ' '
        << std::setprecision(38) << v.v.hard();

    ValuePrinter<f128>::writeClassify(out, v.v);
    return out;
}

} // namespace fpu
} // namespace postrisc
