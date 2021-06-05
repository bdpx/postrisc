#include <iostream>
#include <string>

#include "server.hpp"

void
on_request_page_root(const http::server::request& /*req*/,
                     const http::server::params_map& /*params*/,
                     http::server::reply& rep,
                     http::server::server& /*serv*/)
{
    std::string reply =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "    <title>Welcome</title>\n"
    "</head>\n"
    "<body>\n"
    "    <p>Welcome</p>\n"
    "    <p><a href=\"/stop\">stop</a></p>\n"
    "</body></html>\n";

  rep.content = reply;
  rep.headers.push_back(http::server::header{"Content-Type", "text/html"});
  rep.status = http::server::reply::ok;
}

void
on_request_page_stop(const http::server::request& /*req*/,
                     const http::server::params_map& params,
                     http::server::reply& rep,
                     http::server::server& serv)
{
    if (params.find("confirm") != params.end()) {
        // ask asynchronously the server to stop : we will get our stop page before
        serv.stop();
        rep.content = "<html><head><title>Stopped</title><body><p>Stopped</p></body></html>";
    } else {
        rep.content = "<html><head><title>Stop</title><body><p>Stop</p><a href=\"/\">back</a><br/><a href=\"/stop?confirm\">confirm</a></body></html>";
    }
    rep.headers.push_back(http::server::header{"Content-Type", "text/html"});
    rep.status = http::server::reply::ok;
}

int main(void)
{
    std::cout << "go to 127.0.0.1 port 9000" << std::endl;
    try {
        http::server::server serv("127.0.0.1" /* 0::0 for IPv6 */, 9000);

        // Register our two dynamic resources (web page "/" and "/stop")
        serv.register_resource("/",      on_request_page_root);
        serv.register_resource("/stop",  on_request_page_stop);

        // Run the server until stopped.
        serv.run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
