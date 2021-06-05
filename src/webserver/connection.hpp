#pragma once

#include <utility>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_parser.hpp"

namespace http {
namespace server {

class server;

class connection
    : public boost::intrusive_ref_counter<connection, boost::thread_safe_counter>
    , private boost::noncopyable
{
public:
    explicit connection(boost::asio::io_context& io_context, server& server);
    ~connection();

    /// Get the socket associated with the connection.
    boost::asio::ip::tcp::socket& socket();

    /// Start the first asynchronous operation for the connection.
    void start();

    /// Stop all asynchronous operations associated with the connection.
    void stop();

    connection *shared_from_this() { return this; }

private:
    void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
    void handle_write(const boost::system::error_code& e);

private:
    boost::asio::ip::tcp::socket  socket_;
    server&                       server_;
    boost::array<char, 8192>      buffer_;
    request                       request_;
    request_parser                request_parser_;
    reply                         reply_;
};

using connection_ptr = boost::intrusive_ptr<connection>;

} // namespace server
} // namespace http
