#pragma once

#include "util/common.hpp"
#include "emulator/SoftFpu.hpp"
#include "util/quadruple.hpp"

namespace postrisc {
namespace fpu {

template<typename T> class ValuePrinter;
template<typename T> std::ostream& operator<<(std::ostream& out, const ValuePrinter<T>& r);

template<typename T> class ValuePrinter {
public:
    explicit ValuePrinter(T v_) : v(v_) {}
    friend std::ostream& operator<< <> (std::ostream& out, const ValuePrinter& v);

    template<typename TT> static void writeClassify(std::ostream& out, TT f) {
        if (is_inf(f)) out << " inf";
        if (is_nan(f)) {
            out << (is_signan(f) ? " snan" : " qnan");
        }
    }

    T v;
};

std::ostream& operator<<(std::ostream& out, const ValuePrinter<bool>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i16>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u16>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i32>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u32>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i64>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u64>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<i128>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<u128>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<f16>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<f32>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<f64>& v);
std::ostream& operator<<(std::ostream& out, const ValuePrinter<f128>& v);

} // namespace fpu
} // namespace postrisc
