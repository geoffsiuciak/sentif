#ifndef LOCAL_CLIENT_H 
#define LOCAL_CLIENT_H 

#include <sys/types.h>
#include <dirent.h>

#include <vector>
#include "request.h"
#include "response.h"


namespace http
{

	class LocalClient
	{
	public:
		LocalClient(const std::string&);
		void* GET(const char*);
		Response& GET(const std::string&);
		int search(const std::string&);
	private:
		std::string root{};
	};

}


#endif
