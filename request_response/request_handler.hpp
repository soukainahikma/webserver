#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include "GETRequest.hpp"
#include "POSTRequest.hpp"
#include "DELETERequest.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class RequestHandler
{
private:
	Request req;
	std::vector<Server> Servs;
	
public:
	RequestHandler(std::vector<Server> &Servs);
	RequestHandler(Request &req, std::vector<Server> &Serv);

	void setRequest(Request &req) ;

	std::vector<Server> get_servers() const;

	size_t get_default_index(int port);

	Response Bootstrap();

	~RequestHandler();
};

#endif