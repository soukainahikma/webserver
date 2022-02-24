#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"
#include <sys/ioctl.h>
#include <dirent.h>

int fileCheck(std::string fileName, std::string req_type)
{
	DIR *pDir;
	pDir = opendir(fileName.c_str());
	if (pDir != NULL)
	{
		closedir(pDir);
		return IS_NOT_AUTO_INDEXED;
	}
	if (!access(fileName.c_str(), F_OK))
	{
		if (!access(fileName.c_str(), R_OK))
			return req_type == "GET" || req_type == "DELETE" ? OK : CREATED;
		return FORBIDDEN;
	}
	return NOT_FOUND;
}
int check_body(std::string files, std::map<int, map_info>::iterator &it)
{
	size_t found;
	size_t end;
	size_t start;
	int content_length = it->second.content_length;
	if ((start = files.find("\r\n\r\n")) != std::string::npos)
	{
		if ((found = files.find("Transfer-Encoding: chunked")) != std::string::npos)
		{
			it->second.transfer_encoding = 1;
			if ((end = files.find("0\r\n\r\n", found)) != std::string::npos)
				return (1);
			return (0);
		}
		else if ((found = files.find("Content-Length: ")) != std::string::npos)
		{
			if (content_length == -1)
			{
				if ((end = files.find("\r\n", found)))
				{
					std::string str = files.substr(found + 16, end - found - 16);
					it->second.content_length = atoi(str.c_str());
					if (files.length() - start - 4 == it->second.content_length)
						return (1);
					else
						return (0);
				}
				return (1);
			}
			else
			{
				if (files.length() - start - 4 == it->second.content_length)
					return (1);
				else
					return (0);
			}
		}
		else
		{
			files = files.substr(0, start + 4); /// go back to this line
			return (1);
		}
	}
	return (0);
}
size_t get_size(std::string str)
{
	size_t x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return (x);
}
std::string unchunk_data(std::string files)
{
	std::string result;
	std::vector<std::string> vec_chunk;
	size_t found;
	size_t start;
	size_t end;
	size_t size;
	if ((found = files.find("\r\n\r\n")) != std::string::npos)
	{
		result = files.substr(0, found + 4);
		start = found + 4;
		while ((end = files.find("\r\n", start)) != std::string::npos)
		{
			size = get_size(files.substr(start, end - start));
			if (size == 0)
				return (result);
			result.append(files.substr(end + 2, size));
			start = end + 2 + size + 2;
		}
	}
	return (result);
}

void connection_handler(int i, RequestHandler &req_handler, int port, fd_set &write_fds, fd_set &current_socket, std::map<int, map_info> &map_of_req)
{
	std::string files;
	map_info info ;
	std::map<int, map_info>::iterator it;
	std::string res = "";

	char buffer[100];
	bzero(buffer, 100);
	int rd = recv(i, buffer, 99, 0);
	if (rd > 0)
	{
		if ((it = map_of_req.find(i)) != map_of_req.end())
		{
			it->second.body.append(buffer, rd);
			files = it->second.body;
			if (check_body(files, it) == 0)
				return;
			FD_CLR(i, &current_socket);
		}
		else
		{
			info.body.append(buffer, rd);
			info.content_length = -1;
			info.transfer_encoding = 0;
			info.number = 0;
			it = map_of_req.insert(std::pair<int, map_info>(i, info)).first;
			files = info.body;
			if (check_body(files, it) == 0)
				return;
		}
	}
	else if (rd == 0)
	{
		FD_CLR(i, &current_socket);
	}
	else
	{
		FD_CLR(i, &current_socket);
		return;
	}
	if (it->second.transfer_encoding == 1)
		files = unchunk_data(files);
	if (!files.empty())
	{
		Request req(files, port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		res = resp.get_header();
		if (FD_ISSET(i, &write_fds))
		{
			std::map<int, map_info>::iterator it_send = map_of_req.find(i);
			size_t n = it_send->second.number;
			n = send(i, res.c_str() + n, res.length(), 0);
			if(n <=0)
			{
				map_of_req.erase(i);
				FD_CLR(i, &current_socket);
				FD_CLR(i, &write_fds);
				close(i);
				return;
			}
			it_send->second.number += n; // NOTE Check on 0 AND -1 

			if (n == res.length())
			{
				map_of_req.erase(i);
				FD_CLR(i, &current_socket);
				FD_CLR(i, &write_fds);
				close(i);
			}
		}
	}
}
