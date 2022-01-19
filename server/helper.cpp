#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

void connection_handler(int i,RequestHandler &req_handler, int port)
{
	char buffer[1024] = {0};
	int read_val = read(i, buffer, 1024);
	Response resp;
	// std::cout << "we are in the connection handler " << read_val  <<std::endl;
	if (buffer[0] != 0)
	{

		Request req(buffer, port);
		// req.printer();
		// req_handler.setRequest(req);
		// Response resp = req_handler.Bootstrap();
		resp.header_cleaner();

		// std::cout << buffer << std::endl;
		const char *hello = resp.get_header().c_str();
		// std::cout << hello << std::endl;
		send(i, hello, strlen(hello), 0);
		close(i);
	}
}