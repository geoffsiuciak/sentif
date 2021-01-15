#include "local_client.h"

using namespace http;

LocalClient::LocalClient(const std::string& root)
{
	this->root = root;
}


void* LocalClient::GET(const char* input)
{
	int fd = search(std::string(input));
	if (fd > -1) {
		// read from fd
	}
}


Response& LocalClient::GET(const std::string& input)
{
	const char* c_path = input.c_str();
}


int LocalClient::search(const std::string& input)
{
	DIR* root_ptr = opendir(this->root.c_str());
	struct dirent* entry = nullptr;
	std::string _input = input;
	_input.erase(0, 1);

	while ((entry = readdir(root_ptr)) != NULL)
	{
		if (std::string(entry->d_name) == path) // match found in root 
		{
			if (entry->d_type == DT_REG) // matched file type
			{
				int target_FD{};
				std::string full_path = _root + '/' + path;
				const char* c_path = full_path.c_str();
				// std::cout << full_path << endl;

				if (target_FD = open(c_path, O_RDONLY, 0644) != -1) {
					return target_FD;
				}
				else {
					return -1;
				}
			}
			else if (entry->d_type == DT_DIR) // matched dir type
			{
				;
			}
		}
	}
	return -1;
}