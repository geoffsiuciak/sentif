# sentif
friendly API for operating web servers on unix systems

sentif offers two modes of operation:
1. cl interface
- run a simple or secure HTTP web or media server,
- using TCP or UDP,
- and interact/manage using the provided commands
2. in another program
- pull any of the tools from the sentif namespace into your project

build:
```
make
```

use the cl-interface:
```
./sentif -n my_server -r $(pwd) -p 5555
```
flags:
```
-r : set root directory
-p : set port
-t : use TCP
-u : use UDP
-s : use OpenSSL
-b : run in background
-d : disable outlogging (requires -b)
```

or use in another project:
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


to-do
- reworking inheritances
- main with getopt for cl usage