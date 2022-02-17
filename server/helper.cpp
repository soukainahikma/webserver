#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"
#include <sys/ioctl.h>

#define SIZE 65507
int fileCheck(std::string fileName, std::string req_type)
{
	if (!access(fileName.c_str(), F_OK))
	{
		if (!access(fileName.c_str(), R_OK))
			return req_type == "GET" ? OK : CREATED;
		return FORBIDDEN;
	}
	return NOT_FOUND;
}
int check_body(std::string files, std::map<int, map_info *>::iterator &it)
{
	size_t found;
	size_t end;
	size_t start;
	int content_length = it->second->content_length;
	if ((start = files.find("\r\n\r\n")) != std::string::npos)
	{
		if ((found = files.find("Content-Length: ")) != std::string::npos) //check if the content type is == -1
		{
			if (content_length == -1)
			{
				if ((end = files.find("\r\n", found)))
				{
					std::string str = files.substr(found + 16, end - found - 16);
					it->second->content_length = atoi(str.c_str());
					return(0);
				}
				return (1);
			}
			else
			{
				if (files.size() - start-4 == it->second->content_length)
					return (1);
				else
					return (0);
			}
		}
		else
			return (1);
	}
	return (0);
}

void connection_handler(int i, RequestHandler &req_handler, int port, fd_set &write_fds, fd_set &current_socket, std::map<int, map_info *> &map_of_req)
{
	std::string files;
	map_info *info = new map_info();
	std::map<int, map_info *>::iterator it;
	char *buffer = (char *)malloc(sizeof(char) * 100);
	bzero(buffer, 100);
	int rd = recv(i, buffer, 99, 0);
	if (rd > 0)
	{
		if ((it = map_of_req.find(i)) != map_of_req.end())
		{
			buffer[rd] = '\0';
			// std::cout << buffer << std::endl;
			it->second->body += std::string(buffer);
			files = it->second->body;
			if (check_body(files, it) == 0)
				return;
			std::cout<< it->second->content_length << std::endl;
			FD_CLR(i, &current_socket);
		}
		else
		{
			std::cout << RED << "here" << RESET << std::endl;
			info->body = std::string(buffer);
			info->content_length = -1;
			it = map_of_req.insert(std::pair<int, map_info *>(i, info)).first;
			files = info->body;
			if (check_body(files, it) == 0)
				return;
		}
	}
	// else if (rd == 0)
	// {
	// 	FD_CLR(i,&current_socket);
	// }
	else
		return;
	std::cout<< files << std::endl;
	if (!files.empty())
	{
		Request req(files, port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		const char *hello = resp.get_header().c_str();
		if (FD_ISSET(i, &write_fds))
		{
			send(i, hello, strlen(hello), 0);
			map_of_req.erase(i);
			FD_CLR(i, &write_fds);
			close(i);
		}
	}
	free(buffer);
}
