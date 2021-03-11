#ifndef REQUEST_H
#define REQUEST_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <ctime>


class Request 
{
	friend std::ostream& operator<<(std::ostream&, const Request&);

public:
	Request(const in_addr IP_address, int ID);
    const int& get_ID() const {return this->ID;}
    const std::string& get_client() const;
	const std::string& get_method() const;
	const std::string& get_path() const;
	const std::string& get_time() const;
    void set_request(char*);

    bool clean() const;
	void log_out();

private:
    int ID;
    std::string time;
    std::string IP4_address;

    std::string method{};
	std::string path{};
	
};


#endif
