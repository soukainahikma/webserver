#include "server/socket.hpp"
#include "request_response/response.hpp"
#include "request_response/request.hpp"
#include "request_response/request_handler.hpp"
#include <sys/select.h>
#include <exception>

void connection_handler(int fd,std::vector<t_server> parse_server,RequestHandler req_handler);

t_location fill_location(std::string path, bool autoindex,std::vector<std::string> indexs,std::vector<std::string> allow_methods)
{
	t_location loc;
	loc.path = path;
	loc.autoindex = autoindex;
	loc.indexs =  indexs;
	loc.allow_methods =  allow_methods;
	return(loc);
}
std::vector<t_server> fill_servers(int _listen,std::string _host,std::vector<std::string> _server_names,
									std::string client_max_body,std::vector<std::string> _err_pages,std::string _root)
{
	std::vector<t_location> location;
	location.push_back(fill_location("/test" ,false,std::vector<std::string>(1,"index.html"),std::vector<std::string>(1,"GET")));
	t_server myserver;
	myserver._listen = _listen;
	myserver._host = _host;
	myserver._server_names = _server_names;
	myserver.client_max_body = client_max_body;
	myserver._err_pages = _err_pages;
	myserver._root = _root;
	myserver.locations = location;
	std::vector<t_server> vec_of_servers;
	
	vec_of_servers.push_back(myserver);
	return(vec_of_servers);
}
int main()
{
	try
	{
		std::vector<t_server> parse_server = fill_servers(8080, "127.0.0.1",std::vector<std::string>(1,"localhost"),"1m",std::vector<std::string>(1,"404.html"),"./pages");
		RequestHandler req_handler (parse_server);
		server_socket mysocket(parse_server[0]);
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
				throw std::runtime_error("select failed: ");
			for(int i = 0;i<= max_fd_so_far; i++)
			{
				if(FD_ISSET(i, &ready_sockets))
				{
						std::cout << "test select 1" << std::endl;
					if(i == server_socket)
					{
						if((new_socket = mysocket.accept_socket()) < 0)
							throw std::runtime_error("failed to accept: ");
						std::cout << "test select 2" << std::endl;
						FD_SET(new_socket,&current_sockets);
						if(new_socket > max_fd_so_far)
							max_fd_so_far = new_socket;
					}
					else
					{
						connection_handler(i,parse_server,req_handler);
						FD_CLR(i,&current_sockets);
					}
				}
			}
		}
			return 0;
	}
	catch(const std::exception& e)
	{
		perror(e.what());
		return 1;
	}
}
