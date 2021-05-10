#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "config.h"
#include "sockio.h"

namespace sentif {

class WebServer;

class Admin {
    friend class WebServer;

protected:
    Admin();
    void set_pw(int);
    void login();
    void go(int);
    void stats();
    // void kill();
    void exit(const char *);

private:
    int FD;
    bool NEED_PW;
    bool ONLINE;
    bool MSG_READY;
    std::string admin_name;
    std::string admin_password;
};

} // namespace sentif

#endif
