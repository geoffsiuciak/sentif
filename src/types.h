#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>


typedef struct context {
    int   host;
    int   admin;
    int   protocol;
    bool  log_use;
    bool  run_back;
    char* logout;
    char* errout;
    char* root;
} ctx_t;


#endif // TYPES_H
