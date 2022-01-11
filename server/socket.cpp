#include "socket.hpp"

server_socket::server_socket()
{
	option = 1;
	addrlen = sizeof(address);
}

sockaddr_in server_socket::set_add_struct(Server s)
{
	int port  = std::stoi(s.get_listen());
	memset((char *)&address, 0, sizeof(address)); 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 
	address.sin_port = htons(port);
	return(address);

}

int server_socket::accept_socket(int fd)
{
	if((client_fd = accept(fd,(struct sockaddr*)& address,(socklen_t *)&addrlen))< 0)
	{
		close(fd);
		throw std::runtime_error("failed to accept: ");
	}
	return(client_fd);
}

server_socket server_socket::prepare_socket(Server server)
{
	server_socket mysocket;
	if((mysocket.server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0)
		throw std::runtime_error("socket failed");
	if (setsockopt(mysocket.server_fd, SOL_SOCKET, SO_REUSEADDR , &option, sizeof(option)))
	{
		close(mysocket.server_fd);
		throw std::runtime_error("setsockopt failed");
	}
	mysocket.address = set_add_struct(server);
	if(bind(mysocket.server_fd,(struct sockaddr*)&address,sizeof(address)) < 0)
	{
		close(mysocket.server_fd);
		throw std::runtime_error("binding failed");
	}
	if(listen(mysocket.server_fd,0) < 0)
	{
		close(mysocket.server_fd);
		throw std::runtime_error("listening failed: ");
	}
	return(mysocket);
}

std::vector<server_socket> server_socket::fill_list_socket(std::vector<Server> servers)
{
	server_socket sock;
	FD_ZERO(&set_socket);
	max_fd_so_far = 0;
	std::vector<server_socket> sockets;
	for (size_t i = 0; i < servers.size(); i++)
	{
		sock = prepare_socket(servers[i]);
		sockets.push_back(sock);
		FD_SET(sock.server_fd,&set_socket);
		if(sock.server_fd > max_fd_so_far)
			max_fd_so_far = sock.server_fd;
	}
	return(sockets);
}
int server_socket::get_max_fd_so_far() const
{
	return(max_fd_so_far);
}
int server_socket::get_server_fd() const
{
	return(server_fd);
}
fd_set & server_socket::get_set_socket()
{
	return(set_socket);
}