#include "request.h"


Request::Request(const char* buffer, int length)
{
	// try?
	// char new_buffer[length];
	// memcpy(new_buffer, buffer, length);

	this->buffer_string = std::string(buffer);
	this->time = "4:20";
	this->length = length;

	std::stringstream ss(buffer_string);
	ss >> method;
	ss >> path;
}


Request::Request(const std::string& input)
{
	// called only by a LocalClient

	this->buffer_string = input;
	this->time = "4:20";
	this->length = buffer_string.size();

	std::stringstream ss(buffer_string);
	ss >> method;
	ss >> path;
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
		<< "[ " << request.get_request() << " ]" << "\n\n";

	return out;
}
