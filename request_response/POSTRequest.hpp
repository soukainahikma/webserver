#ifndef POST_REQUEST_HPP
#define POST_REQUEST_HPP
#include "ARequest.hpp"
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class POSTRequest: public ARequest
{
public:
    POSTRequest(Request &req, std::vector<Server> &servs): ARequest(req, servs) {
       this->method = "POST";
    };

    Response logic_function(Server server, Location location) {
		struct stat fileStat;
		std::string upload_path = location.get_upload_store() != "" ? location.get_upload_store() : location.get_path();
	
		if (req.getRequest()["Content-Type"] == "multipart/form-data")
		{
			std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
			{
				if (body[i].filename != "" && location.get_upload_enable() == "on")
				{
					std::cout << RED << " +++ { IT IS A FILE } +++" << RESET << std::endl;
					if (body[i].body.length() > 10 && false) // server.get_client_max_body_size()
						return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413", req);
					if(stat((server.get_root() + upload_path).c_str(), &fileStat) < 0)    
        				return Response(server, server.get_root() + server.get_error_page()["502"], this->method, "502", req);
					if(!(fileStat.st_mode & S_IWUSR))
        				return Response(server, server.get_root() + server.get_error_page()["403"], this->method, "403", req);
					int fd = open((server.get_root() + "/" + upload_path + "/" + body[i].filename).c_str(), O_CREAT | O_RDWR, 0644);
					std::cout << "Get the file's path => " << (server.get_root() + "/" + body[i].filename) << std::endl;
					write(fd, body[i].body.c_str(), body[i].body.length());
					std::cout << RED << " ++++++++++++++++++++" << RESET << std::endl;
				}
				else if (location.get_upload_enable() != "on")
					return Response(server, server.get_root() + server.get_error_page()["405"], this->method, "405", req);
				else
				{
					std::cout << "IT IS NOT A FILE" << std::endl;
					std::cout << body[i].body.c_str()[0] << std::endl;
					// CGI
				}
			}
		}
		else {
			std::cout << "BODY AS STRING => "<< req.getBodyString() << std::endl;
			// CGI
		}
		std::cout << "+++++++++++++++" << std::endl;
		return Response(server, location, this->method, req);
	}

    ~POSTRequest() {

    };
};

#endif