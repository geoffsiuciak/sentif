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

void show_help()
{
    /* cat help.txt */
    char buffer[1024];
    int bytes;
    int fd = open("help.txt", O_RDONLY);
    while ((bytes = read(fd, buffer, 1024)) > 0)
        write(1, buffer, bytes);
}

void init_bail(const char* msg)
{
    fprintf(stderr, "init_bail: %s.\n", msg);
    exit(EXIT_FAILURE);
}

bool port_(char* optarg)
{
    // check for perm/avail on port
    return true;
}
