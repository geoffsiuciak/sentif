#ifndef CONFIG_H
#define CONFIG_H

#define FLAGS        "p:a:r:tubdo:e:shv"
#define HELP         "./txt/help.txt"
#define HELP_VERBOSE "./txt/help_verbose.txt"
#define LOGOUT       "./txt/server_log.txt"
#define ERROUT       "./txt/error_log.txt"
#define DENY         "./pages/deny.html"
#define LOGO         "./txt/logo.txt"
#define ADMIN_PROMPT "~$ "

#define CLIENT             0
#define ADMIN              1
#define INVALID_SOCKET    -1
#define SENTIF_PROPER      0 
#define SENTIF_ERROR       1
#define SENTIF_BAIL        2
#define MAX_LISTEN         100
#define BUFFER_S           256
#define BUFFER_L           1026

#define TCP    SOCK_STREAM
#define UDP    SOCK_DGRAM 
#define DOMAIN AF_INET


#endif // CONFIG_H
