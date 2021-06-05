#pragma once

namespace postrisc {

std::ostream& operator<< (std::ostream& os, quadruple f);
std::istream& operator>> (std::istream& os, quadruple& f);

} // namespace postrisc
