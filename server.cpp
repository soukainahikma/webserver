#include "server/socket.hpp"
#include "request_response/response.hpp"
#include <sys/select.h>
int main()
{
	server_socket mysocket;
	fd_set current_sockets;
	fd_set ready_sockets;
	Response resp;
	int max_fd_so_far = 0;
	int valread;
	char buffer[1024] = {0};

	int new_socket = 0;
	FD_ZERO(&current_sockets);
	
	mysocket.set_server();
	int server_socket = mysocket.get_socket_fd();
	FD_SET(server_socket,&current_sockets);
	max_fd_so_far = server_socket;
	while(1)
	{
		ready_sockets = current_sockets;
		if(select(max_fd_so_far +1 ,&ready_sockets,NULL,NULL,NULL) < 0)
		{
			perror("select failed: ");
			return(0);
		}
		for(int i = 0;i<= max_fd_so_far; i++)
		{
			if(FD_ISSET(i, &ready_sockets))
			{
				if(i == server_socket)
				{
					if((new_socket = mysocket.accept_socket())<0)
					{
						perror("failed to accept: ");
						return(0);
					}
					FD_SET(new_socket,&current_sockets);
					if(new_socket > max_fd_so_far)
						max_fd_so_far = new_socket;
				}
				else
				{
					valread = read(i, buffer, 1024);
					std::cout << buffer << std::endl;
					
					const char *hello = resp.get_header("pages/info.html").c_str();;
					send(i, hello, strlen(hello), 0);
					resp.header_cleaner();
					close(i);
					FD_CLR(i,&current_sockets);
				}
			}
		}
		
	}
	return 0;
}

/* Tout en C++ 98. malloc, free, write, htons, htonl,
ntohs, ntohl, select, poll, epoll (epoll_create,
epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
socket, accept, listen, send, recv, bind, connect,
inet_addr, setsockopt, getsockname, fcntl. */
//select() only uses (at maximum) three bits of data per file descriptor, while poll() typically uses 64 bits per file descriptor. In each syscall invoke poll() thus needs to copy a lot more over to kernel space. A small win for select().