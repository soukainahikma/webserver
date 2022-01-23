#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

void connection_handler(int i,RequestHandler &req_handler, int port)
{
	char buffer[3000] = {0};
	int read_val = read(i, buffer, 3000);
	Response resp;
	// std::cout << "we are in the connection handler " << read_val  <<std::endl;
	if (buffer[0] != 0)
	{
		// std::cout << buffer << std::endl;

		Request req(buffer, port);
		// std::cout<< "----------------------------------------"<<std::endl;
		// std::cout<< buffer<<std::endl;
		// req_handler.setRequest(req);
		// Response resp = req_handler.Bootstrap();
		// resp.header_cleaner();

		// req.printer();
		// std::cout << buffer << std::endl;
		// std::string hello = resp.get_header().c_str();
		// std::cout << hello << std::endl;
		// send(i, hello.c_str(), strlen(hello.c_str()), 0);
		close(i);
	}
}