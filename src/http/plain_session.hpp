#pragma once

#include "session.hpp"

class listener;

class plain_session
    : public session<plain_session>
    , public std::enable_shared_from_this<plain_session>
{
public:
    plain_session(boost::asio::ip::tcp::socket&& socket,
                  boost::beast::flat_buffer buffer,
                  std::shared_ptr<listener> const& listener)
        : session<plain_session>(std::move(buffer), listener)
        , stream_(std::move(socket))
    {
        std::cerr << "plain_session ctor" << std::endl;
    }

    ~plain_session()
    {
        std::cerr << "plain_session dtor" << std::endl;
    }

    // Called by the base class
    boost::beast::tcp_stream& stream()
    {
        return stream_;
    }

    // Start the asynchronous operation
    void run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        boost::asio::dispatch(stream_.get_executor(),
            boost::beast::bind_front_handler(&session::do_read, shared_from_this()));
    }

    void do_eof()
    {
        // Send a TCP shutdown
        boost::beast::error_code ec;
        stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }

private:
    boost::beast::tcp_stream stream_;
};

template<>
void
session<plain_session>::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == boost::beast::http::error::end_of_stream)
        return derived().do_eof();

    if(ec) {
        std::cerr << "read failed" << std::endl;
        return fail(ec, "read");
    }

    std::cerr << "host=" << req_[boost::beast::http::field::host] << std::endl;

    boost::beast::http::response<boost::beast::http::empty_body>
        res{boost::beast::http::status::moved_permanently, req_.version()};
    //res.set(boost::beast::http::field::content_type, "text/html");
    std::string ans = "https://";
    ans += std::string(req_[boost::beast::http::field::host]);
    ans += std::string(req_.target());
    res.set(boost::beast::http::field::location, ans);
    res.keep_alive(req_.keep_alive());
    send(std::move(res));
}
