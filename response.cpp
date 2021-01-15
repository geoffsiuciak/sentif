#include "response.h"


Response::Response(int http_code)
{
	this->http_code = http_code;
}


Response::Response(int http_code, const char* filename)
{
	this->http_code = http_code;
	this->filename = std::string(filename);
}


const std::string& Response::get_post_time() const
{
	return post_time;
}


std::ostream& operator<<(std::ostream& out, const Response& response)
{
	/*out << "Request ID: " << response.get_ID() << '\n'
		<< "Time: " << response.get_time() << '\n'
		<< "[ " << response.get_request() << " ]" << "\n\n";*/

	return out;
}
