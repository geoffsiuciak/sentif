#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

#include "config.h"
#include "sentif.h"

  
int main(int argc, char *argv[]) 
{
    int flag;
	options opt;
	
    while((flag = getopt(argc, argv, opt_flags)) != -1) 
    { 
        switch(flag) 
        { 
            case 'p':
				if (port_(optarg))
					opt.port = atoi(optarg);
				else
					init_bail("bad port");
				break; 
            case 't':
				opt.protocol = TCP;
				break;
            case 'u': 
                opt.protocol = UDP;
                break; 
            case 's': 
                opt.secure = true; 
                break; 
            case 'b':
				opt.background = true;
				break;
			case 'd':
				opt.logging = false;
				break;
			case 'l':
				opt.log_file = optarg;
				break; 
        } 
    } 
      
    printf("port: %i\n", opt.port);
    printf("proto: %i\n", opt.protocol);
      
    return 0;
}
