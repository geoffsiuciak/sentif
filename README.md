```
                _   _  __ 
               | | (_)/ _|
 ___  ___ _ __ | |_ _| |_ 
/ __|/ _ \ '_ \| __| |  _|
\__ \  __/ | | | |_| | |  
|___/\___|_| |_|\__|_|_|

-p : host
-a : admin
-r : root
-t : TCP
-u : UDP
-b : background
-d : disable log
-o : log file
-e : err file
-s : show err
-h : help
-v : help verbose
```
build:
```
make
```
example:
```
./sentif -p 5555 -a 9999 -t -r $(pwd)
```
or use the classes elsewhere:
```
#include "web_server.h"

int main()
{
    sentif::WebServer s("your/root/here", 5555, TCP);
    s.go();

    /* any other code or method calls */

    return 0;
}
```
