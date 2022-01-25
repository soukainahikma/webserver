#include "socket.hpp"
#include "../request_response/response_beta.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

int fileCheck(std::string fileName)
{
	if (!access(fileName.c_str(), F_OK))
	{
		if (!access(fileName.c_str(), R_OK))
			return OK;
		return FORBIDDEN;
	}
	return NOT_FOUND;
}


void connection_handler(int i,RequestHandler &req_handler, int port)
{
	char buffer[1024] = {0};
	int read_val = read(i, buffer, 1024);
	if (buffer[0] != 0)
	{
		Request req(buffer, port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		std::string hello = resp.get_header();
		std::cout << hello << std::endl;
		send(i, hello.c_str(), strlen(hello.c_str()), 0);
		close(i);
	}
}
