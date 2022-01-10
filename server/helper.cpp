#include "socket.hpp"
#include "../request_response/response.hpp"
#include "../request_response/request.hpp"
#include "../request_response/request_handler.hpp"

void request_printer(Request req)
{
	std::cout << " ****** Request Printer ******** " << std::endl;
	Request::map_request map_ = req.getRequest();
	for (std::map<std::string,std::string>::iterator it= map_.begin(); it!=map_.end(); ++it)
    	std::cout << it->first << " => " << it->second << '\n';
	std::cout << " ******************************* " << std::endl;
}

void location_printer(t_location loc) {
	std::cout << " ------ Server Printer ------ " << std::endl;
	std::cout << "path :" << loc.path << std::endl;
	std::cout << "autoindex :" << loc.autoindex << std::endl;
	std::cout << " ---------------------------- " << std::endl;
}

void server_printer(t_server serv) {
	std::cout << " ****** Server Printer ********* " << std::endl;
	std::cout << "listen :" << serv._listen << std::endl;
	std::cout << "host :" << serv._host << std::endl;
	std::cout << "root :" << serv._root << std::endl;
	for (size_t i = 0; i < serv.locations.size(); i++)
		location_printer(serv.locations[i]);
	std::cout << " ******************************* " << std::endl;
}

void connection_handler(int fd,std::vector<t_server> parse_server,RequestHandler req_handler)
{
	Response resp;
	char buffer[1024] = {0};
	read(fd, buffer, 1024);
	// Request req(buffer);
	/* ****** Request Printer ******** */
	// request_printer(req);
	// server_printer(parse_server[0]);
	/* ******************************* */
	// req_handler.setRequest(req);
	// resp = req_handler.Bootstrap();
	const char *hello = strdup("hello");//resp.get_header().c_str();
	send(fd, hello, strlen(hello), 0);
	resp.header_cleaner();
	close(fd);
}