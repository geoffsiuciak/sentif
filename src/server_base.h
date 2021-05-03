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
#include <fcntl.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include "config.h"
#include "types.h"

namespace sentif {

    typedef std::map<std::string, bool> iplist_t;

    void bail(const char *);

    class ServerBase
    {
        friend class Admin;
        friend class WebServer;

    protected:
        ServerBase(ctx_t ctx);
        ~ServerBase();

        void show_banlist() const;
        const iplist_t& get_banlist() { return banned; }
        bool allowed(const char*);
        bool allowed(const std::string&);
        void deny(int);
        void LOG(const char*) noexcept;
        void kill();

    public:
        void ban(const char*);
        void ban(const std::string&);

    private:
        virtual void go() = 0;
        ctx_t settings;
        iplist_t banned;
        int server_socket = 0;
        sockaddr_in server_info;
};

} // namespace sentif

#endif // SERVER_BASE_H
