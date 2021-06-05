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

#include "listener.hpp"
#include "detect_session.hpp"
#include "plain_session.hpp"
#include "ssl_session.hpp"

detect_session::detect_session(
    boost::asio::ip::tcp::socket&& socket,
    boost::asio::ssl::context& ctx,
    std::shared_ptr<listener> const& listener)
    : stream_(std::move(socket))
    , ctx_(ctx)
    , listener_(listener)
{
    std::cerr << "detect_session ctor" << std::endl;
}

detect_session::~detect_session()
{
    std::cerr << "detect_session dtor" << std::endl;
}

void detect_session::run()
{
    // Set the timeout.
    boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Detect a TLS handshake
    async_detect_ssl(
        stream_,
        buffer_,
        boost::beast::bind_front_handler(
            &detect_session::on_detect,
            shared_from_this()));
}

void
detect_session::on_detect(boost::beast::error_code ec, bool result)
{
    if(ec)
        return fail(ec, "detect");

    if(result) {
        // Launch SSL session
        auto session = std::make_shared<ssl_session>(stream_.release_socket(), ctx_, std::move(buffer_), listener_);
        session->run();
        return;
    }

    // Launch plain session
    auto session = std::make_shared<plain_session>(stream_.release_socket(), std::move(buffer_), listener_);
    session->run();
}
