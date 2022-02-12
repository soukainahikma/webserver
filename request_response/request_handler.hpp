#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include "GETRequest.hpp"
#include "POSTRequest.hpp"
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

	bool checkAllowedMethods (std::vector<std::string> &allowed_mehods, std::string method) {
		for (size_t i = 0; i < allowed_mehods.size(); i++)
		{
			if (allowed_mehods[i] == method)
				return (true);
		}
		return (false);
	}

	Response DELETERequestHelper(Server server, Location location) {
		struct stat fileStat;

		if (req.getRequest()["Content-Type"] == "multipart/form-data")
		{
			std::vector<body_struct> body = req.getBodyStructs();
			// CGI
			std::cout << "SIZE => "<< req.getBodyStructs().size() << std::endl;
		}
		else {
			std::cout << "BODY AS STRING => "<< req.getBodyString() << std::endl;
			// CGI
		}
			std::cout << "+++++++++++++++" << std::endl;
		return Response(server, location, "POST");
	}

	Response DELETERequestHandler () {
		Request::map_request req_map = req.getRequest();
		size_t found;
		std::string server_name;
		std::map<std::string, int> server_names;
		size_t index;
		int k = -1;

		server_name = req_map["Host"];
		if ((found = server_name.find("\r"))!= std::string::npos)
				server_name = server_name.substr(0,found);	
		if ((found = server_name.find(":"))!= std::string::npos)
				server_name = server_name.substr(0,found);
		std::map<std::string, std::string> defaultErrorPages = Servs[0].get_error_page();

		for (size_t i = 0; i < Servs.size(); i++)
		{
			if (req.get_port() == Servs[i].get_listen() && Servs[i].get_server_name()[server_name])
			{
				if (k == -1)
					k = i;
				std::vector<Location> locations = Servs[i].get_location();
				for (size_t j = 0; j < locations.size(); j++)
				{
					if (req_map["URL"] == locations[j].get_path())
					{
						if (checkAllowedMethods(locations[j].get_request_method(), "DELETE"))
							return DELETERequestHelper(Servs[i], locations[j]);
						return Response(Servs[i], Servs[i].get_root() + Servs[i].get_error_page()["403"], "DELETE", "403");
					}
				}
				return Response(Servs[i], Servs[i].get_root() + req_map["URL"], "DELETE", "200");
			}
		}
		return Response(Servs[k], Servs[k].get_root() + defaultErrorPages["502"], "DELETE", "502");
	}

	Response Bootstrap() {	
		if (req.getRequest()["Method"] == "GET")
			return GETRequest(req, Servs).returnedResponse();
		if (req.getRequest()["Method"] == "POST")
			return POSTRequest(req, Servs).returnedResponse();
		if (req.getRequest()["Method"] == "DELETE")
			return DELETERequestHandler();
		return GETRequest(req, Servs).returnedResponse();
	}

	~RequestHandler()
	{};
};

#endif