#include "request.h"


Request::Request(const in_addr IP_address, int ID)
{
    this->ID = ID;
    this->time = init_time();
    IP4_address = std::string(inet_ntoa(IP_address));
}


std::string Request::init_time()
{
	this->time = "00:00 date";
}


const std::string& Request::get_request() const
{
	return buffer_string;
}


void Request::set_request(char* buffer)
{
    std::stringstream ss(buffer);
	ss >> method;
	ss >> path;

    this->path.erase(0, 1);
}


const std::string& Request::get_method() const
{
	return method;
}


const std::string& Request::get_path() const
{
	return path;
}


const std::string& Request::get_time() const
{
	return time;
}


bool Request::clean() const
{
	int count{};
	for (char c : path) {
		if (c == '.') ++count;
		if (count > 1) return false;
	}
	return true;
}


void Request::log_file()
{
	printf("logging to file\n");
}


void Request::log_out()
{
	// std::lock_guard<std::mutex> gaurd(request_lock);
	std::cout << "request log:\n"
	          << "-> " << buffer_string
		      << "time: " << time
		      << "\n\n";
}


std::ostream& operator<<(std::ostream& out, const Request& request)
{
	out << "Time: " << request.get_time() << '\n'
		<< "-> " << request.get_request() << "\n\n";

	return out;
}
