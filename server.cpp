#include "server/socket.hpp"
#include "response.hpp"
int main()
{
	response resp;
	server_socket mysocket;
	int new_socket = 0;
	int valread;
	char buffer[1024] = {0};
	mysocket.set_server();
	while(1)
	{
		if((new_socket = mysocket.accept_socket())<0)
		{
			perror("failed to accept: ");
			return(0);
		}
		valread = read(new_socket, buffer, 1024);
		const char *hello = resp.get_header("pages/info.html").c_str();;
		send(new_socket, hello, strlen(hello), 0);
		resp.header_cleaner();
		close(new_socket);
	}
	return 0;
}

/* Tout en C++ 98. malloc, free, write, htons, htonl,
ntohs, ntohl, select, poll, epoll (epoll_create,
epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
socket, accept, listen, send, recv, bind, connect,
inet_addr, setsockopt, getsockname, fcntl. */
//select() only uses (at maximum) three bits of data per file descriptor, while poll() typically uses 64 bits per file descriptor. In each syscall invoke poll() thus needs to copy a lot more over to kernel space. A small win for select().