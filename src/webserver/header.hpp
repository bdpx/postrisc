#pragma once

#include <string>
#include <vector>

namespace http {
namespace server {

/// Structure of a header in a HTTP request or reply
struct header {
  ~header();
  std::string name;   ///< normalized name of the header
  std::string value;  ///< string value of the header
};

/// List of HTTP headers.
using headers_list = std::vector<header>;

} // namespace server
} // namespace http
