#include "admin.h"

namespace sentif {

Admin::Admin() : admin_name(std::string("admin"))
{
    NEED_PW = true;
    ONLINE = false;
}

void Admin::set_pw(int socket)
{
    write_file(socket, (char*)LOGO);
    write_msg(socket, "login: ");
    std::string name = get_msg(socket);

    if (name != this->admin_name){
        exit("\ninvalid admin name - this will be reported.\n");
    } else {
        write_msg(socket, "set pw: ");
        std::string pass1 = get_msg(socket);
        write_msg(socket, "confirm: ");
        std::string pass2 = get_msg(socket);

        if (pass1 == pass2) {
            NEED_PW = false;
            this->admin_password = pass1;
            write_msg(socket, "\npassword set.\nwelcome admin!\n\n");
        } else
            exit("\nnew passwords did not match.\n");
    }
}

void Admin::login()
{
    write_file(FD, (char*)LOGO);
    write_msg(FD, "login: ");
    std::string name = get_msg(FD);

    if (name == this->admin_name) {
        write_msg(FD, "pw: ");
        std::string pass = get_msg(FD);

        if (pass == this->admin_password)
            write_msg(FD, "\nwelcome admin!\n\n");
        else
            exit("\nincorrect pw - this will be reported.\n");

    } else exit("\ninvalid admin name - this will be reported.\n");
}

void Admin::go(int socket)
{
    this->FD = socket;
    ONLINE = true;

    if (NEED_PW) 
        set_pw(socket);
    else 
        login();

    while (ONLINE) {
        std::string cmd, arg;
        write_msg(FD, (char *)ADMIN_PROMPT);

        try {
            cmd = get_msg(FD);
        }
        catch (...) {
            ONLINE = false;
            break;
        }

        if (cmd == "stats") {
            ;
        }
        else if (cmd == "exit") {
            this->exit("\ngoodbye!\n");
        }
        else if (cmd == "") { /* no input */ }
        else {
            write_msg(FD, "invalid command\n");
        }
    }
}

void Admin::exit(const char* msg)
{
    write_msg(FD, msg);
    close(FD);
    FD = -1;
    ONLINE = false;
}

// void Admin::interp_loop()
// {
// 	std::thread interp([this]() -> void {
// 		// std::lock_guard<std::mutex> gaurd(this->Admin_lock);
// 		while (RUNNING)
// 		{ 
// 			std::string entry;
// 			std::cout << INTERP_PROMPT;
// 			std::getline(std::cin, entry);
// 			interpret(std::stringstream(entry));
// 		}
// 	});
// 	interp.detach();
// }


// void Admin::interpret(std::stringstream&& entry) 
// {
// 	std::string cmd;
// 	std::string arg;

// 	entry >> cmd;
// 	entry >> arg;

// 	cmd == "ban"   ? ban(arg)     : 
// 	cmd == "allow" ? allow(arg)   :
// 	cmd == "show"  ? show(arg)    :
// 	cmd == "kill"  ? kill()       :
// 	cmd == ""      ? (void)[](){} : 
// 	LOG("invalid command!");
// }


// void Admin::allow(const std::string& IP)
// {
// 	bool hit = false;
// 	std::vector<std::string>::iterator i = this->IP_banlist.begin();
// 	std::vector<std::string>::iterator end = this->IP_banlist.end();

// 	for (i; !hit; ) {
//     	if (*i == IP) {
// 			this->IP_banlist.erase(i);
// 			LOG("* IP allowed *");
// 			hit = true;
// 		} else { ++i; }
// 	} 
// 	if (!hit) LOG("IP already allowed");
// }


// void Admin::show(const std::string& arg)
// {
// 	if (arg == "banlist") {
// 		print_IP_banlist();
// 	} 
// 	else if (isdigit(arg[0])) 
// 	{
// 		std::lock_guard<std::mutex> lock(Admin_lock);
// 		std::cout << "\n-- data log for client [ " << arg << " ] --\n";

// 		if (request_DB.size() == 0) {
// 			std::cout << "* no connections this runtime *\n";
// 		}

//         for (auto req : request_DB) {
//             if (arg == req.get_client())
//                 std::cout << req;
//         }

// 		std::cout << "-- end data log --\n\n";

// 	} else if (arg.size() == 0) {
// 		LOG("nothing to show!");
// 	} else {
// 		LOG("invalid arg!");
// 	}
// }


// void Admin::show(const char* _IP)
// {
// 	std::string IP(_IP);
// 	std::lock_guard<std::mutex> lock(Admin_lock);
// 	std::cout << "-- data log for client [ " << IP << " ] --\n";

// 	if (request_DB.size() == 0) {
// 		std::cout << "* no sessions this runtime *\n";
// 	}

// 	for (auto req : request_DB) {
//             if (IP == req.get_client())
//                 std::cout << req;
//         }

// 	std::cout << "-- end data log --\n\n";
// }

} // namespace sentif
