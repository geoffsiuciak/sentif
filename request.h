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
#include <vector>



class Request 
{
	friend std::ostream& operator<<(std::ostream&, const Request&);

public:
	Request(const char*);
    std::string init_time();
    const std::string& get_client() const;
    const std::string& get_request() const;
	const std::string& get_method() const;
	const std::string& get_path() const;
	const std::string& get_time() const;
	bool clean() const;

    void set_request(char);

    void log_file();
	void log_out();

private:
    int ID;
    std::string IP4_address;
    std::string method{};
	std::string path{};
	std::string time{};
};


#endif
