#ifndef SENTIF_H
#define SENTIF_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SET_PORT        'p'
#define SET_PROTO_TCP   't'
#define SET_PROTO_UDP   'u'
#define SET_INTERACTIVE 'i'
#define SET_BACKGROUND  'b'
#define SET_LOGGING     'l'
#define SET_LOGFILE     'o'
#define SET_ROOT_DIR    'r'
#define SHOW_HELP       'h'

#define opt_flags "p:tuiblo:r:h"

#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

typedef struct sentif_options {
	int port        {   -1    };
	int proto       {   -1    };
	bool interact   {  false  };
	bool background {  false  };
	bool logging    {  false  };
	char* logfile   { nullptr };
	char* root_dir  { nullptr };
} sentif_options_t;

void init_bail(const char* msg)
{
	fprintf(stderr, "init_bail: %s.\n", msg);
	exit(EXIT_FAILURE);	
}

void show_help()
{
	const char files[2][9] = { "logo.txt", "help.txt" };
	char *buffer;
	int bytes;
	
	for (int i = 0; i < 2; ++i) 
	{
		buffer = new char[1024];
		int fd = open(files[i], O_RDONLY);

		while ((bytes = read(fd, buffer, 1024)) > 0)
    		write(1, buffer, bytes);
	
		printf("\n");
		close(fd);
	}
	delete[] buffer;
}

bool port_(char* optarg)
{
	// check for perm/avail on port
	return true;
}

#endif // SENTIF_H
