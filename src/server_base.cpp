#include "server_base.h"

namespace sentif {

ServerBase::ServerBase(ctx_t ctx)
{
    admin_socket = 0;
    server_socket = 0;
    this->settings = std::move(ctx);
    
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = DOMAIN;
    server_info.sin_addr.s_addr = INADDR_ANY;   
	server_info.sin_port = htons(settings.host);
    
    if (settings.admin > -1) {
        memset(&admin_info, 0, sizeof(admin_info));
        admin_info.sin_family = DOMAIN;
        admin_info.sin_addr.s_addr = INADDR_ANY;   
        admin_info.sin_port = htons(settings.admin);
    }
}

ServerBase::~ServerBase() {}

void ServerBase::ban(const char* ip)
{
	this->banned.insert({std::string(ip), false});
}

void ServerBase::ban(const std::string& ip)
{
	this->banned.insert({std::move(ip), false});
}

void ServerBase::show_banlist() const
{
	// std::cout << "IP BAN LIST:\n";
	// for (auto ip : this->IP_banlist) {
    //     std::cout << ip << '\n';
    // }
    // std::cout << "\n\n";
}

bool ServerBase::allowed(const char* ip)
{
	// bool allowed = true;
	// std::string IP(client_IP);
    // std::lock_guard<std::mutex> lock(server_lock);

	// for (auto ip : this->IP_banlist) {
    //     if (ip == IP)
    //         allowed = false;
    // }

	// return allowed;
    return true;
}

bool ServerBase::allowed(const std::string& ip)
{
    if (banned.count(ip) > 0)
        return banned.at(ip);
    else
        return true;
}

void ServerBase::deny(int client)
{
    int fd = open(DENY, O_RDONLY);

    if (fd != -1) {
        char buffer[BUFFER_L];
        int bytes;
        while ((bytes = read(fd, buffer, 1024)) > 0)
            write(client, buffer, bytes);
    }

    close(client);
}

void ServerBase::kill()
{
	close(server_socket);
	LOG("* killed *");
}

void bail(const char* err_msg)
{
	fprintf(stderr, "server error: %s\n", err_msg);
	exit(SENTIF_ERROR);
}

void ServerBase::LOG(const char* msg) noexcept
{
	// std::lock_guard<std::mutex> gaurd(server_lock);
	std::cout << msg << '\n';
}

} // namespace sentif
