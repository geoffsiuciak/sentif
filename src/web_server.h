#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "config.h"
#include "types.h"
#include "server_base.h"

namespace sentif {

class WebServer : public ServerBase {
    
    public:
        WebServer(ctx_t settings);
        ~WebServer();
        void go();

    private:
        void log_new(volatile bool, const char*);
        int search(const std::string &);

    private:
        // std::map<std::string, std::vector<Request>> history;
};

} // namespace sentif

#endif // WEB_SERVER_H