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
#include <sstream>
#include <iostream>
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

#define _GET "GET"
#define _POST "POST"
#define _INDEX "/"
#define __INDEX '/'
#define __ROOT std::string("/home/xzxthagod/server")

#define __BUFFER_SIZE 256
#define __MAX_REQUEST_QUEUE 10;
typedef std::string::iterator string_itr;


namespace http
{

	struct Client_Data
	{
		int socket_ID;
		sockaddr_in TCP_struct;
		std::string IP4_address;
		std::string session_start_time;
		char read_buffer[__BUFFER_SIZE];
		int current_buffer_len;
		int request_count;
		double time_alive;
		bool client_alive;
		std::vector<Request> session_requests;
		std::vector<Response> session_responses;
	};

	class Client
	{
	public:
		explicit Client(int, sockaddr_in);
		void handle();
		void log_connection();
		const std::string& get_IP() const;
		int get_count();
		bool got_buffer();
		char* return_buffer() { return this->client_data->read_buffer; }
		bool is_alive();
		void client_end_log();
		int get_request_count() { return this->client_data->request_count; }
		int search();
		void serve(Request&, int);
		void deny();
		void post_response();
		void process_request();
		void log_data__();
		bool read_from_file(int, const char *, int *);
		bool write_to_client(int, const char *, int);


	private:
		void generate_default_response();
		void generate_unique_response();
		void generate_error_response(int);
		void o(const char*);

		std::mutex client_lock;
		
		int request_count{};
		std::shared_ptr<Client_Data> client_data;
	};
}


#endif
