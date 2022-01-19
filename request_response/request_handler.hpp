#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>

/* ************ Successful responses ******** */

#define OK 200
#define CREATED 201

/* *********** Redirection messages ********* */

#define MOVED_PERMANENTLY 301

/* ********** Client error responses ******** */

#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404

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
		std::cout << this->req.get_port() << std::endl;
	}

	Response GETRequesHandler () {
		Request::map_request req_map = req.getRequest();
		size_t found;
		std::string server_name;
		std::map<std::string, int> server_names;
		size_t index;

		server_name = req_map["Host"];
		if ((found = server_name.find("\r"))!= std::string::npos)
				server_name = server_name.substr(0,found);	
		if ((found = server_name.find(":"))!= std::string::npos)
				server_name = server_name.substr(0,found);
		std::map<std::string, std::string> defaultErrorPages = Servs[0].get_error_page();
		for (size_t i = 0; i < Servs.size(); i++)
		{
			if (req.get_port() == Servs[i].get_listen() && Servs[i].get_server_name()[server_name]) {
				std::vector<Location> locations = Servs[i].get_location();
				for (size_t j = 0; j < locations.size(); j++)
				{
					if (req_map["URL"] == locations[j].get_path())
					{
						locations[j].get_index();
						return Response(200, Servs[i].get_root() + locations[j].get_path());
					}
				}
				return Response(404, Servs[i].get_root() + Servs[i].get_error_page()["404"]);
			}
		}
		return Response(502, Servs[0].get_root() + defaultErrorPages["502"]);
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