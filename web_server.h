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

#include <algorithm>
#include <iostream>
#include <string>

#include "server_base.h"
#include "http_codes.h"
#include "client.h"
#include "request.h"
#include "response.h"
#include "message.h"

#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

namespace sentif {

void bail(const char*);

class WebServerTCP : public ServerBase {
    
    public:
        WebServer(int port, char* root_dir);
        ~WebServer();

    private:
        bool validate_client(const std::string&);
        void main_accept_loop();
        void serve(const Request &, int);
        int search(const std::string&);

    private:
        std::string root_dir;
        std::vector<std::string> IP_banlist;
        std::vector<Request> request_DB;
};


} // namespace sentif
#endif // WEB_SERVER_H