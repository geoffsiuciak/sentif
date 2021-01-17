#include "client.h"


using namespace http;


Client::Client(int socket_ID, sockaddr_in TCP_struct)
{
	this->client_data = std::make_shared<Client_Data>();
	this->client_data->socket_ID = socket_ID;
	this->client_data->TCP_struct = TCP_struct;

	this->client_data->IP4_address =
		std::string(inet_ntoa(this->client_data->TCP_struct.sin_addr));

	this->client_data->session_start_time =
		std::string("*session_start_time 12.34*"); // TO-DO

	this->client_data->current_buffer_len = 0;
	this->client_data->request_count = 0;
	this->client_data->time_alive = 0.0;
	this->client_data->client_alive = true;
}




/**********************************************************************************/
void Client::handle()
{
	log_connection();
	while (client_data->client_alive)
	{
		if (got_buffer()) {
			process_request();
			post_response();
		}
		else { client_data->client_alive = false; }
	}
}
/**********************************************************************************/




bool Client::got_buffer()
{
	client_data->current_buffer_len = 0;
	memset(client_data->read_buffer, 0, __BUFFER_SIZE);

	try
	{
		if (client_data->current_buffer_len = read(
			client_data->socket_ID, client_data->read_buffer, __BUFFER_SIZE - 1) <= 0) 
		{ throw std::runtime_error("client quit: "); }
	}
	catch (const std::runtime_error& e) {
		client_data->client_alive = false;
		std::lock_guard<std::mutex> gaurd(client_lock);
		std::cout << e.what() << client_data->IP4_address << "\n\n";
		return false;
	}

	client_data->session_requests.
		emplace_back(Request(client_data->read_buffer, client_data->current_buffer_len));

	return true;
}


void Client::o(const char* out)
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << out << '\n';
}


void Client::process_request()
{
	Request& curr_req = client_data->session_requests.back();
  
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
	client_data->session_responses.emplace_back(Response(http_code));
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
		else {
			*file_size += _read;
		}
	}

	return true;
}


bool Client::write_to_client(int socket_fd, const char* content_buffer, int file_size) 
{
	int _write{};
	if (_write = write(socket_fd, (void*)content_buffer, file_size) == -1) {
		return false;
	}
	else {
		return true;
	}
}


void Client::generate_unique_response()
{
	o("unique");

	int file_fd = search();
	if (file_fd > -1) {

		int file_size = 0;
		const char* content_buffer = nullptr;

		if (read_from_file(file_fd, content_buffer, &file_size) &&
		    write_to_client(client_data->socket_ID, content_buffer, file_size)) {
			client_data->session_responses.emplace_back(Response(OK));
		}
		else {
			generate_error_response(INTERNAL_SERVER_ERROR);
		}
	}
	else { generate_error_response(BAD_REQUEST);}
}


void Client::post_response()
{
	// o("posting\n");
}


bool Client::is_alive()
{
	return client_data->client_alive;
}


int Client::get_count()
{
	return client_data->request_count;
}


void Client::deny()
{
	int ec{};
	const char buff[19]{ "CONNECTION DENIED\n" };

	write(client_data->socket_ID, (const void*)buff, 19);
	client_data->client_alive = false;
	close(this->client_data->socket_ID);

	std::lock_guard<std::mutex> guard(client_lock);
	std::cout << "banned IP denied: " << client_data->IP4_address << "\n\n";
}


int Client::search()
{
	std::string _root(__ROOT);
	const char* _root_ptr = _root.c_str();

	DIR* root_ptr = opendir(_root_ptr);
	struct dirent* entry = nullptr;

	std::string path = client_data->session_requests.back().get_path();
	int target_FD = 0;
	
	// -- //

	while ((entry = readdir(root_ptr)) != NULL && fd == -1)
	{
		if (std::string(entry->d_name) == path) // match found in root 
		{
			if (entry->d_type == DT_REG) // matched file type
			{
				
				std::string full_path = _root + path;
				const char* c_path = full_path.c_str();
				// std::cout << full_path << endl;

				if (target_FD = open(c_path, O_RDONLY, 0644) != -1) {
					return target_FD;
				}
			}
			else if (entry->d_type == DT_DIR) // matched dir type
			{
				;
			}
		}
	}
	return target_FD;
}


void Client::log_connection()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "new client: " << client_data->IP4_address << "\n\n";
}


void Client::generate_default_response()
{
	o("nice index");

	// const char* 

	// read_from_file("index.html");
	client_data->session_responses.emplace_back(Response(OK, "index.html"));
}


const std::string& Client::get_IP() const
{
	return client_data->IP4_address;
}


void Client::client_end_log()
{
	std::lock_guard<std::mutex> gaurd(client_lock);
	std::cout << "client log:\n"
		      << "[ " << client_data->IP4_address << " ]"
		      << "joined at: " << client_data->session_start_time << "\n\n";

	std::for_each(client_data->session_requests.begin(), 
			      client_data->session_requests.end(),
				 [&](Request request) { std::cout << request; }
	);
}


void Client::log_data__()
{
	/*std::lock_guard<std::mutex> gaurd(client_lock);
	int i = 

	std::cout << "session start: " << client_data->session_start_time << "\n";

	std::for_each(client_data->session_requests.begin(),
		          client_data->session_requests.end(),
		[&]() {
			std::cout << client_data->session_requests[i];
			std::cout << client_data->session_responses[i];
			++i;
		}
	);*/
}
