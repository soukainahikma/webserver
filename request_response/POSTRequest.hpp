#ifndef POST_REQUEST_HPP
#define POST_REQUEST_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class POSTRequest
{
private:
	Request req;
	std::vector<Server> servs;

public:
    POSTRequest(Request &req, std::vector<Server> &servs) {
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

    Response POSTRequestHelper(Server server, Location location) {
		struct stat fileStat;
	
		std::cout << "WHAT HAPPENED => " << req.getRequest()["Content-Type"] << std::endl;
		if (req.getRequest()["Content-Type"] == "multipart/form-data")
		{
			std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
			{
				if (body[i].filename != "")
				{
					std::cout << RED << " +++ IT IS A FILE +++" << RESET << std::endl;
					std::cout << "Client size Body => " << body[i].body.length() << std::endl;
					std::cout << "Max Client => " <<server.get_client_max_body_size() << std::endl;
					if (body[i].body.length() > 10)
						return Response(server, server.get_root() + server.get_error_page()["413"], "POST", "413");
					if(stat((server.get_root() + location.get_path()).c_str(), &fileStat) < 0)    
        				return Response(server, server.get_root() + server.get_error_page()["502"], "POST", "502");
					if(!(fileStat.st_mode & S_IWUSR))    
        				return Response(server, server.get_root() + server.get_error_page()["403"], "POST", "403");
					int fd = open((server.get_root() + "/" + location.get_path() + "/" + body[i].filename).c_str(), O_CREAT | O_RDWR, 0644);
					std::cout << "Get the file's path => " << (server.get_root() + "/" + body[i].filename) << std::endl;
					write(fd, body[i].body.c_str(), body[i].body.length());
					std::cout << RED << " ++++++++++++++++++++" << RESET << std::endl;
				}
				else
				{
					std::cout << "IT IS NOT A FILE" << std::endl;
				}
			}
			std::cout << "SIZE => "<< req.getBodyStructs().size() << std::endl;
		}
		else {
			std::cout << "BODY AS STRING => "<< req.getBodyString() << std::endl;
		}
			std::cout << "+++++++++++++++" << std::endl;
		return Response(server, location, "POST");
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
						if (checkAllowedMethods(locations[j].get_request_method(), "POST"))
							return POSTRequestHelper(servs[i], locations[j]);
						return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["403"], "POST", "403");
					}
				}
				return Response(servs[i], servs[i].get_root() + req_map["URL"], "POST", "201");
			}
		}
		return Response(servs[k], servs[k].get_root() + defaultErrorPages["502"], "POST", "502");
	}


    ~POSTRequest() {

    };
};

#endif