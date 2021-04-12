#include <iostream>
#include "server.h"

// using namespace sentif;

int main(int argc, char** argv)
{
    sentif::WebServer s(atoi(argv[0]), argv[1]);
    s.go();
	
	// /* perform any action while server is running */
	/*
	s.ban("10.0.0.123");
	// s.ban_IP("127.0.0.1");
	s.print_IP_banlist();
	s.set_host_IP("10.0.0.175");

	http::LocalClient lcl;
	std::vector<http::Message> lr;

	lr.push_back(lcl.GET("index.html"));
	lr.push_back(lcl.GET("this_page.html"));
	lr.push_back(lcl.GET("another.html"));
	
	s.run_for(60);
    s.kill();

    s.show("10.0.0.789");
	
	for (http::Message msg : lr) {
		// std::cout << msg;
	}
	
	*/
	return 0;
}
