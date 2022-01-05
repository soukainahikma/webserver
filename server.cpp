#include "server/socket.hpp"
#include "request_response/response.hpp"
#include <sys/select.h>
int main()
{
	server_socket mysocket;
	fd_set current_sockets;
	fd_set ready_sockets;
	Response resp;
	int valread;
	char buffer[1024] = {0};

	int new_socket = 0;
	FD_ZERO(&current_sockets);
	
	mysocket.set_server();
	int server_socket = mysocket.get_socket_fd();
	FD_SET(server_socket,&current_sockets);
	while(1)
	{
		ready_sockets = current_sockets;
		if(select(FD_SETSIZE,&ready_sockets,NULL,NULL,NULL) < 0)
		{
			perror("select failed: ");
			return(0);
		}
		for(int i = 0;i< FD_SETSIZE; i++)
		{
			if(FD_ISSET(i, &ready_sockets))
			{
				std::cout << "index     :"<<i<< std::endl; 
				if(i == server_socket)
				{
					if((new_socket = mysocket.accept_socket())<0)
					{
						perror("failed to accept: ");
						return(0);
					}
					FD_SET(new_socket,&current_sockets);	
				}
				else
				{
					valread = read(i, buffer, 1024);
					// std::cout << buffer << std::endl;
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