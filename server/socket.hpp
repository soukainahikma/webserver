#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include "../parsing/Server.hpp"

class server_socket
{
	private:
		int option;
		int addrlen;
		struct sockaddr_in address;
		int server_fd;
		int client_fd;
		int max_fd_so_far;
		fd_set set_socket;
		int port_sock;

	public:
		server_socket();
		// sockaddr_in set_add_struct(Server server);
		sockaddr_in set_add_struct(int port);
		// server_socket prepare_socket(Server server);
		server_socket prepare_socket(int port);
		std::vector<server_socket> fill_list_socket(std::vector<Server> servers, std::map<int,int> &ports);
		int accept_socket(int fd);
		int get_max_fd_so_far() const;
		int get_server_fd() const;
		int get_port() const;
		fd_set &get_set_socket();
};
#endif