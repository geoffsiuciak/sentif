#include "client.h"


using namespace http;


Client::Client(int socket_ID, sockaddr_in TCP_struct)
{
	this->info = std::make_shared<info>();
	this->info->socket_ID = socket_ID;
	this->info->TCP_struct = TCP_struct;

	this->info->IP4_address =
		std::string(inet_ntoa(this->info->TCP_struct.sin_addr));

	this->info->session_start_time =
		std::string("*session_start_time 12.34*"); // TO-DO

	this->info->current_buffer_len = 0;
	this->info->request_count = 0;
	this->info->time_alive = 0.0;
	this->info->client_alive = true;
}




/**********************************************************************************/
void Client::main_loop()
{
	log_connection();
	while (info->client_alive)
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
	info->msg_buf.resize(0);

	try {
		msg_len = read(info->socket_ID, info->msg_buf.data(), __BUFFER_SIZE);

		if (msg_len < 0) {
			throw std::runtime_error("client quit: ");
		}
		else if (msg_len == 0) {
			info->session_requests.emplace_back(Request(info->msg_buf);
		}
		else {
			info->session_requests.emplace_back(Request(info->msg_buf));
		}
	}
	catch (const std::runtime_error& e) {
		info->client_alive = false;
		std::lock_guard<std::mutex> gaurd(client_lock);
		std::cout << e.what() << info->IP4_address << "\n\n";
		return false;
	}

	

	return true;
}



bool Client::new_message()
{
	info->current_buffer_len = 0;
	memset(info->read_buffer, 0, __BUFFER_SIZE);

	try {
		if (info->current_buffer_len = read(
			info->socket_ID, info->read_buffer, __BUFFER_SIZE - 1) <= 0) 
			throw std::runtime_error("client quit: ");
	}
	catch (const std::runtime_error& e) {
		info->client_alive = false;
		std::lock_guard<std::mutex> gaurd(client_lock);
		std::cout << e.what() << info->IP4_address << "\n\n";
		return false;
	}

	info->session_requests.
		emplace_back(Request(info->read_buffer, info->current_buffer_len));

	return true;
}


void Client::o(const char* out)
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << out << '\n';
}


void Client::process_request()
{
	Request& curr_req = info->session_requests.back();
  
	if (curr_req.get_method() == _GET && curr_req.get_path() == _INDEX
		&& curr_req.clean()) { generate_default_response(); } 
	else 
	if (curr_req.get_method() == _GET && curr_req.get_path()[0] == __INDEX
		&& curr_req.get_path().size() > 1 && curr_req.clean())
	{ generate_unique_response(); }
	
	else { generate_error_response(BAD_REQUEST); }
}


void Client::generate_error_response(int http_code)
{
	o("error");
	info->session_responses.emplace_back(Response(http_code));
}


bool Client::read_from_file(int file_fd, const char* content_buffer, int* file_size) 
{
	int _read = 1;

	while (_read > 0) { 
		if (_read = read(file_fd, 
		                 (void*)content_buffer, 
						  __BUFFER_SIZE) == -1) {
			return false;
		}
		else { *file_size += _read; }
	}

	return true;
}


bool Client::write_to_client(int socket_fd, const char* content_buffer, int file_size) 
{
	int _write{};
	if (_write = write(socket_fd, (void*)content_buffer, file_size) == -1) {
		return false;
	}
	else { return true; }
}


// bool Client::post_reponse()


void Client::generate_unique_response()
{
	o("unique");

	int file_fd = search();
	if (file_fd > -1) {

		int file_size = 0;
		const char* content_buffer = nullptr;

		if (read_from_file(file_fd, content_buffer, &file_size)) {
			if (write_to_client(info->socket_ID, content_buffer, file_size)) {
				info->session_responses.emplace_back(Response(OK));
			}
		}
		else { generate_error_response(INTERNAL_SERVER_ERROR); }
	}
	else { 
		close(file_fd);
		generate_error_response(BAD_REQUEST);
	}
}


bool Client::is_alive()
{
	return info->client_alive;
}


int Client::get_count()
{
	return info->request_count;
}


void Client::deny()
{
	int ec{};
	const char buff[19]{ "CONNECTION DENIED\n" };

	write(info->socket_ID, (const void*)buff, 19);
	info->client_alive = false;
	close(this->info->socket_ID);

	std::lock_guard<std::mutex> guard(client_lock);
	std::cout << "banned IP denied: " << info->IP4_address << "\n\n";
}


int Client::search()
{
	std::string _root(__ROOT);
	const char* _root_ptr = _root.c_str();

	DIR* root_ptr = opendir(_root_ptr);
	struct dirent* entry = nullptr;

	std::string path = info->session_requests.back().get_path();
	int target_fd = 0;
	
	// -- //

	while ((entry = readdir(root_ptr)) != NULL && target_fd == 0) 
	{
		if (std::string(entry->d_name) == path) {
			if (entry->d_type == DT_REG) 
			{
				std::string full_path = _root + path;
				const char* c_path = full_path.c_str();
				std::cout << c_path << std::endl;

				target_fd = open(c_path, O_RDONLY, 0644);
			}
			else if (entry->d_type == DT_DIR) {
				target_fd = -2;  // flag for now
			}
		}
	}

	// -- //
	std::cout << target_fd << '\n';
	return target_fd;
}


void Client::log_connection()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "new client: " << info->IP4_address << "\n\n";
}


void Client::generate_default_response()
{
	o("nice index");

	// const char* 

	// read_from_file("index.html");
	info->session_responses.emplace_back(Response(OK, "index.html"));
}


const std::string& Client::get_IP() const
{
	return info->IP4_address;
}


void Client::client_end_log()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "client log:\n"
		      << "[ " << info->IP4_address << " ]"
		      << "joined at: " << info->session_start_time << "\n\n";

	std::for_each(info->session_requests.begin(), 
			      info->session_requests.end(),
				 [&](Request request) { std::cout << request; }
	);
}


void Client::log_data__()
{
	/*std::lock_guard<std::mutex> gaurd(client_lock);
	int i = 

	std::cout << "session start: " << info->session_start_time << "\n";

	std::for_each(info->session_requests.begin(),
		          info->session_requests.end(),
		[&]() {
			std::cout << info->session_requests[i];
			std::cout << info->session_responses[i];
			++i;
		}
	);*/
}
