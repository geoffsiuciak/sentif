#ifndef CONFIG_H
#define CONFIG_H

#define FLAGS        "p:a:r:tubdo:e:shv"
#define HELP         "./txt/help.txt"
#define HELP_VERBOSE "./txt/help_verbose.txt"
#define LOGOUT       "./txt/server_log.txt"
#define ERROUT       "./txt/error_log.txt"
#define DENY         "./pages/deny.html"
#define ADMIN_PROMPT "~$ "

#define TCP    SOCK_STREAM
#define UDP    SOCK_DGRAM 
#define DOMAIN AF_INET
#define SENTIF_PROPER      0 
#define SENTIF_ERROR       1
#define SENTIF_BAIL        2
#define BUFFER_S           256
#define BUFFER_L           1026

#endif // CONFIG_H
