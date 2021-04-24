#ifndef SENTIF_H
#define SENTIF_H

#include <sys/types.h>
#include <sys/socket.h>

#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

struct options {
	int port;
	int protocol;
	bool secure;
	bool background;
	bool logging;
	char* log_file;

	options() {
		secure = false;
		background = false;
		logging = true;
		log_file = nullptr;
	}
};

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


#endif // SENTIF_H
