#include "socket.hpp"
#include "../request_response/response_beta.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

void connection_handler(int i,RequestHandler &req_handler, int port)
{
	char buffer[1024] = {0};
	int read_val = read(i, buffer, 1024);
	if (buffer[0] != 0)
	{
		Request req(buffer, port);
		req_handler.setRequest(req);
		Response resp = req_handler.Bootstrap();
		const char *hello = resp.get_header().c_str();
		send(i, hello, strlen(hello), 0);
		close(i);
	}
}