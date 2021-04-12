#ifndef SERVER_BASE_H
#define SERVER_BASE_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <algorithm>
#include <iostream>
#include <string>


#include "http_codes.h"
#include "client.h"
#include "request.h"
#include "response.h"
#include "local_client.h"
#include "message.h"
#include "config.h"


namespace sentif {

void bail(const char*);

class ServerBase {
    protected:
        ServerBase(int port);
        ~ServerBase();
       
        void ban(const char*);
        void ban(const std::string&);
        void print_IP_banlist() const;
        void set_host_IP(const char *);
        void LOG(const char *msg) noexcept;
        void kill();

        bool is_client_allowed(const char*);
        bool is_client_allowed(const std::string&);
        const std::string& get_host_IP() const { return this->host_IP; }
        const std::vector<std::string>& return_IP_blacklist() const;

        void deny(const std::string&);

    private:
        bool validate_client(const std::string&);
        virtual void main_accept_loop() = 0;
        virtual void serve(const Request &, int);
        int search(const std::string&);

    private:
        int port;
        int server_socket{};
        std::string host_IP;
        sockaddr_in server_info;
};

} // namespace sentif

#endif // SERVER_BASE_H
