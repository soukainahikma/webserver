#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"
#include <sys/ioctl.h>

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

	// if (ioctl(i, FIONREAD, &size) == -1)
	// 	throw std::runtime_error("INTERNAL SERVER ERROR");
	// char *buffer = (char *)malloc(size * sizeof(char));
	// int read_val = read(i, buffer, size);
	std::string files;
	int		ret;

	if (ioctl(i, FIONREAD, &size) == -1)
		throw std::runtime_error("INTERNAL SERVER ERROR");
	char *buffer =(char*) malloc(sizeof(char) * 1025);
	bzero(buffer,1025);
	while((ret = recv(i, buffer, 1024, 0))>0)
	{
		// std::cout<< buffer<<std::endl;
		std::cout << "ret: { " << ret << " } gurrela debugging \n";
		files += std::string(buffer);
		bzero(buffer,1025);
		if(ret< 1024)
			break;
	};

	std::cout << "out \n";

	if (!files.empty())
	// if (buffer[0] != 0)
	{
		Request req(files, port);
		// Request req(std::string(buffer), port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		const char *hello = resp.get_header().c_str();
		// std::cout << hello << std::endl;
		send(i, hello, strlen(hello), 0);
		close(i);
	}
	free(buffer);
}
