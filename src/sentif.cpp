#include "../include/sentif.h"
  
int main(int argc, char *argv[]) 
{
    int flag;
	sentif_options_t opt;
	
    while((flag = getopt(argc, argv, opt_flags)) != -1) 
    { 
        switch(flag) 
        { 
            case SET_PORT:
				if (port_(optarg)) opt.port = atoi(optarg);
				else init_bail("bad port");
				break;
			case SET_ROOT_DIR:
				opt.root_dir = optarg; 
            case SET_PROTO_TCP:
				if (opt.proto == -1) opt.proto = TCP;
				else init_bail("only 1 proto flag allowed");
				break;
            case SET_PROTO_UDP: 
				if (opt.proto == -1) opt.proto = UDP;
				else init_bail("only 1 proto flag allowed");
                break; 
            case SET_INTERACTIVE: 
                opt.interact = true; 
                break; 
            case SET_BACKGROUND:
				opt.background = true;
				break;
			case SET_LOGGING:
				opt.logging = true;
				break;
			case SET_LOGFILE:
				opt.logfile = optarg;
				break;
			case SHOW_HELP:
				show_help();
				break;
			default:
				exit(EXIT_FAILURE); 
        } 
    } 
            
    return 0;
}
