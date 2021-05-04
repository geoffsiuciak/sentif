#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "http.h"
#include "server_base.h"


#define NL '\n'

namespace sentif {

class WebServer : public ServerBase {
    friend class Admin;

public:
    WebServer(ctx_t settings);
    ~WebServer();
    void go();

private:
    int which_accept(int[], int*, struct sockaddr_in *, socklen_t *);
    void handle_request(int);
    void error_response(int, int);
    // void terminal_lognew(bool client_OK, const std::string& ip);
    int search(const std::string &);

private:
    int requests;
    Admin admin;
    bool RUNNING;
    // std::map<std::string, std::vector<Request>> history;
};

} // namespace sentif

#endif // WEB_SERVER_H