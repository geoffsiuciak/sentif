#ifndef SENTIF_H
#define SENTIF_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/ip.h>

#define PORT              'p'
#define ROOT              'r'
#define PROTO_TCP         't'
#define PROTO_UDP         'u'
#define MULTITHREAD       'm'
#define INTERACTIVE       'i'
#define BACKGROUND        'b'
#define LOGGING           'l'
#define LOGFILE           'o'
#define HELP              'h'
#define HELP_VERBOSE      'v'

#define FLAGS             "p:r:tum:iblo:hv"
#define DEFAULT_LOGFILE   "sentif_log.txt"
#define HELP_FILE         "help.txt"
#define HELP_FILE_VERBOSE "help_verbose.txt"

#define init_int          -1
#define init_bool         false
#define init_ptr          nullptr

#define SENTIF_PROPER     0 
#define SENTIF_ERROR      1
#define SENTIF_BAIL       2

#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

typedef struct sentif_options {
	int port		= init_int;
	int proto       = init_int;
	bool append     = init_bool;
	bool logging    = init_bool;
	bool interact   = init_bool;
	bool background = init_bool;
	char* logfile   = init_ptr;
	char* root_dir  = init_ptr;
} sentif_options_t;

void sentif_init(const sentif_options_t *options);
void init_bail(const char* msg);
void show_help(const char* file);

#endif // SENTIF_H
