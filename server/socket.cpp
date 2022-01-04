#include "socket.hpp"

void server_socket::set_add_struct()
{
	int port  = 8080;
	memset((char *)&address, 0, sizeof(address)); 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 
	address.sin_port = htons(port);
}
int server_socket::accept_socket()
{
	client_fd = accept(server_fd,(struct sockaddr*)& address,(socklen_t *)&addrlen);
	return(client_fd);
}

int server_socket::set_server()
{
	if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0)
	{
		perror("socket failed"); 
		return(0);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &option, sizeof(option)))
	{
		perror("setsockopt failed");
		return(0);
	}
	set_add_struct();
	if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0)
	{
		perror("binding failed");
		return(0);
	}
	if(listen(server_fd,3) < 0)
	{
		perror("listening failed: ");
		return(0);
	}
	return(1);
}