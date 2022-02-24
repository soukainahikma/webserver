#include "ARequest.hpp"

ARequest::ARequest(Request &req, std::vector<Server> &servs)
{
	this->req = req;
	this->servs = servs;
};

bool ARequest::checkAllowedMethods(std::vector<std::string> &allowed_mehods, std::string method)
{
	for (size_t i = 0; i < allowed_mehods.size(); i++)
	{
		if (allowed_mehods[i] == method)
			return (true);
	}
	return (false);
};


std::string ARequest::server_name_extractor()
{
	size_t found;
	std::string server_name;

	server_name = req.getRequest()["Host"];
	if ((found = server_name.find("\r")) != std::string::npos)
		server_name = server_name.substr(0, found);
	if ((found = server_name.find(":")) != std::string::npos)
		server_name = server_name.substr(0, found);
	return (server_name);
}

size_t ARequest::get_num_of_servers_per_port()
{
	size_t count = 0;
	for (size_t i = 0; i < servs.size(); i++)
	{
		if (req.get_port() == servs[i].get_listen())
			count++;
	}
	return (count);
}

Response ARequest::returnedResponse()
{
	Request::map_request req_map = req.getRequest();
	std::string server_name;
	size_t count = get_num_of_servers_per_port();
	int k = -1;

	server_name = server_name_extractor();
	
	std::map<std::string, std::string> defaultErrorPages;
	
	for (size_t i = 0; i < servs.size(); i++)
	{
		if (req.get_port() == servs[i].get_listen())
		{
			if (k == -1)
				k = i;
			if (count > 1 && !servs[i].get_server_name()[server_name])
				continue;
			if (req.getBodyString().length() > servs[i].get_client_max_body_size())
				return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["413"], this->method, "413", req);
			if (req.getRequest()["Method"] != "GET" && this->method == "GET")
				return Response(servs[k], servs[k].get_root() + servs[k].get_error_page()["400"], this->method, "400", req);
			std::vector<Location> locations = servs[i].get_location();
			for (size_t j = 0; j < locations.size(); j++)
			{
				if (req_map["URL"] == locations[j].get_path() || req_map["URL"] == locations[j].get_path() + "/")
				{
					if (checkAllowedMethods(locations[j].get_request_method(), this->method))
						return logic_function(servs[i], locations[j]);
					return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["405"], this->method, "405", req);
				}
			}
			return Response(servs[i], servs[i].get_root() + req_map["URL"], this->method, this->method == "POST" ? "201" : "200", req);
		}
	}
	k = k == -1 ? 0 : k;
	return Response(servs[k], servs[k].get_root() + servs[k].get_error_page()["404"], this->method, "404", req);
}

std::string ARequest::getQuery()
{
	return (this->req.getQueryVar());
}

ARequest::~ARequest(){};
