#ifndef SENTIF_H
#define SENTIF_H

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <netinet/ip.h>

#include "config.h"
#include "types.h"
#include "server_base.h"
#include "web_server.h"
// #include "admin.h"

ctx_t* new_opt_ctx();

void parse_flags(ctx_t *op, int argc, char * const argv[]);

void val_ctx(ctx_t *op);

void to_the_moon(const ctx_t *op);

void _(const char* msg);

void bail_(const char* msg);

void display_(const char* file);


#endif // SENTIF_H
