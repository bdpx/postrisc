#pragma once

#include "pages.hpp"

void fail(boost::beast::error_code ec, char const* what);

class listener;

// Handles an HTTP server connection.
// This uses the Curiously Recurring Template Pattern so that
// the same code works with both SSL streams and regular sockets.
template<typename Derived>
class session {
public:
    // Take ownership of the buffer
    session(boost::beast::flat_buffer buffer,
            std::shared_ptr<listener> const& listener)
        : buffer_(std::move(buffer))
        , listener_(listener)
    {
    }

    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred);

private:
    // Access the derived class, this is part of
    // the Curiously Recurring Template Pattern idiom.
    Derived& derived()
    {
        return static_cast<Derived&>(*this);
    }

    template<bool isRequest, typename Body, typename Fields>
    void
    send(boost::beast::http::message<isRequest, Body, Fields>&& msg);

    template<typename Body, typename Allocator>
    void
    handle_request(boost::beast::http::request<Body,
                   boost::beast::http::basic_fields<Allocator>>&& req);

protected:
    boost::beast::flat_buffer buffer_;

private:
    std::shared_ptr<listener>                                     listener_;
    boost::beast::http::request<boost::beast::http::string_body>  req_;
    std::shared_ptr<void>                                         res_;
};

template<typename Derived>
void
session<Derived>::do_read()
{
    // Set the timeout.
    boost::beast::get_lowest_layer(derived().stream())
        .expires_after(std::chrono::seconds(30));

    // Read a request
    boost::beast::http::async_read(
        derived().stream(),
        buffer_,
        req_,
        boost::beast::bind_front_handler(
            &session::on_read,
            derived().shared_from_this()));
}

template<typename Derived>
void
session<Derived>::on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    if(close) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return derived().do_eof();
    }

    // We're done with the response so delete it
    res_ = nullptr;

    // Read another request
    do_read();
}

template<typename Derived>
template<bool isRequest, typename Body, typename Fields>
void
session<Derived>::send(boost::beast::http::message<isRequest, Body, Fields>&& msg)
{
    // The lifetime of the message has to extend
    // for the duration of the async operation so
    // we use a shared_ptr to manage it.
    auto sp = std::make_shared<
        boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

    // Store a type-erased version of the shared
    // pointer in the class to keep it alive.
    res_ = sp;

    // Write the response
    boost::beast::http::async_write(
        derived().stream(),
        *sp,
        boost::beast::bind_front_handler(
            &session::on_write,
            derived().shared_from_this(),
            sp->need_eof()));
}

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<typename Derived>
template<typename Body, typename Allocator>
void
session<Derived>::handle_request(
    boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        boost::beast::http::response<boost::beast::http::string_body>
            res{boost::beast::http::status::bad_request, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

#if 0
    // Returns a not found response
    auto const not_found =
    [&req](boost::beast::string_view target)
    {
        boost::beast::http::response<boost::beast::http::string_body>
            res{boost::beast::http::status::not_found, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](boost::beast::string_view what)
    {
        boost::beast::http::response<boost::beast::http::string_body>
            res{boost::beast::http::status::internal_server_error, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };
#endif

    // Make sure we can handle the method
    if( req.method() != boost::beast::http::verb::get &&
        req.method() != boost::beast::http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Attempt to open the file
    // boost::beast::error_code ec;

    // Respond to HEAD request
    if (req.method() == boost::beast::http::verb::head) {
        boost::beast::http::response<boost::beast::http::empty_body>
            res{boost::beast::http::status::ok, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.content_length(0/*size*/);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }

    // Check if the request is for a registered dynamic resource
    //resource_map::const_iterator resource = resource_map_.find(request_path);
    //if (resource_map_.end() != resource) {
        boost::beast::http::response<boost::beast::http::string_body> res; //dynamic
        res.version(req.version());
        res.keep_alive(req.keep_alive());
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        std::ostringstream oss;
        page_root(oss, req.target());
        //resource->second(oss);
        res.body() = oss.str();
        res.content_length(res.body().size());
        return send(std::move(res));
    //}

    //return send(not_found(req.target()));
}
