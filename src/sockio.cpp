#include "sockio.h"

namespace sentif {

void write_file(int target_fd, int file_fd)
{
    char buffer[IO_BUFSIZE];
    int bytes;
        
    while ((bytes = read(file_fd, buffer, IO_BUFSIZE-1)) > 0)
        write(target_fd, buffer, bytes);
}

void write_file(int target_fd, const char* filename)
{
    if ((access(filename, R_OK) == 0)) 
    {
        char buffer[IO_BUFSIZE];
        int bytes;
        int wrfd = open(filename, O_RDONLY);
        
        while ((bytes = read(wrfd, buffer, IO_BUFSIZE-1)) > 0)
            write(target_fd, buffer, bytes);
    } 
    else
        printf("failed to write file to target\n");
}

void write_msg(int target_fd, const char* msg)
{
    if ((write(target_fd, msg, strlen(msg))) < 0) 
        printf("failed to write msg to target\n");
}

std::string get_msg(int socket)
{
    char buffer[IO_BUFSIZE/2];
    memset(buffer, '\0', IO_BUFSIZE/2);
    read(socket, buffer, IO_BUFSIZE / 2);
    auto msg = std::string(buffer);
    msg.erase(msg.end()-2, msg.end());
    return msg;
}

std::string get_packet(int socket)
{
    char buffer[IO_BUFSIZE*2];
    memset(buffer, '\0', IO_BUFSIZE*2);
    read(socket, buffer, IO_BUFSIZE*2);
    auto msg = std::string(buffer);
    // msg.erase(msg.end()-2, msg.end());
    return msg;
}


} // namespace sentif
