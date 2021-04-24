#ifndef CONFIG_H
#define CONFIG_H

#define opt_flags "p:tusbd"

#define LOCAL_HOST      // explicitly set host IP to 127.0.0.1 
#define INTERP_MODE     // run in interpreter mode - shell and command behavior
#define AUTOLOG         // periodically auto log client db out to file
#define TERMINAL_OUT    // dump msgs to stdout as requests are recieved and filled
#define DEFAULT_LOGFILE "server_log.txt"

#define INTERP_PROMPT "serv~$ "
#define DEFAULT_LOG_INTERVAL 30


#endif // CONFIG_H 
