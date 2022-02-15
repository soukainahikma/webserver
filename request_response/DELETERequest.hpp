#ifndef DELETE_REQUEST_HPP
#define DELETE_REQUEST_HPP

#include "request.hpp"
#include "response.hpp"
#include "AbstractRequest.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class DELETERequest: public AbstractRequest
{

public:
    DELETERequest(Request &req, std::vector<Server> &servs): AbstractRequest(req, servs) {
        this->method = "DELETE";
    };

    Response logic_function(Server server, Location location) {
		struct stat fileStat;

        if (false) {
                
        }
		else {
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
        }
			std::cout << "+++++++++++++++" << std::endl;
		return Response(server, location, "POST");
	}

    ~DELETERequest() {};
};

#endif