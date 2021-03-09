# http_server
C++ library for quickly implementing a multithreaded HTTP/\*nix socket server

# Features
- TCP and UDP support 
- Maintain client history for the lifetime of the program, log out
- 2 runtime modes: default and interpreter


Basic usage:
```
#include <iostream>
#include "server.h"

// using namespace http;

int main(int argc, char** argv)
{
    http::Server s(argc, argv);
    s.go();

    /* perform any action while server is running */
    s.ban("10.0.0.123");
    s.print_IP_banlist();
    s.set_host_IP("10.0.0.175");

    http::LocalClient lcl;
    std::vector<http::Message> lr;

    lr.push_back(lcl.GET("index.html"));
    lr.push_back(lcl.GET("this_page.html"));
    lr.push_back(lcl.GET("another.html"));

    s.run_for(60);
    s.kill();

    s.show("10.0.0.789");

    for (auto msg : lr) {
        std::cout << msg;
    }

    return 0;
}
```
