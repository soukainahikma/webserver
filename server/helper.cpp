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


void connection_handler(int i,RequestHandler &req_handler, int port)
{
	int size;
	std::string files;
	// std::ofstream file("req.txt");
	int		ret;

	// if (ioctl(i, FIONREAD, &size) == -1)
	// 	throw std::runtime_error("INTERNAL SERVER ERROR");
	char *buffer =(char*) malloc(sizeof(char) * 100);
	bzero(buffer,100);
	while((ret = recv(i, buffer, 99, 0))>0)
	{
		// std::cout<< buffer<<std::endl;
		std::cout<< "ret : " << ret << std::endl;
		files += std::string(buffer);
		bzero(buffer,100);
		if(ret< 99)
			break;
	};
	std::cout<< "last ret : " << ret << std::endl;
	std::cout << files << std::endl;
	// if (buffer[0] != 0)
	// {
	// 	Request req(buffer, port);
	// 	req_handler.setRequest(req);
	// 	Response resp = req_handler.Bootstrap();
	// 	const char *hello = resp.get_header().c_str();
	// 	// std::cout << hello << std::endl;
	// 	send(i, hello, strlen(hello), 0);
	// 	close(i);
	// }
	free(buffer);
}
