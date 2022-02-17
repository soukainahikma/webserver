#ifndef POST_REQUEST_HPP
#define POST_REQUEST_HPP
#include "AbstractRequest.hpp"
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class POSTRequest: public AbstractRequest
{
public:
    POSTRequest(Request &req, std::vector<Server> &servs): AbstractRequest(req, servs) {
       this->method = "POST";
    };

    Response logic_function(Server server, Location location) {
		struct stat fileStat;
	
		// std::cout << "WHAT HAPPENED => " << req.getRequest()["Content-Type"] << std::endl;
		if (req.getRequest()["Content-Type"] == "multipart/form-data" && true)
		{
			std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
			{
				if (body[i].filename != "")
				{
					// std::cout << RED << " +++ IT IS A FILE +++" << RESET << std::endl;
					// std::cout << "Client size Body => " << body[i].body.length() << std::endl;
					// std::cout << "Max Client => " <<server.get_client_max_body_size() << std::endl;
					std::cout << " ++++++++++++ \n";
					std::cout << body[i].body.length() << std::endl;
					std::cout << server.get_client_max_body_size() << std::endl;
					std::cout << " ++++++++++++ \n";
					if (body[i].body.length() > server.get_client_max_body_size())
						return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413");
					if(stat((server.get_root() + location.get_path()).c_str(), &fileStat) < 0)    
        				return Response(server, server.get_root() + server.get_error_page()["502"], this->method, "502");
					if(!(fileStat.st_mode & S_IWUSR))    
        				return Response(server, server.get_root() + server.get_error_page()["403"], this->method, "403");
					int fd = open((server.get_root() + "/" + location.get_path() + "/" + body[i].filename).c_str(), O_CREAT | O_RDWR, 0644);
					// std::cout << "Get the file's path => " << (server.get_root() + "/" + body[i].filename) << std::endl;
					write(fd, body[i].body.c_str(), body[i].body.length());
					// std::cout << RED << " ++++++++++++++++++++" << RESET << std::endl;
				}
				else
				{
					// std::cout << "IT IS NOT A FILE" << std::endl;
				}
			}
			// std::cout << "SIZE => "<< req.getBodyStructs().size() << std::endl;
		}
		else {
			// std::cout << "BODY AS STRING => "<< req.getBodyString() << std::endl;
		}
			// std::cout << "+++++++++++++++" << std::endl;
		return Response(server, location, this->method);
	}

    ~POSTRequest() {

    };
};

#endif