#include "server.h"


using namespace http;


Server::Server(char** argv)
{
    if (opendir(ROOT) == 0) {
		bail("bad root directory");
	}

    this->port = atoi(argv[1]);
	memset(&server_info, 0, sizeof(server_info));
	server_info.sin_family = AF_INET;

	if (LOCAL_HOST) {
        server_info.sin_port = inet_addr("127.0.0.1");
    } else {
        server_info.sin_addr.s_addr = INADDR_ANY;
    }
        
	server_info.sin_port = htons(port);

	#if AUTOLOG
	auto_update_log();
	#endif

	LOG("* server ready to go() *");
}


Server::~Server()
{
	close(server_socket);
}


bool Server::setup()
{
	socklen_t server_len = sizeof(server_info);
	auto* server_ptr = (sockaddr*)&server_info;

	#ifdef SET_TCP
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		bail("failed to establish socket");
	}

	#elif SET_UDP
	if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		bail("failed to establish socket");
	}
	#endif 

	if (bind(server_socket, server_ptr, sizeof(server_info)) < 0) {
		bail("failed to bind");
	}
	
	SETUP = true;

	if (listen(server_socket, QUEUE_SIZE) == -1) {
		bail("failed to listen");
	}
		
	printf("* server running on port %i *\n\n", this->port);
	RUNNING = true;

	#if INTERP_MODE
	interp_loop();
	#endif

	return true;
} 


bool Server::connections_allowed() const
{
	return (active_clients < MAX_CONNECTIONS and RUNNING) ? true : false;
}


void Server::ban(const char* ip_to_ban)
{
	this->IP_banlist.emplace_back(std::string(ip_to_ban));

	#if TERMINAL_OUT
	std::cout << "BANNED IP: " << IP_banlist.back() << "\n\n";
	#endif
}


void Server::ban(const std::string& IP)
{
	this->IP_banlist.emplace_back(std::string(IP));

	std::string log = "BANNED IP: " + IP;
	interp_LOG(log);
}


void Server::interp_LOG(const std::string& msg)
{
	std::lock_guard<std::mutex> gaurd(server_lock);
	std::cout << "\n\n" << msg << "\n\n";
}


const std::vector<std::string>& Server::return_IP_blacklist() const
{
	return IP_banlist;
}


void Server::print_IP_banlist() const
{
	std::cout << "IP BAN LIST:\n";
	for (auto ip : this->IP_banlist) {
        std::cout << ip << '\n';
    }
    std::cout << "\n\n";
}


void Server::set_host_IP(const char* host)
{
	this->host_IP = std::string(host);
}


bool Server::is_client_allowed(const char* client_IP)
{
	bool allowed = true;
	std::string IP(client_IP);
    std::lock_guard<std::mutex> lock(server_lock);

	for (auto ip : this->IP_banlist) {
        if (ip == IP)
            allowed = false;
    }

	return allowed;
}


bool Server::is_client_allowed(const std::string& client_IP)
{
	bool allowed = true;
	std::lock_guard<std::mutex> lock(server_lock);

	for (auto ip : this->IP_banlist) {
        if (ip == client_IP)
            allowed = false;
    }

	return allowed;
}


void Server::main_accept_loop()
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
    }
}


void Server::serve(const Request& request, int client_socket)
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


int Server::search(const std::string& path)
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


void Server::deny(const std::string& IP)
{
    // return page or just text?
}


void Server::kill()
{
	RUNNING = false;
	close(server_socket);
	LOG("* killed *");
}


void Server::auto_update_log()
{
	// rework
}


void Server::update()
{
	std::ofstream file;
	file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!file) {
		file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

	file << "- LOGFILE UPDATE -\n\n";
	for (auto req : this->request_DB) {
        file << req;
    }
}


bool Server::validate_client(const std::string& IP)
{
	for (auto ip : this->IP_banlist) {
        if (ip == IP) {
            return false;
        }
    }

    return true;
}


void http::bail(const char* err_msg)
{
	fprintf(stderr, "server error: %s\n", err_msg);
	exit(EXIT_FAILURE);
}


void Server::go()
{
	setup();
	if (RUNNING)
	{
		std::thread main_loop([this]()->void{main_accept_loop();});
		main_loop.join();
	}
}


void Server::run_for(int minutes_to_run) const
{
	sleep(minutes_to_run * 60);
}


void Server::run_for(float seconds_to_run) const
{
	sleep(seconds_to_run);
}


void Server::send_request(const char* request)
{
	// std::string new_requst(request);
}


void Server::send_request(std::string)
{

}


void Server::LOG(const char* msg) noexcept
{
	std::lock_guard<std::mutex> gaurd(server_lock);
	std::cout << msg << '\n';
}


void Server::interp_loop()
{
	std::thread interp([this]() -> void {
		// std::lock_guard<std::mutex> gaurd(this->server_lock);
		while (RUNNING)
		{
			std::string entry;
			std::cout << INTERP_PROMPT;
			std::getline(std::cin, entry);
			interpret(std::stringstream(entry));
		}
	});
	interp.detach();
}


void Server::interpret(std::stringstream&& entry) 
{
	std::string cmd;
	std::string arg;

	entry >> cmd;
	entry >> arg;

	cmd == "ban"   ? ban(arg)     : 
	cmd == "allow" ? allow(arg)   :
	cmd == "show"  ? show(arg)    :
	cmd == "kill"  ? kill()       :
	cmd == ""      ? (void)[](){} : 
	LOG("invalid command!");
}


void Server::allow(const std::string& IP)
{
	bool hit = false;
	std::vector<std::string>::iterator i = this->IP_banlist.begin();
	std::vector<std::string>::iterator end = this->IP_banlist.end();

	for (i; !hit; ) {
    	if (*i == IP) {
			this->IP_banlist.erase(i);
			LOG("* IP allowed *");
			hit = true;
		} else { ++i; }
	} 
	if (!hit) LOG("IP already allowed");
}


void Server::show(const std::string& arg)
{
	if (arg == "banlist") {
		print_IP_banlist();
	} 
	else if (isdigit(arg[0])) 
	{
		std::lock_guard<std::mutex> lock(server_lock);
		std::cout << "\n-- data log for client [ " << arg << " ] --\n";

		if (request_DB.size() == 0) {
			std::cout << "* no connections this runtime *\n";
		}

        for (auto req : request_DB) {
            if (arg == req.get_client())
                std::cout << req;
        }

		std::cout << "-- end data log --\n\n";

	} else if (arg.size() == 0) {
		LOG("nothing to show!");
	} else {
		LOG("invalid arg!");
	}
}


void Server::show(const char* _IP)
{
	std::string IP(_IP);
	std::lock_guard<std::mutex> lock(server_lock);
	std::cout << "-- data log for client [ " << IP << " ] --\n";

	if (request_DB.size() == 0) {
		std::cout << "* no sessions this runtime *\n";
	}

	for (auto req : request_DB) {
            if (IP == req.get_client())
                std::cout << req;
        }

	std::cout << "-- end data log --\n\n";
}
