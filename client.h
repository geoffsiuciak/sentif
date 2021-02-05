#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>

#include <algorithm>
#include <exception>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <ostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include <mutex>
#include <map>

#include "http_codes.h"
#include "request.h"
#include "response.h"
#include "config.h"


#define BUFFER_SIZE 256
#define MAX_REQUEST_QUEUE 10;


namespace http
{

	class Client
	{
	public:
		explicit Client(int e, sockaddr_in info);
		void main_loop();
		void log_connection();
		const std::string& get_IP() const;
		int get_count();
		bool new_message();
		bool is_alive();
		void client_end_log();
		int get_request_count() { return this->request_count; }
		int search();
		void serve(Request&, int);
		void deny();
		void process_request();
		void log_data__();
		bool read_from_file(int, char *, int *);
		bool write_to_client(int, char *, int);

	private:
		void generate_default_response();
		void generate_unique_response();
		void generate_error_response(int);
		void LOG(const char*);

		std::mutex client_lock;
		
		int socket_ID;
		std::vector<char> msg_buf;

		sockaddr_in TCP_struct;
		std::string IP4_address;
		std::string session_start_time;

		int request_count;
		double time_alive;
		bool client_alive;

		std::vector<char> read_buffer;

		std::vector<Request> session_requests;
		std::vector<Response> session_responses;
	};
}


#endif
