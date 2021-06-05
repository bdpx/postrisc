#pragma once

#include <string>
#include "header.hpp"

namespace http {
namespace server {

// Structure of a HTTP request received from a client.
//
// todo The headers list should probably become a map (but harder parsing)
// todo Add the decoded/tokenized post_params[] and get_params[] options here
struct request {
    ALWAYS_INLINE ~request() {}
    std::string        method;
    std::string        uri;
    std::string        query;
    int                http_version_major;
    int                http_version_minor;
    headers_list       headers;
    size_t             content_length;
    std::string        content;
};

} // namespace server
} // namespace http
