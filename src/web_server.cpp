#include "web_server.h"


namespace sentif {


WebServer::WebServer(ctx_t settings) : ServerBase(settings)
{
	auto* server_ptr = (sockaddr*)&server_info;

    if ((server_socket = socket(DOMAIN, settings.protocol, 0)) < 0) 
        bail("failed to establish server socket");
    
	if (bind(server_socket, server_ptr, sizeof(server_info)) < 0)
		bail("failed to bind with server socket");

	if (listen(server_socket, MAX_LISTEN) == -1)
		bail("failed to listen on server socket");

    if (settings.admin > -1) {
        auto* admin_ptr = (sockaddr*)&admin_info;
        if ((admin_socket = socket(DOMAIN, settings.protocol, 0)) < 0) 
            bail("failed to establish admin socket");
    
        if (bind(admin_socket, admin_ptr, sizeof(admin_info)) < 0)
            bail("failed to bind with admin socket");

        if (listen(admin_socket, 1) == -1)
            bail("failed to listen on admin socket");
    }

    RUNNING = true;
}

WebServer::~WebServer()
{
    close(this->server_socket);
    close(this->admin_socket);
}

int WebServer::which_accept(int fds[],
                            int* c_port,
                            struct sockaddr_in *addr, 
                            socklen_t *addrlen)
{
    fd_set readfds;
    int maxfd, fd;
    unsigned int i;
    int status;

    FD_ZERO(&readfds);
    maxfd = -1;
    for (i = 0; i < 2; i++) {
        FD_SET(fds[i], &readfds);
        if (fds[i] > maxfd)
            maxfd = fds[i];
    }

    status = select(maxfd + 1, &readfds, NULL, NULL, NULL);

    if (status < 0)
        return INVALID_SOCKET;

    fd = INVALID_SOCKET;
    for (i = 0; i < 2; i++)
        if (FD_ISSET(fds[i], &readfds)) {
            fd = fds[i];
            *c_port = i;
            break;
        }

    if (fd == INVALID_SOCKET) return INVALID_SOCKET;
    else                      return accept(fd, (sockaddr *)addr, addrlen);
}

void WebServer::go()
{
    sockaddr_in        c_info;
    auto *c_ptr      = (sockaddr *)&c_info;
    socklen_t c_size = sizeof(c_info);
    int socks[2]     = { server_socket, admin_socket };
	
    while (RUNNING) {
        int c_port = 0;
        int c_sock = which_accept(socks, &c_port, &c_info, &c_size);

        char c_msg[21]{"u are a pleb client\n"};

        bool ad = false;
        if (c_port == CLIENT) {
            write(c_sock, c_msg, strlen(c_msg));
            close(c_sock);
        } else
        if (c_port == ADMIN) {
            std::thread as([this, c_sock]() { 
                if (!admin.ONLINE)
                    admin.go(c_sock);
                else {
                    write_msg(c_sock, "\nadmin already logged on.\nthis will be reported.\n\n");
                    close(c_sock);
                } 
            });
            as.detach();
        } else {
            close(c_sock);
            bail("failed to match incoming socket");
        }
    }
}

// void WebServer::terminal_lognew(bool client_OK, const sockaddr_in*)
// {
//     time_t clk = time(NULL);
//     printf("client %i ", ip.c_str());
//     if (!client_OK) printf(" DENIED ");
//     printf("- %s", ctime(&clk));
// }

int WebServer::search(const std::string& path)
{
	// DIR* root_ptr = opendir(ROOT);
	// struct dirent* entry = nullptr;
	// int target_fd = 0;

	// while ((entry = readdir(root_ptr)) != NULL && target_fd == 0) 
	// {
	// 	if (entry->d_name == path) {
	// 		if (entry->d_type == DT_REG) 
	// 		{
	// 			std::string full_path = ROOT + '/' + path;
	// 			const char* c_path = full_path.c_str();
	// 			target_fd = open(c_path, O_RDONLY, 0644);
	// 		}
	// 		else if (entry->d_type == DT_DIR) {
	// 			target_fd = -2;  // flag for now
	// 		}
	// 	}
	// }
	
	// // std::cout << target_fd << '\n';
	// return target_fd;
    return 0;
}

} // namespace sentif
