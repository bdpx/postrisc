#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "connection.hpp"

namespace http {
namespace server {

class server;

using params_map = std::map<std::string, std::string>;
using resource_function = void (*)(const request& req, const params_map& params, reply& rep, server& serv);
using resource_map = std::map<std::string, resource_function>;

class server : private boost::noncopyable {
public:
    explicit server(const std::string& address, unsigned short port_num);
    ~server();

    void run();
    void stop();

    void start(connection_ptr c);
    void stop(connection_ptr c);
    void stop_all();

    void handle_request(const request& req, reply& rep);
    void register_resource(const std::string& resource_name, resource_function&& function);
    void unregister_resource(const std::string& resource_name);

private:
    void handle_accept(const boost::system::error_code& e);
    void handle_stop();
    static bool url_decode(const std::string& in, std::string& out);
    static void query_tokenize(const std::string& in, params_map& out);

private:
    resource_map                     resource_map_;
    boost::asio::io_context          io_context_;
    std::string                      doc_root_;
    boost::asio::ip::tcp::acceptor   acceptor_;
    std::set<connection_ptr>         connections_;
    connection_ptr                   new_connection_;
};

} // namespace server
} // namespace http
