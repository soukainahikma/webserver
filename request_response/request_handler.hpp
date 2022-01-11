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

	Response generateResponse () {
		return Response();
	}

	Response GETRequesHandler () {
		Request::map_request req_map = req.getRequest();
		size_t found;
		std::string server_name;
		std::vector<std::string> server_names;
		size_t index;

		server_name = req_map["Host"];
		if ((found = server_name.find(":"))!= std::string::npos)
				server_name = server_name.substr(0,found);	
		for (size_t i = 0; i < Servs.size(); i++)
		{
			server_names = Servs[i].get_server_name();
			if (std::find(server_names.begin(), server_names.end(), server_name) != server_names.end()) {
				std::vector<Location> locations = Servs[i].get_location();
				for (size_t j = 0; j < locations.size(); j++)
				{
					if (req_map["URL"] == locations[j].get_path())
						return Response(Servs[i].get_root() + locations[j].get_path() +"/index.html");
				}
			}
		}
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

	~RequestHandler()
	{};
};

#endif