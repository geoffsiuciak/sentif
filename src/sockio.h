#ifndef SOCKIO_H
#define SOCKIO_H

#define IO_BUFSIZE 1024

#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <string>

namespace sentif {

void write_file(int, int);
void write_file(int, const char*);
void write_msg(int, const char*);
std::string get_msg(int);
std::string get_packet(int);

} // namespace sentif

#endif
