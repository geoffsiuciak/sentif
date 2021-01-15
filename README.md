# http_server
C++ classes for quickly implementing a multithreaded HTTP/\*nix socket server.
- Allowed clients may connect and are spawned into a new thread. Data members in http::Server maintain client request and response history for the lifetime of the server, which may be periodically logged to a file and/or printed. This data management is thread safe and allows any code to run in the main thread.
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
    s.print_IP_banlist();
    s.set_host_IP("10.0.0.175");

    std::cout << "host is: " << s.get_host_IP() << "\n\n";

    http::LocalClient l;
    auto response = l.GET("page_request_here.html");

    s.run_for(60);
    s.kill();

    // more code may run in main thread

    s.print_client_data_log("10.0.0.789);

    return 0;
}
```


# to-do
- tweak request/response management to account for browser behavior
- thread pool/task queue over individual client thread inits
- finish writing, both file contents and packet headers
- finish LocalClient and remaining incomplete methods

Sort of turning into a sandbox project, though I intend to continue building on this. Intended for use on a secure local network, perhaps on a raspberry pi...
