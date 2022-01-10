#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

typedef struct	s_location
{
	std::string					path;
	bool						autoindex;
	std::vector<std::string>	indexs;
	std::vector<std::string>	allow_methods;
}				t_location;


typedef struct	s_server
{
	int							_listen;
	std::string					_host;
	std::vector<std::string>	_server_names;
	std::string					client_max_body;
	std::vector<std::string>	_err_pages;
	std::string					_root;
	std::vector<t_location>		locations;
}				t_server;

class server_socket
{
	private:
		int server_fd;
		int client_fd;
		int valread_checker;
		struct sockaddr_in address;
		int option ;
		int addrlen;
		t_server s;

	public:
		server_socket(t_server s)
		{
			option = 1;
			addrlen = sizeof(address);
			this->s = s;
		}
		void set_add_struct();
		void set_server();
		int accept_socket();
		int get_socket_fd() const;

};
#endif