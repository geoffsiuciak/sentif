#include "admin.h"

namespace sentif {

void Admin::interp_loop()
{
	std::thread interp([this]() -> void {
		// std::lock_guard<std::mutex> gaurd(this->Admin_lock);
		while (RUNNING)
		{
			std::string entry;
			std::cout << INTERP_PROMPT;
			std::getline(std::cin, entry);
			interpret(std::stringstream(entry));
		}
	});
	interp.detach();
}


void Admin::interpret(std::stringstream&& entry) 
{
	std::string cmd;
	std::string arg;

	entry >> cmd;
	entry >> arg;

	cmd == "ban"   ? ban(arg)     : 
	cmd == "allow" ? allow(arg)   :
	cmd == "show"  ? show(arg)    :
	cmd == "kill"  ? kill()       :
	cmd == ""      ? (void)[](){} : 
	LOG("invalid command!");
}


void Admin::allow(const std::string& IP)
{
	bool hit = false;
	std::vector<std::string>::iterator i = this->IP_banlist.begin();
	std::vector<std::string>::iterator end = this->IP_banlist.end();

	for (i; !hit; ) {
    	if (*i == IP) {
			this->IP_banlist.erase(i);
			LOG("* IP allowed *");
			hit = true;
		} else { ++i; }
	} 
	if (!hit) LOG("IP already allowed");
}


void Admin::show(const std::string& arg)
{
	if (arg == "banlist") {
		print_IP_banlist();
	} 
	else if (isdigit(arg[0])) 
	{
		std::lock_guard<std::mutex> lock(Admin_lock);
		std::cout << "\n-- data log for client [ " << arg << " ] --\n";

		if (request_DB.size() == 0) {
			std::cout << "* no connections this runtime *\n";
		}

        for (auto req : request_DB) {
            if (arg == req.get_client())
                std::cout << req;
        }

		std::cout << "-- end data log --\n\n";

	} else if (arg.size() == 0) {
		LOG("nothing to show!");
	} else {
		LOG("invalid arg!");
	}
}


void Admin::show(const char* _IP)
{
	std::string IP(_IP);
	std::lock_guard<std::mutex> lock(Admin_lock);
	std::cout << "-- data log for client [ " << IP << " ] --\n";

	if (request_DB.size() == 0) {
		std::cout << "* no sessions this runtime *\n";
	}

	for (auto req : request_DB) {
            if (IP == req.get_client())
                std::cout << req;
        }

	std::cout << "-- end data log --\n\n";
}

} // namespace sentif
