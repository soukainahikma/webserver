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
	try
	{
		std::vector<Server> parse = parsing("./webserv.conf"); 
		server_socket info;
		RequestHandler req_handler (parse);
		std::vector<server_socket> socket_list = info.fill_list_socket(parse);
		int max_fd_so_far = info.get_max_fd_so_far(); // getter of the max;
		fd_set current_sockets;
		FD_ZERO(&current_sockets);
		current_sockets = info.get_set_socket();
		fd_set ready_sockets;
		int new_socket;
		while(1)
		{
			ready_sockets = current_sockets;
			if(select(max_fd_so_far +1 ,&ready_sockets,NULL,NULL,NULL) < 0)
				throw "select failed: ";
			for(int i = 0;i<= max_fd_so_far; i++)
			{
				if(FD_ISSET(i, &ready_sockets))
				{
					if(i == socket_list[0].get_server_fd())
					{
						new_socket = socket_list[0].accept_socket(i);
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
	}
	catch (const std::exception& e)
	{
		perror(e.what());
		return (1);
	}
}
//[alert] socket: select and discovered it's not ready sock.c:384: Operation timed out
