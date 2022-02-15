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
		if (!access(fileName.c_str(), R_OK	))
			return req_type == "GET" ? OK : CREATED;
		return FORBIDDEN;
	}
	return NOT_FOUND;
}


void connection_handler(int i,RequestHandler &req_handler, int port,fd_set write_fds,std::map<int,std::string> map_of_req)
{
	std::string files;
	std::map<int ,std::string>::iterator it;
	char *buffer =(char*) malloc(sizeof(char) * 1025);
	bzero(buffer,1025);
	if((recv(i, buffer, 1024, 0))>=0)
	{
		if((it = map_of_req.find(i))!=map_of_req.end())
		{
			it->second+=std::string(buffer);
			files = it->second;
		}
		else
		{
			map_of_req[i] = std::string(buffer);
			files = std::string(buffer);
		}
		bzero(buffer,1025);

	};
	std::cout<< files << std::endl;
	//throw exception for the read == -1
	if (!files.empty())
	{
		Request req(files, port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		const char *hello = resp.get_header().c_str();
		if(FD_ISSET(i,&write_fds))
		{
			send(i, hello, strlen(hello), 0);
			close(i);
		}
	}
	free(buffer);
}
