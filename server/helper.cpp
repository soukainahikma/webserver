#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

void connection_handler(int i,RequestHandler &req_handler)
{
	Response resp;
	char buffer[1024] = {0};
	read(i, buffer, 1024);
	Request req(buffer);
	req_handler.setRequest(req);
	resp = req_handler.Bootstrap();
	const char *hello = resp.get_header().c_str();
	send(i, hello, strlen(hello), 0);
	resp.header_cleaner();
	close(i);

}