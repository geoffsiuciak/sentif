#include "request.h"


Request::Request(const in_addr IP_address, int ID)
{
    this->ID = ID;
    // this->time = std::string(std::chrono::system_clock::now());
    this->IP4_address = std::string(inet_ntoa(IP_address));

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    this->time = ss.str();
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


void Request::log_out()
{
	std::cout << "request log:"
              << "\nID: " << ID
		      << "\ntime: " << time
	          << method << path
              << "\n\n";
}


std::ostream& operator<<(std::ostream& out, const Request& request)
{
    out << "request log:"
        << "\nID: " << request.get_ID()
        << "\ntime: " << request.get_time()
        << "\n" << request.get_method() << request.get_path()
        << "\n\n";

	return out;
}
