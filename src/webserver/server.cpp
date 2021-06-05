#include <iostream>
#include <fstream>
#include <boost/bind/bind.hpp>

#include "server.hpp"

namespace http {
namespace server {

server::server(const std::string& address, unsigned short port_num)
    : io_context_()
    , doc_root_(".")
    , acceptor_(io_context_)
    , new_connection_(new connection(io_context_, *this))
{

    boost::system::error_code ec;
    // Step 2. Using IP protocol version independent address representation.
    boost::asio::ip::address ip_address = boost::asio::ip::make_address(address, ec);
    if (ec.value() != 0) {
        // Provided IP address is invalid. Breaking execution.
        std::cerr << "Failed to parse the IP address. Error code = "
                  << ec.value() << ". Message: " << ec.message() << std::endl;
    }

      
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    //boost::asio::ip::tcp::resolver resolver(io_context_);
    //boost::asio::ip::tcp::resolver::query query(address, port);
    //boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

    boost::asio::ip::tcp::endpoint endpoint(ip_address, port_num);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    acceptor_.async_accept(new_connection_->socket(),
                           boost::bind(&server::handle_accept, this,
                           boost::asio::placeholders::error));
}

server::~server()
{
    std::cerr << "dtor -server" << std::endl;
}

void server::run()
{
    // The io_context::run() call will block until all asynchronous operations
    // have finished. While the server is running, there is always at least one
    // asynchronous operation outstanding: the asynchronous accept call waiting
    // for new incoming connections.
    io_context_.run();
}

void server::stop()
{
    // Post a call to the stop function so that server::stop() is safe to call
    // from any thread.
    boost::asio::post(io_context_, boost::bind(&server::handle_stop, this));
}

void server::handle_accept(const boost::system::error_code& e)
{
    if (!e) {
        start(new_connection_);
        new_connection_.reset(new connection(io_context_, *this));
        acceptor_.async_accept(new_connection_->socket(),
                               boost::bind(&server::handle_accept, this,
                               boost::asio::placeholders::error));
    }
}

void server::handle_stop()
{
    // The server is stopped by cancelling all outstanding asynchronous
    // operations. Once all operations have finished the io_context::run() call
    // will exit.
    acceptor_.close();
    stop_all();
}

void server::start(connection_ptr c)
{
  connections_.insert(c);
  c->start();
}

void server::stop(connection_ptr c)
{
  connections_.erase(c);
  c->stop();
}

void server::stop_all()
{
    for (const auto& conn : connections_) {
         conn->stop();
    }
    connections_.clear();
}

void server::handle_request(const request& req, reply& rep)
{
    // Decode url to path.
    std::string request_path;
    if (!url_decode(req.uri, request_path)) {
        reply::stock_reply(reply::bad_request, rep);
        return;
    }

    // Decode and tokenize the query part of the URI
    params_map params;
    std::string query;
    if (!url_decode(req.query, query)) {
        reply::stock_reply(reply::bad_request, rep);
        return;
    }
    query_tokenize(query, params);

    // Decode and tokenize the content part of the POST request
    std::string content;
    if (!url_decode(req.content, content)) {
        reply::stock_reply(reply::bad_request, rep);
        return;
    }
    query_tokenize(content, params);

    // Check if the request is for a registered dynamic resource
    resource_map::const_iterator resource = resource_map_.find(request_path);
    if (resource_map_.end() != resource) {
        resource->second(req, params, rep, *this);
        // rep.headers.push_back(http::server::header{"Content-Length", std::to_string(rep.content.size())});
        return;
    }

    reply::redirect_reply("/", rep);
    return;
}

bool server::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else if (in[i] == '+') {
      out += ' ';
    } else {
      out += in[i];
    }
  }
  return true;
}

void
server::query_tokenize(const std::string& in, params_map& out)
{
  bool        in_option_name = true;
  std::string option_name;
  std::string option_value;

  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in_option_name) {
      // parsing the name of an option
      if (in[i] == '=') {
        in_option_name = false;
      } else if (in[i] == '&') {
        out[option_name] = ""; // option without value
        option_name.clear();
      } else {
        option_name += in[i];
      }
    } else {
      // parsing the value of an option
      if (in[i] == '&') {
        out[option_name] = option_value; // option with value
        option_name.clear ();
        option_value.clear ();
        in_option_name = true;
      } else {
        option_value += in[i];
      }
    }
  }

  if (false == option_name.empty()) {
    out[option_name] = option_value; // last option with or without value
  }
}

void server::register_resource(const std::string& resource_name, resource_function&& function)
{
  resource_map_.insert(std::pair<std::string, resource_function>(resource_name, std::move(function)));
}

void server::unregister_resource(const std::string& resource_name)
{
  resource_map::iterator resource = resource_map_.find(resource_name);
  if (resource_map_.end() != resource) {
    resource_map_.erase(resource);
  }
}

} // namespace server
} // namespace http
