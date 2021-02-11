# http_server
C++ library for quickly implementing a multithreaded HTTP/\*nix socket server

# Features
- Supports TCP and UDP protocols 
- Maintains client history for the lifetime of the program, which may be periodically logged out
- 2 runtime modes 
    - Default: While in scope, any code or method may be run or called on the server object
    - Interp: Shell-style interpreter which supports the same core commands:
        - ban - add an IPv4 address to the banlist
        - allow - remove an IPv4 address to the banlist
        - show - show logged data from any client in the history, or show banlist
        - kill - kills server, but does not exit main()
- Edit config.h to define desired behavior


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
    lr.push_back(lcl.GET("anothers.html"));

    s.run_for(60);
    s.kill();

    s.show("10.0.0.789");

    for (auto msg : lr) {
        std::cout << msg;
    }

    return 0;
}
```

# To-do/status
- threading model still needs work
- finish msg container type
- thread pool/task queue over individual client thread inits
- LocalClient methods (merge with interp syntax)
