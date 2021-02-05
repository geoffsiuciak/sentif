#include "server.h"


using namespace http;


Server::Server(int argc, char** argv)
{
	this->port = atoi(argv[1]);
	this->root = std::string(argv[2]);

	init_server_info();
	init_thread_pool();

	#ifdef AUTOLOG
	std::thread logger([this]()->void{while (RUNNING) auto_update_log();});
	logger.detach();
	#endif

	LOG("* server ready to go() *");
}


Server::Server(int argv_1, std::string argv_2) : port(argv_1), root(argv_2)
{
	// copy over
}


Server::~Server()
{
	close(server_socket);
}


void Server::init_server_info()
{
	if (opendir(root.c_str()) == 0) {
		bail("bad root directory");
	}

	memset(&server_info, 0, sizeof(server_info));
	server_info.sin_family = AF_INET;

	#ifdef LOCAL_HOST
	server_info.sin_port = inet_addr("127.0.0.1");
	#elif 
	server_info.sin_addr.s_addr = INADDR_ANY;
	#endif

	server_info.sin_port = htons(port);
	CALLED = true;
}


void Server::init_thread_pool()
{
	/*for (int i = 0; i < __DEFAULT_INIT_THREAD_COUNT; ++i)
	{
		thread_queue.emplace(new std::thread);
	}*/
}


void Server::launch_thread_manager()
{
	init_thread_pool();

	std::thread thread_manager(
		[&](){
			while (RUNNING)
			{

			}
		}
	);
	
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

	#ifdef INTERP_MODE
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
	std::cout << "BANNED IP: " << IP_banlist.back() << "\n\n";
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
	std::for_each(IP_banlist.begin(), IP_banlist.end(),
		[&](std::string IP) {
			std::cout << IP << '\n';
		}
	);
	
	printf("\n");
}


void Server::set_host_IP(const char* host)
{
	this->host_IP = std::string(host);
}


bool Server::is_client_allowed(const char* client_IP)
{
	bool allowed = true;
	std::string this_client(client_IP);
	std::lock_guard<std::mutex> lock(server_lock);

	std::for_each(IP_banlist.begin(), IP_banlist.end(),
		[&](const std::string& banned_IP) {
			if (this_client == banned_IP)
				allowed = false;
		});

	return allowed;
}


bool Server::is_client_allowed(const std::string& client_IP)
{
	bool allowed = true;
	std::lock_guard<std::mutex> lock(server_lock);

	std::for_each(IP_banlist.begin(), IP_banlist.end(),
		[&](const std::string& banned_IP) {
			if (client_IP == banned_IP)
				allowed = false;
		});

	return allowed;
}


void Server::main_accept_loop()
{
	int client_socket{};
	sockaddr_in client_info{};
	auto* client_info_ptr = (sockaddr*)&client_info;
	socklen_t client_size = sizeof(client_info);
	
	while ((client_socket = accept(server_socket,
		                           client_info_ptr,
		                           &client_size)) != -1
		&& connections_allowed())
	{
		std::thread client_thread([=]()->void
			{
				std::shared_ptr<Client> client_ptr =
					std::make_shared<Client>(client_socket, client_info);

				if (validate_client(client_ptr)) {
					std::unique_lock<std::mutex> db_guard(server_lock);
					client_DB.push_back(client_ptr);
					db_guard.unlock();

					std::shared_ptr<Client> RUN_client_ptr = client_ptr;
					RUN_client_ptr->main_loop();
				}
				else { client_ptr->deny(); }
			}
		);
		client_thread.detach();
	}
}


void Server::kill()
{
	RUNNING = false;
	close(server_socket);
	LOG("* killed *");
}


void Server::auto_update_log()
{
	std::ofstream file;
	file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!file) {
		file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

	file << "- LOGFILE UPDATE -\n\n";
	for (int i = this->log_entries; i < this->client_DB.size(); ++i) {
		// file << this->client_DB[i]->log_data__();
		file << "client data here!!!\n\n";
		++this->log_entries;
	}

	sleep(DEFAULT_LOG_INTERVAL);
}


void Server::update()
{
	std::ofstream file;
	file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!file) {
		file.open(LOGFILE, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

	file << "- LOGFILE UPDATE -\n\n";
	for (auto client : this->client_DB) {
		// file << this->client_DB[i]->log_data__();
		file << "client data here!!!\n\n";
	}
}


bool Server::validate_client(std::shared_ptr<Client> client_ptr)
{
	bool client_allowed = true;
	std::for_each(IP_banlist.begin(), IP_banlist.end(),
		[&](std::string& banned_IP) {
			if (banned_IP == client_ptr->get_IP()) {
				std::lock_guard<std::mutex> gaurd(server_lock);
				client_allowed = false;
				++active_clients;
			}
		}
	);

	return client_allowed;
}


bool http::valid(const int argc, const char** argv) 
{
	if (argc != 3) 
		bail("invalid argc");

	/*if (atoi(argv[1]) <= 1024) 
		bail("invalid port");*/

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
	if (CALLED && RUNNING)
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
	else if (is_IP(arg)) 
	{
		std::lock_guard<std::mutex> lock(server_lock);
		std::cout << "-- data log for client [ " << arg << " ] --\n";

		if (client_DB.size() == 0) {
			std::cout << "* no sessions this runtime *\n";
		}

		std::for_each(client_DB.begin(), client_DB.end(),
			[&](std::shared_ptr<Client> client) {
				if (arg == client->get_IP()) {
					client->log_data__();
				}
			});

		std::cout << "-- end data log --\n\n";

	} else if (arg.size() == 0) {
		LOG("nothing to show!");
	} else {
		LOG("invalid arg!");
	}
}


bool Server::is_IP(const std::string& IP)
{
	// build this out
	return isdigit(IP[0]);
}


void Server::show(const char* _IP)
{
	std::string IP(_IP);
	std::lock_guard<std::mutex> lock(server_lock);
	std::cout << "-- data log for client [ " << IP << " ] --\n";

	if (client_DB.size() == 0) {
		std::cout << "* no sessions this runtime *\n";
	}

	std::for_each(client_DB.begin(), client_DB.end(),
		[&](std::shared_ptr<Client> client) {
			if (IP == client->get_IP()) {
				client->log_data__();
			}
		});

	std::cout << "-- end data log --\n\n";
}
