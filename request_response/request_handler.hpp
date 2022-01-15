#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>

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
		std::map<std::string, int> server_names;
		size_t index;

		server_name = req_map["Host"];
		
		if ((found = server_name.find(":"))!= std::string::npos)
				server_name = server_name.substr(0,found);	
		for (size_t i = 0; i < Servs.size(); i++)
		{
			server_names = Servs[i].get_server_name();
			if (server_names.find(server_name) != server_names.end()) {
				// std::vector<Location> locations = Servs[i].get_location();
				std::map<std::string, Location> locs = Servs[i].get_location_map();
				if (locs.find(req_map["URL"]) != locs.end()) {
					std::vector<std::string> allowed_methods = locs[req_map["URL"]].get_request_method();
					if ((allowed_methods.size() == 0 || std::find(allowed_methods.begin(), allowed_methods.end(), req_map["Method"]) != allowed_methods.end()))
						return Response(Servs[i].get_root() + locs[req_map["URL"]].get_path() +"/index.html"); // indexes Autoindexn
					std::cout << Servs[i].get_error_page()["500"] << std::endl;
					return Response(500, Servs[Servs.size() - 1].get_root() + Servs[i].get_error_page()["500"]);
				}
				// for (size_t j = 0; j < locations.size(); j++)
				// {
				// 	std::vector<std::string> allowed_methods = locations[j].get_request_method();
				// 	if (req_map["URL"] == locations[j].get_path()) // Allowed Method
				// 	{
				// 		if ((allowed_methods.size() == 0 || std::find(allowed_methods.begin(), allowed_methods.end(), req_map["Method"]) != allowed_methods.end()))
				// 			return Response(Servs[i].get_root() + locations[j].get_path() +"/index.html"); // indexes Autoindexn
				// 		std::cout << Servs[i].get_error_page()["500"] << std::endl;
				// 		return Response(500, Servs[Servs.size() - 1].get_root() + Servs[i].get_error_page()["500"]);
				// 	}	
				// }
			}
		}
		std::map<std::string, std::string> errorPages = Servs[Servs.size() - 1].get_error_page();
		return Response(404, Servs[Servs.size() - 1].get_root() + errorPages["404"]);
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