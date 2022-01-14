#include "server/socket.hpp"
#include "request_response/response.hpp"
#include "request_response/request.hpp"
#include "request_response/request_handler.hpp"
#include "parsing/Server.hpp"
#include <sys/select.h>
#include <exception>

void connection_handler(int i,RequestHandler &req_handler);
std::vector<Server> parsing(std::string file, std::map<int,int> &m);

int main()
{
	try
	{
		std::map<int,int> ports;
		std::vector<Server> parse = parsing("./webserv.conf", ports); 
		server_socket info;
		RequestHandler req_handler (parse);
		std::vector<server_socket> socket_list = info.fill_list_socket(parse, ports);
		int max_fd_so_far = info.get_max_fd_so_far(); // getter of the max;
		fd_set current_sockets;
		FD_ZERO(&current_sockets);
		current_sockets = info.get_set_socket();
		fd_set ready_sockets;
		int new_socket;
		int port = -1;
		while(1)
		{
			ready_sockets = current_sockets;
			// std::cout<< "here before select"<<std::endl;
			if(select(max_fd_so_far +1 ,&ready_sockets,NULL,NULL,NULL) < 0)
				throw "select failed: ";
			for(int i = 0;i<= max_fd_so_far; i++)
			{
				if(FD_ISSET(i, &ready_sockets))
				{
				// std::cout<< "index : " << i << std::endl;
					bool check = false;
					//check to accept
					for (size_t j = 0; j < socket_list.size(); j++)
					{
						if(i == socket_list[j].get_server_fd())
						{
							new_socket = socket_list[j].accept_socket(i);
							port = socket_list[j].get_port();
							FD_SET(new_socket,&current_sockets);
							if(new_socket > max_fd_so_far)
								max_fd_so_far = new_socket;
							check = true;
							// std::cout << "<----- got request " << new_socket << " ------->" << std::endl;
							break;
						}
					}
					//check to accept
					if(check == false)
					{

						connection_handler(i,req_handler);
						// std::cout << "<----- request: {" << new_socket << " <==>  " << i  <<  "} handled ------->" << std::endl;
						FD_CLR(i,&current_sockets);
					}
				}
			}
			// std::cout << "<----- request show time ------->" << std::endl;
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
