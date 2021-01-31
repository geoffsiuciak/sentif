# http_server
C++ classes for quickly implementing a multithreaded HTTP/\*nix socket server
```
// basic usage

#include <iostream>
#include "server.h"

// using namespace http;

int main(int argc, char** argv)
{
    http::Server s(argc, argv);
    s.go();

    /* perform any action while server is running */
    s.ban_IP("10.0.0.123");
    // s.ban_IP("127.0.0.1");
    s.print_IP_banlist();
    s.set_host_IP("10.0.0.175");

    http::LocalClient lcl;
    std::vector<http::Message> lr;

    lr.push_back(lcl.GET("index.html"));
    lr.push_back(lcl.GET("this_page.html"));
    lr.push_back(lcl.GET("anothers.html"));

    s.run_for(60);
    s.kill();

    s.print_client_data_log("10.0.0.789");

    for (http::Message msg : lr) {
        std::cout << msg;
    }

    return 0;
}
```

# to-do/status
- need generic message container type
- thread pool/task queue over individual client thread inits
