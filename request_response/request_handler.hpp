#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include <vector>

class RequestHandler
{
private:
	Request req;
	std::vector<t_server> servs;
	
public:

	RequestHandler(std::vector<t_server> &servs) {
		this->servs = servs;
	};

	RequestHandler(Request &req, std::vector<t_server> &serv) {
		this->req = req;
		this->servs = serv;
	};

	void setRequest(Request &req) {
		this->req = req;
	}

	Response generateResponse () {
		return Response();
	}
	
	Response GetRequesHandler () {
		Request::map_request req_map = req.getRequest();
		for (size_t i = 0; i < servs.size(); i++)
		{
			/* if (std::find(servs[i]._server_names.begin(), servs[i]._server_names.end(), "abc") != servs[i]._server_names.end())
			{

			} */
			for (size_t j = 0; j < servs[i].locations.size(); j++)
			{
				if (req_map["URL"] == servs[i].locations[j].path)
				{
					return Response(servs[i]._root + "/index.html");
				}
			}
		}
		
		return Response(404, servs[]._root);
	}

	Response Bootstrap() {	
		// if (req_map["Method"] == "GET")
			return GetRequesHandler();
	}

	~RequestHandler() {
		
	};
};

#endif