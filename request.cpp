#include "request.h"


Request::Request(const char* buffer)
{
	this->buffer_string = std::string(buffer);
	this->time = init_time();

	std::stringstream ss(buffer_string);
	ss >> method;
	ss >> path;
}


std::string Request::init_time()
{
	return "00:00 date";
}


const std::string& Request::get_request() const
{
	return buffer_string;
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
