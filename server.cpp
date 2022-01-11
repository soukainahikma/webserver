#include "server/socket.hpp"
#include "request_response/response.hpp"
#include "request_response/request.hpp"
#include "request_response/request_handler.hpp"
#include "parsing/Server.hpp"
#include <sys/select.h>
#include <exception>

void connection_handler(int i,RequestHandler &req_handler);

int main()
{
	try {
		std::vector<Server> parse = parsing("./webserv.conf"); 
		RequestHandler req_handler (parse);
		server_socket mysocket(parse[0]);
		fd_set current_sockets;
		fd_set ready_sockets;
		int max_fd_so_far = 0;
		int new_socket;
		mysocket.set_server();
		int server_socket = mysocket.get_socket_fd();
		FD_ZERO(&current_sockets);	
		FD_SET(server_socket,&current_sockets);
		max_fd_so_far = server_socket;
		while(1)
		{
			ready_sockets = current_sockets;
			if(select(max_fd_so_far +1 ,&ready_sockets,NULL,NULL,NULL) < 0)
				throw "select failed: ";
			for(int i = 0;i<= max_fd_so_far; i++)
			{
				if(FD_ISSET(i, &ready_sockets))
				{
					if(i == server_socket)
					{
						if((new_socket = mysocket.accept_socket()) < 0)
							throw "failed to accept: ";
						FD_SET(new_socket,&current_sockets);
						if(new_socket > max_fd_so_far)
							max_fd_so_far = new_socket;
					}
					else
					{
						connection_handler(i,req_handler);
						FD_CLR(i,&current_sockets);
					}
				}
			}
		}
		return 0;
	} catch (const std::exception& e) {
		perror(e.what());
		return (1);
	}
}
//[alert] socket: select and discovered it's not ready sock.c:384: Operation timed out
