#ifndef CONFIG_H
#define CONFIG_H


#define LOCAL_HOST 1      // explicitly set host IP to 127.0.0.1 
#define INTERP_MODE 1     // run in interpreter mode - shell and command behavior
#define AUTOLOG 0         // periodically auto log client db out to file
#define TERMINAL_OUT 0    // dump msgs to stdout as requests are recieved and filled
#define SET_TCP 1     
#define SET_UDP 0
#define LOGFILE "server_log.txt"
#define HOMEPAGE "index.html"
#define ROOT "/home/xzxthagod/server"

#define INTERP_PROMPT "serv~$ "
#define BANNED_IP_MSG "u are banned lol\n"


#define QUEUE_SIZE 100
#define MAX_CONNECTIONS 100
#define DEFAULT_LOG_INTERVAL 30



#endif /* CONFIG_H */
