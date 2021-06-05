#include "util/common.hpp"
#include "general_register.hpp"

namespace postrisc {

std::ostream& operator<<(std::ostream& out, const CGeneralRegister& f)
{
    return
    out << fmt::hex(f.high())
        << '_'
        << fmt::hex(f.low());
}

} //namespace postrisc
