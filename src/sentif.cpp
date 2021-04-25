#include "../include/sentif.h"
  
int main(int argc, char *argv[]) 
{
    int flag;
	sentif_options_t opt;
	
    while((flag = getopt(argc, argv, FLAGS)) != -1) 
    { 
        switch(flag) 
        { 
            case PORT:
				opt.port = atoi(optarg);
				break;
			case ROOT:
				opt.root_dir = optarg; 
            case PROTO_TCP:
				if (opt.proto == -1) opt.proto = TCP;
				else init_bail("only 1 proto flag allowed");
				break;
            case PROTO_UDP: 
				if (opt.proto == -1) opt.proto = UDP;
				else init_bail("only 1 proto flag allowed");
                break; 
            case INTERACTIVE: 
                opt.interact = true; 
                break; 
            case BACKGROUND:
				opt.background = true;
				break;
			case LOGGING:
				opt.logging = true;
				break;
			case LOGFILE:
				opt.logfile = optarg;
				break;
			case HELP:
				show_help(HELP_FILE);
				break;
			case HELP_VERBOSE:
				show_help(HELP_FILE_VERBOSE);
				break;
			default:
				exit(SENTIF_BAIL); 
        } 
    } 
            
	sentif_init(&opt);

    return 0;
}

void sentif_init(const sentif_options_t *op)
{
	// 1. check port
    int sockfd;
    sockaddr_in serv_addr;
     
	if (op->port == -1) init_bail("missing port");

    sockfd = socket(AF_INET, op->proto, 0);
    if (sockfd < 0) { init_bail("failed to open test socket"); }
  
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(op->port);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        init_bail("port is not avaiable");
	}
 
    close(sockfd);
	
	// 2. check protocol
	if (op->proto == -1) init_bail("missing protocol");
    
	// 3. check root directory
	if (!op->root_dir) {
		char cwd[256];
		if (!getcwd(cwd, 256)) {
			init_bail("failed to get name of cwd");
		} else {
			strcpy(op->root_dir, cwd);
		}
	}
	
	if (!(access(op->root_dir, R_OK) == 0 && access(op->root_dir, W_OK) == 0)) {
		init_bail("you don't have R/W priviledges for this root");
	}

	// check/setup logfile
	if (!op->logfile) { strcpy(op->logfile, DEFAULT_LOGFILE); }

	if (!(access(op->logfile, F_OK) == 0)) {
		if (!(creat(op->logfile, O_CREAT) > -1))
			init_bail("failed to init logfile");
	}
}

void show_help(const char* file)
{
    char buffer[1024];
    int bytes;
    int fd = open(file, O_RDONLY);
    
	while ((bytes = read(fd, buffer, 1024)) > 0)
        write(1, buffer, bytes);

	exit(SENTIF_PROPER);
}

void init_bail(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
	printf("use flags -h or -v for help\n\n");
    exit(SENTIF_BAIL);
}
