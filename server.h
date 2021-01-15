#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <stdint.h>

#include <condition_variable>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <list>
#include <map>

#include "http_codes.h"
#include "client.h"
#include "request.h"
#include "response.h"
#include "local_client.h"

#define __MODE 0
#define __LOCAL_HOST 1
#define __QUEUE_SIZE 100
#define __MAX_CONNECTIONS 100
#define __DEFAULT_PER_MIN 30
#define __DEFAULT_PER_SESSION 100
#define __DEFAULT_HISTORY_SAVED 250
#define __DEFAULT_CLIENTS_SAVED 500
#define __DEFAULT_LOG_INTERVAL 30
#define __DEFAULT_INIT_THREAD_COUNT 25


namespace http
{
	void bail(const char*);
	bool valid(const int, const char**);

	class Server
	{
	public:
		Server(int, char**);
		~Server();
		
		void go();
		void run_for(int) const;
		void run_for(float) const;
		void ban_IP(const char*);
		void print_IP_banlist() const;
		bool connections_allowed() const;
		void set_host_IP(const char*);
		void set_rules(int, int);
		void send_request(const char*);
		void send_request(std::string);
		void LOG(int, const char*) noexcept;
		void kill();

		void print_client_data_log(const char* client_IP);
		void print_client_data_log(std::string client_IP);
		bool is_client_allowed(const char*);
		bool is_client_allowed(const std::string&);
		const std::string& get_host_IP() const { return this->host_IP; }
		const std::vector<std::string>& return_IP_blacklist() const;

		bool log_client_DB_to_file(int);

	private:
		Server(int, std::string);
		bool setup();
		void init_server_info();
		void init_thread_pool();
		void launch_client_DB_logger();
		void launch_thread_manager();
		void client_DB_log_printer(std::ofstream&, int&);
		bool validate_client(std::shared_ptr<Client>);
		void main_accept_loop();

		void DEBUG();

	private:
		int port;
		std::string root;
		std::string host_IP;

		int server_socket{};
		int request_count{};
		int active_clients{};
		bool __CALLED = false;
		bool __SETUP = false;
		bool __RUNNING = false;

		sockaddr_in server_info;
		int _per_minute = __DEFAULT_PER_MIN;
		int _per_session = __DEFAULT_PER_SESSION;
		
		std::mutex server_lock;
		std::condition_variable cond_var;

		std::queue<std::thread> thread_queue;
		std::vector<std::string> IP_whitelist;
		std::vector<std::string> IP_banlist;
		std::vector<std::shared_ptr<Client>> client_DB;
		// dllist<std::shared_ptr<Client>> client_DB;
	};

}


#endif
