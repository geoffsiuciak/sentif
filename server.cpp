#include "server.h"


using namespace http;


Server::Server(int argc, char** argv)
{
	this->port = atoi(argv[1]);
	this->root = std::string(argv[2]);

	init_server_info();
	init_thread_pool();

	LOG(1, "* server ready to go() *\n");
}


Server::Server(int argv_1, std::string argv_2) : port(argv_1), root(argv_2)
{
	init_server_info();
	init_thread_pool();
	LOG(1, "* server ready to go() *\n");
}


Server::~Server()
{
	// dump_client_DB();
	close(server_socket);
}


void Server::init_server_info()
{
	if (opendir(root.c_str()) == 0) {
		bail("bad root directory");
	}

	memset(&server_info, 0, sizeof(server_info));
	server_info.sin_family = AF_INET;

	if (LOCAL_HOST == 1) {
		server_info.sin_port = inet_addr("127.0.0.1");
	}
	else {
		server_info.sin_addr.s_addr = INADDR_ANY;
	}

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

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		bail("failed to establish socket");
	}
		
	if (bind(server_socket, server_ptr, sizeof(server_info)) < 0) {
		bail("failed to bind");
	}
	
	SETUP = true;

	if (listen(server_socket, QUEUE_SIZE) == -1) {
		bail("failed to listen");
	}
		
	printf("* server running on port %i *\n\n", this->port);
	RUNNING = true;

	return true;
} 


bool Server::connections_allowed() const
{
	return (active_clients < MAX_CONNECTIONS
		    and RUNNING) ? true : false;
}


void Server::ban_IP(const char* ip_to_ban)
{
	std::lock_guard<std::mutex> gaurd(server_lock);
	this->IP_banlist.emplace_back(std::string(ip_to_ban));
	std::cout << "BANNED IP: " << IP_banlist.back() << "\n\n";
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


void Server::print_client_data_log(const char* client_IP)
{
	std::lock_guard<std::mutex> lock(server_lock);
	std::cout << "-- data log for client [ " << client_IP << " ] --\n";

	if (client_DB.size() == 0) {
		std::cout << "* no sessions this runtime *\n";
	}

	std::for_each(client_DB.begin(), client_DB.end(),
		[&](std::shared_ptr<Client> client) {
			if (strcmp(client_IP, (client->get_IP()).c_str()) != -1) {
				client->log_data__();
			}
		});

	std::cout << "-- end data log --\n\n";
}


void Server::print_client_data_log(std::string client_IP)
{

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
	std::lock_guard<std::mutex> gaurd(server_lock);

	RUNNING = false;
	close(server_socket);
	// log_client_DB_to_file

	printf("* killed *\n");
}


void Server::launch_client_DB_logger()
{
	int entries_logged{};
	auto logger = [&]()
	{
		while (RUNNING)
		{
			std::lock_guard<std::mutex> guard(server_lock);
			std::ofstream client_DB_file;
			client_DB_file.open("client_DB.txt", std::fstream::in | std::fstream::out | std::fstream::app);

			if (!client_DB_file) {
				client_DB_file.open("client_DB.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
				client_DB_file << "- session client_DB log -\n\n";
				client_DB_file.close();
			}
			else {
				client_DB_log_printer(client_DB_file, entries_logged);
				client_DB_file.close();
			}

			client_DB_file.close();
			sleep(DEFAULT_LOG_INTERVAL);
		}
	};

	std::thread log_to_file(logger);
	log_to_file.detach();
}


void Server::client_DB_log_printer(std::ofstream& file_stream, int& entries_logged)
{
	std::lock_guard<std::mutex> guard(server_lock);
	int entries_to_log = client_DB.size() - entries_logged;



	for (int i = 0; i < entries_to_log; ++i) {
		file_stream << client_DB[entries_logged + 1];
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
		main_loop.detach();
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


void Server::LOG(int stream, const char* msg) noexcept
{
	std::lock_guard<std::mutex> gaurd(server_lock);
	write(stream, msg, strlen(msg));
}


void Server::DEBUG()
{
	printf("DEBUG\n\n");
	/*std::lock_guard<std::mutex> guard(server_lock);
	std::cout << "*** debug dump ***\n";

	std::for_each(client_DB.begin(), client_DB.end(),
		[](std::shared_ptr<Client> client) {
			client->client_end_log();
		});*/
}
