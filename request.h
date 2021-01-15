#ifndef REQUEST_H
#define REQUEST_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <mutex>



class Request 
{
	friend std::ostream& operator<<(std::ostream&, const Request&);

public:
	Request(const char*, int);
	Request(const std::string&, int);
	const std::string& get_request() const;
	const std::string& get_method() const;
	const std::string& get_path() const;
	const std::string& get_time() const;
	bool clean() const;

	void log_file();
	void log_out();

private:
	std::string buffer_string{};
	std::string method{};
	std::string path{};
	std::string time{};
	int length{};
	// std::mutex request_lock;
};


#endif
