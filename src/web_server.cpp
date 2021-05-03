#include "web_server.h"


namespace sentif {


WebServer::WebServer(ctx_t settings) : ServerBase(settings)
{
	auto* server_ptr = (sockaddr*)&server_info;

    if ((server_socket = socket(AF_INET, settings.protocol, 0)) < 0) 
        bail("failed to establish socket");
    
	if (bind(server_socket, server_ptr, sizeof(server_info)) < 0)
		bail("failed to bind");

	if (listen(server_socket, 100) == -1)
		bail("failed to listen");
}

WebServer::~WebServer()
{
    close(this->server_socket);
}

void WebServer::go()
{
    int c_sock = 0;
    sockaddr_in c_info;
    auto *c_ptr = (sockaddr *)&c_info;
    socklen_t c_size = sizeof(c_info);
	
	while ((c_sock = accept(server_socket, c_ptr, &c_size)) != -1) 
    {
        volatile bool banned = false;
        // check_admin();
        auto ip = std::string(inet_ntoa(c_info.sin_addr));
        if (allowed(ip)) { 
            char request[BUFFER_L];
            read(c_sock, request, BUFFER_L-1);
            auto req = std::string(request);
            std::stringstream ss(request);
            


        } else {
            deny(c_sock); 
            banned = true;
        }

        log_new(banned, ip.c_str());
        close(c_sock);
    }
}

void WebServer::log_new(bool banned, const char* ip)
{
    time_t clk = time(NULL);

    if (!banned) {
        printf("client %s - ", ip);
    } else {
        printf("BANNED CLIENT %s DENIED - ", ip);
    }
    
    printf("%s", ctime(&clk));
}

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
