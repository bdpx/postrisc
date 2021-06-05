#pragma once

#include "pages.hpp"

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener> {
public:
    listener(
        boost::asio::io_context& ioc,
        boost::asio::ssl::context& ctx,
        boost::asio::ip::tcp::endpoint endpoint);
    ~listener();

    // Start accepting incoming connections
    void run();

private:
    void do_accept();
    void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

private:
    boost::asio::io_context&           ioc_;
    boost::asio::ssl::context&         ctx_;
    boost::asio::ip::tcp::acceptor     acceptor_;
    resource_map                       resource_map_;
};
