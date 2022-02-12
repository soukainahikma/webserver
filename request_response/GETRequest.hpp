#ifndef GET_REQUEST_HPP
#define GET_REQUEST_HPP

#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class GETRequest
{
private:
	Request req;
	std::vector<Server> servs;

public:
    GETRequest(Request &req, std::vector<Server> &servs) {
        this->req = req;
        this->servs = servs;
    };

    bool checkAllowedMethods (std::vector<std::string> &allowed_mehods, std::string method) {
		for (size_t i = 0; i < allowed_mehods.size(); i++)
		{
			if (allowed_mehods[i] == method)
				return (true);
		}
		return (false);
	}


    Response returnedResponse () {
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
		std::map<std::string, std::string> defaultErrorPages = servs[0].get_error_page();
		for (size_t i = 0; i < servs.size(); i++)
		{
			if (req.get_port() == servs[i].get_listen() && servs[i].get_server_name()[server_name])
			{
				if (k == -1)
					k = i;
				std::vector<Location> locations = servs[i].get_location();
				for (size_t j = 0; j < locations.size(); j++)
				{
					if (req_map["URL"] == locations[j].get_path())
					{
						if (checkAllowedMethods(locations[j].get_request_method(), "GET"))
							return Response(servs[i], locations[j], "GET");
						return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["405"], "GET", "405");
					}
				}
				return Response(servs[i], servs[i].get_root() + req_map["URL"], "GET", "200");
			}
		}
		return Response(servs[k], servs[k].get_root() + defaultErrorPages["502"], "GET", "502");
	}

    ~GETRequest() {
    };
};

#endif