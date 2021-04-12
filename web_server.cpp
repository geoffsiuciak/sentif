#include "web_server.h"


namespace sentif {


WebServer::WebServer(int port, char* root_dir, int protocol) : ServerBase(port);
{
    this->root_dir = std::string(root_dir);

	auto* server_ptr = (sockaddr*)&server_info;

    if (protocol == TCP or protocol == UDP) 
    {
        if ((server_socket = socket(AF_INET, protocol, 0)) < 0) 
		    bail("failed to establish socket");
    }
    else { bail("invalid socket protocol"); }
    
	if (bind(server_socket, server_ptr, sizeof(server_info)) < 0)
		bail("failed to bind");

	if (listen(server_socket, QUEUE_SIZE) == -1)
		bail("failed to listen");
		
	std::cout << "* web server running on port %i *\n\n" 
              << this->port << '\n';
}


WebServer::~WebServer()
{

}


void WebServer::main_accept_loop()
{
    int ID{};
    int client_socket{};
	sockaddr_in client_info{};
	auto* client_info_ptr = (sockaddr*)&client_info;
	socklen_t client_size = sizeof(client_info);
	
	while ((client_socket = 
        accept(server_socket, client_info_ptr, &client_size)) != -1) 
    {
        std::thread request_thread([=]()->void 
        {
            Request request(client_info.sin_addr, ID);
            std::string IP = request.get_client();

            if (!validate_client(IP)) { deny(IP); }
            else {
                int msg_len = 0;
                char buffer[BUFFER_SIZE];

                if (msg_len = read(client_socket, buffer, BUFFER_SIZE - 1) < 0) {
                    // read error
                } else {
                    buffer[msg_len + 1] = '\0';
                    request.set_request(buffer);
                    serve(request, client_socket);
                    this->request_DB.push_back(std::move(request));
                }
            }
            close(client_socket);
        });
        request_thread.detach();
        ++ID;

        #ifdef RUN_FOR
        // lifetime decrements here
    }
}


void WebServer::serve(const Request& request, int client_socket)
{
    if (request.get_method() == "GET")
    {
        int file_fd = search(request.get_path());
        if (file_fd != -1) {

            int bytes_read = 1;
            char buffer[10];

            while (bytes_read > 0)
            {
                bytes_read = read(file_fd, buffer, 10);
                write(client_socket, buffer, bytes_read);
            }
        }
        close(file_fd);
    }    
}


int WebServer::search(const std::string& path)
{
	DIR* root_ptr = opendir(ROOT);
	struct dirent* entry = nullptr;
	int target_fd = 0;

	while ((entry = readdir(root_ptr)) != NULL && target_fd == 0) 
	{
		if (entry->d_name == path) {
			if (entry->d_type == DT_REG) 
			{
				std::string full_path = ROOT + '/' + path;
				const char* c_path = full_path.c_str();
				target_fd = open(c_path, O_RDONLY, 0644);
			}
			else if (entry->d_type == DT_DIR) {
				target_fd = -2;  // flag for now
			}
		}
	}
	
	// std::cout << target_fd << '\n';
	return target_fd;
}

} // namespace sentif
