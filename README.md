```
                _   _  __ 
               | | (_)/ _|
 ___  ___ _ __ | |_ _| |_ 
/ __|/ _ \ '_ \| __| |  _|
\__ \  __/ | | | |_| | |  
|___/\___|_| |_|\__|_|_|

-p : port
-r : root
-t : TCP 
-u : UDP
-m : multithread
-i : interaction 
-b : background
-l : logging
-o : logfile name
-h : help
-v : help verbose

```

or embed:
```
#include <iostream>
#include "web_server.h"

int main()
{
    int port = 5555;
    std::string root_dir = "your/path/here/";

    sentif::WebServer s(root_dir, port, TCP);
    s.go();

    s.set_host_IP("10.0.0.175");
    s.ban("10.0.0.123");
    s.ban("10.0.0.456");
    s.print_IP_banlist();

    http::LocalClient lcl;
    std::vector<http::Message> lr;

    lr.push_back(lcl.GET("index.html"));
    lr.push_back(lcl.GET("this_page.html"));
    lr.push_back(lcl.GET("another.html"));

    for (auto msg : lr) {
        std::cout << msg;
    }

    s.show("10.0.0.789");
    s.show("10.0.0.789");

    /* any other code */

    return 0;
}
```
