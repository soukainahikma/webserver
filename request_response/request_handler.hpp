#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
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

	Response GETRequesHandler () {
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
						if (checkAllowedMethods(locations[j].get_request_method(), "GET"))
							return Response(Servs[i], locations[j], "GET");
						return Response(Servs[i], Servs[i].get_root() + Servs[i].get_error_page()["405"], "GET", "405");
					}
				}
				return Response(Servs[i], Servs[i].get_root() + req_map["URL"], "GET", "200");
			}
		}
		return Response(Servs[k], Servs[k].get_root() + defaultErrorPages["502"], "GET", "502");
	}

	Response POSTRequestHelper(Server server, Location location) {
		struct stat fileStat;
	
		std::cout << "WHAT HAPPENED => " << req.getRequest()["Content-Type"] << std::endl;
		// |Content-Type| => multipart/form-data
		// 413 EXCEEEDED the size
		if (req.getRequest()["Content-Type"] == "multipart/form-data")
		{
			std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
			{
				if (body[i].filename != "")
				{
					std::cout << RED << " +++ IT IS A FILE +++" << RESET << std::endl;
					// std::cout << server.get_root() + location.get_path() + body[i].filename << std::endl;
					std::cout << body[i].body << std::endl;
					if (body[i].body.length() > server.get_client_max_body_size())
						return Response(server, server.get_root() + server.get_error_page()["413"], "POST", "413");
					if(stat((server.get_root() + location.get_path()).c_str(), &fileStat) < 0)    
        				return Response(server, server.get_root() + server.get_error_page()["502"], "POST", "502");
					if(!(fileStat.st_mode & S_IWUSR))    
        				return Response(server, server.get_root() + server.get_error_page()["403"], "POST", "403");
					int fd = open((server.get_root() + location.get_path() + body[i].filename).c_str(), O_CREAT | O_RDWR, 0644);
					write(fd, body[i].body.c_str(), body[i].body.length());
					// std::cout << RED << " ++++++++++++++++++++" << RESET << std::endl;
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

	Response POSTRequesHandler () {
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
						if (checkAllowedMethods(locations[j].get_request_method(), "POST"))
							return POSTRequestHelper(Servs[i], locations[j]);
						return Response(Servs[i], Servs[i].get_root() + Servs[i].get_error_page()["403"], "POST", "403");
					}
				}
				return Response(Servs[i], Servs[i].get_root() + req_map["URL"], "POST", "201");
			}
		}
		return Response(Servs[k], Servs[k].get_root() + defaultErrorPages["502"], "POST", "502");
	}
	Response Bootstrap() {	
		if (req.getRequest()["Method"] == "GET")
			return GETRequesHandler();
		if (req.getRequest()["Method"] == "POST")
			return POSTRequesHandler();
		return GETRequesHandler();
	}

	~RequestHandler()
	{};
};

#endif