#ifndef RESPONSE_H
#define RESPONSE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <ostream>
#include <string>


class Response
{
	friend std::ostream& operator<<(std::ostream&, const Response&);

public:
	Response(int);
	Response(int, const char*);
	const std::string& get_post_time() const;

private:
	int http_code{};
	std::string post_time{};
	std::string filename{};
};


#endif
