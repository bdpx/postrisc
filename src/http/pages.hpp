#pragma once

#include <map>

using resource_function = void (*)(std::ostream& out, boost::beast::string_view target);
using resource_map = std::map<std::string, resource_function>;

void page_root(std::ostream& out, boost::string_view target);
