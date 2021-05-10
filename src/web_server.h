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
#include <ostream>
#include <string>
#include <thread>
#include <mutex>

#include "http.h"
#include "server_base.h"


#define NL '\n'

// typedef struct stats {
//     int* request_count;
//     std::map<std::string, std::vector<Request>>* hi
// } stats_t;

namespace sentif {

    class Admin;

    class WebServer : public ServerBase
    {
        friend class Admin;

    public:
        WebServer(ctx_t settings);
        ~WebServer();
        void go();
        void kill() { RUNNING = false; }

    private:
        int which_accept(int[], int *, struct sockaddr_in *, socklen_t *);
        void handle_request(int);
        void error_response(int, int);
        void welcome_response(int, const std::string &);
        void terminal_log(const std::string &, const std::string &);
        int search(const std::string &);

    private:
        std::mutex mtx;
        int requests;
        Admin admin;
        bool RUNNING;
        // std::map<std::string, std::vector<Request>> history;
};

} // namespace sentif

#endif // WEB_SERVER_H