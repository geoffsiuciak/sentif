# sentif
friendly API for operating and interacting with web servers on unix systems

# features
- recieve from other processes
- C++ api
- command line interface
- easily configurable
- multithreaded

cl usage
```
./sentif server1 $(pwd) -p 5555 -b 
```

embedded usage:
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
