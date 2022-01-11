#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>

class RequestHandler
{
private:
	Request req;
	std::vector<t_server> servs;
	std::vector<Server> Servs;
	
public:
	RequestHandler(std::vector<t_server> &servs) { this->servs = servs;};

	RequestHandler(std::vector<Server> &Servs) { this->Servs = Servs;};

	RequestHandler(Request &req, std::vector<t_server> &serv) {
		this->req = req;
		this->servs = serv;
	};

	RequestHandler(Request &req, std::vector<Server> &Serv) {
		this->req = req;
		this->Servs = Serv;
	};

	void setRequest(Request &req) {
		this->req = req;
	}

	Response generateResponse () {
		return Response();
	}

	Response GETRequesHandler () {
		Request::map_request req_map = req.getRequest();
		for (size_t i = 0; i < Servs.size(); i++)
		{
			std::vector<Location> locations = Servs[i].get_location();
			for (size_t j = 0; j < locations.size(); j++)
			{
				if (req_map["URL"] == locations[j].get_path())
					return Response(Servs[i].get_root() + locations[j].get_path() +"/index.html");
			}
		}
		std::cout << Servs[Servs.size() - 1].get_error_page()[0] << Servs[Servs.size() - 1].get_error_page()[1] << std::endl;
		std::vector<std::string> errorPages = Servs[Servs.size() - 1].get_error_page();
		size_t i;
		for (i = 0; i < errorPages.size(); i++)
		{
			std::cout << errorPages[i] << std::endl;
			if (errorPages[i] == "502")
				break;
		}
		return Response(502, Servs[Servs.size() - 1].get_root() + errorPages[i + 1]);
	}

	Response Bootstrap() {	
		if (req.getRequest()["Method"] == "GET")
			return GETRequesHandler();
		return GETRequesHandler();
	}

	~RequestHandler() {
		
	};
};

#endif