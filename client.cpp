#include "client.h"


using namespace http;


Client::Client(int socket_ID, sockaddr_in TCP_struct)
{
	// this->info = std::make_unique<Client_Data>();
	this->socket_ID = socket_ID;
	this->TCP_struct = TCP_struct;

	IP4_address =
		std::string(inet_ntoa(this->TCP_struct.sin_addr));

	this->session_start_time =
		std::string("*session_start_time 12.34*"); // TO-DO

	this->read_buffer.resize(BUFFER_SIZE);
	this->request_count = 0;
	this->time_alive = 0.0;
	this->client_alive = true;
}




/**********************************************************************************/
void Client::main_loop()
{
	log_connection();
	while (client_alive)
	{
		if (new_message()) {
			process_request();
		}
	}
}
/**********************************************************************************/


bool Client::new_message()
{
	int msg_len = 0;
	read_buffer.resize(BUFFER_SIZE);

	try {
		if (msg_len = read(
			socket_ID, read_buffer.data(), BUFFER_SIZE - 1) <= 0) 
			throw std::runtime_error("client quit: ");
	}
	catch (const std::runtime_error& e) {
		client_alive = false;
		std::lock_guard<std::mutex> gaurd(client_lock);
		std::cout << e.what() << IP4_address << "\n\n";
		return false;
	}

	session_requests.emplace_back(Request(read_buffer.data()));

	return true;
}


void Client::o(const char* out)
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << out << '\n';
}


void Client::process_request()
{
	Request& curr_req = session_requests.back();

	if (curr_req.get_method() == "GET" && 
	    curr_req.get_path() == "/" && 
		curr_req.clean()) { 
			generate_default_response(); 
	} 
	else if (curr_req.get_method() == "GET" && 
	         curr_req.get_path()[0] == '/' &&
			 curr_req.get_path().size() > 1 && 
			 curr_req.clean()) { 
			 	generate_unique_response();
	}
	else {
		generate_error_response(BAD_REQUEST);
	}
}


void Client::generate_error_response(int http_code)
{
	o("error");
	session_responses.emplace_back(Response(http_code));
}


bool Client::read_from_file(int file_fd, char* content_buffer, int* file_size) 
{
	int _read = 1;

	while (_read > 0) { 
		if (_read = read(file_fd, (void*)content_buffer, 10) == -1) {
			printf("HERE\n");
			perror("read");
			exit(1);
			return false;
		}
		else { *file_size += _read; }
	}

	return true;
}


void Client::generate_unique_response()
{
	o("unique");

	int file_fd = search();
	if (file_fd > -1) {

		int bytes_read = 1;
		char buffer[10];

		while (bytes_read > 0)
		{
			bytes_read = read(file_fd, buffer, 10);
			write(this->socket_ID, buffer, bytes_read);
		}

		session_responses.emplace_back(Response(OK));
		/* else { generate_error_response(INTERNAL_SERVER_ERROR); }
		   need to come back for sys call error handling
		*/

		close(file_fd);
	}
	else { 
		close(file_fd);
		generate_error_response(NOT_FOUND);
	}
}


bool Client::is_alive()
{
	return client_alive;
}


int Client::get_count()
{
	return request_count;
}


void Client::deny()
{
	int ec{};
	const char buff[19]{ "CONNECTION DENIED\n" };

	write(socket_ID, (const void*)buff, 19);
	client_alive = false;
	close(this->socket_ID);

	std::lock_guard<std::mutex> guard(client_lock);
	std::cout << "banned IP denied: " << IP4_address << "\n\n";
}


int Client::search()
{
	char root[23]{"/home/xzxthagod/server"};
	DIR* root_ptr = opendir(root);
	struct dirent* entry = nullptr;

	auto path = session_requests.back().get_path();
	path.erase(0, 1);
	int target_fd = 0;

	while ((entry = readdir(root_ptr)) != NULL && target_fd == 0) 
	{
		if (entry->d_name == path) {
			if (entry->d_type == DT_REG) 
			{
				std::string full_path = root + '/' + path;
				const char* c_path = full_path.c_str();
				target_fd = open(c_path, O_RDONLY, 0644);
			}
			else if (entry->d_type == DT_DIR) {
				target_fd = -2;  // flag for now
			}
		}
	}

	std::cout << target_fd << '\n';
	return target_fd;
}


void Client::log_connection()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "new client: " << IP4_address << "\n\n";
}


void Client::generate_default_response()
{
	o("nice index");

	// const char* 

	// read_from_file("index.html");
	session_responses.emplace_back(Response(OK, "index.html"));
}


const std::string& Client::get_IP() const
{
	return IP4_address;
}


void Client::client_end_log()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "client log:\n"
		      << "[ " << IP4_address << " ]"
		      << "joined at: " << session_start_time << "\n\n";

	std::for_each(session_requests.begin(), 
			      session_requests.end(),
				 [&](Request request) { std::cout << request; }
	);
}


void Client::log_data__()
{
	/*std::lock_guard<std::mutex> gaurd(client_lock);
	int i = 

	std::cout << "session start: " << session_start_time << "\n";

	std::for_each(session_requests.begin(),
		          session_requests.end(),
		[&]() {
			std::cout << session_requests[i];
			std::cout << session_responses[i];
			++i;
		}
	);*/
}
