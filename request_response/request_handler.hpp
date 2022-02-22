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
	RequestHandler(std::vector<Server> &Servs) { this->Servs = Servs;};
	RequestHandler(Request &req, std::vector<Server> &Serv) {
		this->req = req;
		this->Servs = Serv;
	};

	void setRequest(Request &req) {
		this->req = req;
	}

	Response Bootstrap() {	
		if (req.getRequest()["Method"] == "GET")
			return GETRequest(req, Servs).returnedResponse();
		if (req.getRequest()["Method"] == "POST")
			return POSTRequest(req, Servs).returnedResponse();
		if (req.getRequest()["Method"] == "DELETE")
			return DELETERequest(req, Servs).returnedResponse();
		return GETRequest(req, Servs).returnedResponse();
	}

	~RequestHandler()
	{};
};

#endif