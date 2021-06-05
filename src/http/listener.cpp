#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "session.hpp"
#include "listener.hpp"
#include "detect_session.hpp"

listener::listener(
    boost::asio::io_context& ioc,
    boost::asio::ssl::context& ctx,
    boost::asio::ip::tcp::endpoint endpoint)
    : ioc_(ioc)
    , ctx_(ctx)
    , acceptor_(boost::asio::make_strand(ioc))
{
    boost::beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec) {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if(ec) {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if(ec) {
        fail(ec, "listen");
        return;
    }
}

listener::~listener()
{
    std::cerr << "listener dtor" << std::endl;
}

void
listener::run()
{
    do_accept();
}

void
listener::do_accept()
{
    // The new connection gets its own strand
    acceptor_.async_accept(
        boost::asio::make_strand(ioc_),
        boost::beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}

void
listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
{
    if(ec) {
        fail(ec, "accept");
    } else {
        // Create the detector session and run it
        auto session = std::make_shared<detect_session>(std::move(socket), ctx_, shared_from_this());
        session->run();
    }

    // Accept another connection
    do_accept();
}
