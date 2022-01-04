#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
class server_socket
{
	private:
		int server_fd;
		int client_fd;
		int valread_checker;
		struct sockaddr_in address;
		int option ;
		int addrlen;

	public:
		server_socket()
		{
			option = 1;
			addrlen = sizeof(address);

		}
		void set_add_struct();
		int set_server();
		int accept_socket();

};
#endif