#include <iostream>
#include <iomanip>
#include <vector>
#include <boost/bind/bind.hpp>

#include "connection.hpp"
#include "server.hpp"

namespace http {
namespace server {

connection::connection(boost::asio::io_context& io_context,
                       server& server)
  : socket_(io_context),
    server_(server),
    buffer_()
{
    std::cerr << "ctor +connection" << std::endl;
}

connection::~connection()
{
    std::cerr << "dtor -connection" << std::endl;
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start()
{
  // ask to read asynchronously the client request
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
  socket_.close();
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e) {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result) {
      // parse done successfully, handle the request
      server_.handle_request(request_, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    } else if (!result) {
      // error interrupted the parse of the request
      reply::stock_reply(reply::bad_request, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    } else {
      // request not complete, ask to keep reading asynchronously
      socket_.async_read_some(boost::asio::buffer(buffer_),
          boost::bind(&connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
  } else if (e != boost::asio::error::operation_aborted) {
    server_.stop(shared_from_this());
  }
}

void connection::handle_write(const boost::system::error_code& e)
{
  if (!e) {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted) {
    server_.stop(shared_from_this());
  }
}

} // namespace server
} // namespace http
