#pragma once

#include "session.hpp"

class ssl_session
    : public session<ssl_session>
    , public std::enable_shared_from_this<ssl_session>
{
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;

public:
    ssl_session(boost::asio::ip::tcp::socket&& socket,
                boost::asio::ssl::context& ctx,
                boost::beast::flat_buffer buffer,
                std::shared_ptr<listener> const& listener)
        : session<ssl_session>(std::move(buffer), listener)
        , stream_(std::move(socket), ctx)
    {
        std::cerr << "ssl_session ctor" << std::endl;
    }

    ~ssl_session()
    {
        std::cerr << "ssl_session dtor" << std::endl;
    }

    // Called by the base class
    boost::beast::ssl_stream<boost::beast::tcp_stream>& stream()
    {
        return stream_;
    }

    // Start the asynchronous operation
    void run()
    {
        auto self = shared_from_this();
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session.
        boost::asio::dispatch(stream_.get_executor(), [self]() {
            // Set the timeout.
            boost::beast::get_lowest_layer(self->stream_).expires_after(
                std::chrono::seconds(30));

            // Perform the SSL handshake
            // Note, this is the buffered version of the handshake.
            self->stream_.async_handshake(
                boost::asio::ssl::stream_base::server,
                self->buffer_.data(),
                boost::beast::bind_front_handler(
                    &ssl_session::on_handshake,
                    self));
        });
    }

    void on_handshake(boost::beast::error_code ec, std::size_t bytes_used)
    {
        if(ec)
            return fail(ec, "handshake");

        // Consume the portion of the buffer used by the handshake
        buffer_.consume(bytes_used);

        do_read();
    }

    void do_eof()
    {
        // Set the timeout.
        boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Perform the SSL shutdown
        stream_.async_shutdown(
            boost::beast::bind_front_handler(
                &ssl_session::on_shutdown,
                shared_from_this()));
    }

    void on_shutdown(boost::beast::error_code ec)
    {
        if(ec)
            return fail(ec, "shutdown");

        // At this point the connection is closed gracefully
    }
};

template<>
void
session<ssl_session>::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == boost::beast::http::error::end_of_stream)
        return derived().do_eof();

    if(ec)
        return fail(ec, "read");

    // Send the response
    handle_request(std::move(req_));
}
