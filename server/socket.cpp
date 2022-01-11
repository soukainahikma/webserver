#include "socket.hpp"

void server_socket::set_add_struct()
{
	int port  = std::stoi(s.get_listen());
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

void server_socket::set_server()
{
	if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0)
		throw std::runtime_error("socket failed");
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &option, sizeof(option)))
		throw std::runtime_error("setsockopt failed");
	set_add_struct();
	if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0)
		throw std::runtime_error("binding failed");
	// remember to close the server fd before quiting
	if(listen(server_fd,0) < 0)
		throw std::runtime_error("listening failed: ");
}

int server_socket::get_socket_fd() const
{
	return(server_fd);
}