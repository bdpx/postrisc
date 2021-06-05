#pragma once

class listener;

// Detects SSL handshakes
class detect_session : public std::enable_shared_from_this<detect_session> {
public:
    detect_session(
        boost::asio::ip::tcp::socket&& socket,
        boost::asio::ssl::context& ctx,
        std::shared_ptr<listener> const& listener);

    ~detect_session();

    // Launch the detector
    void run();
    void on_detect(boost::beast::error_code ec, bool result);

private:
    boost::beast::tcp_stream            stream_;
    boost::asio::ssl::context&          ctx_;
    std::shared_ptr<listener>           listener_;
    boost::beast::flat_buffer           buffer_;
};
