#ifndef LOCAL_CLIENT_H 
#define LOCAL_CLIENT_H 

#include <sys/types.h>
#include <dirent.h>

#include <vector>
#include "request.h"
#include "response.h"
#include "message.h"


namespace http
{

	class LocalClient
	{
	public:
		LocalClient() = default;
		Message GET(const char*);
		Message GET(const std::string&);
		int search(const std::string&);

	private:
		std::string root{};
	};

}


#endif
