#ifndef DELETE_REQUEST_HPP
#define DELETE_REQUEST_HPP

#include "request.hpp"
#include "response.hpp"
#include "ARequest.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class DELETERequest: public ARequest
{

public:
    DELETERequest(Request &req, std::vector<Server> &servs): ARequest(req, servs) {
        this->method = "DELETE";
    };

    Response logic_function(Server server, Location location) {
		struct stat fileStat;

        if (req.getBodyString().length() > 10 && false) // server.get_client_max_body_size()
				return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413", req);
        if (false && (req.getRequest()["Content-Type"] == "multipart/form-data")) {
            std::vector<body_struct> body = req.getBodyStructs();
			for (size_t i = 0; i < body.size(); i++)
            {
				if(stat((server.get_root() + location.get_path()).c_str(), &fileStat) < 0)    
        			return Response(server, server.get_root() + server.get_error_page()["500"], this->method, "500", req);
				if(!(fileStat.st_mode & S_IWUSR))
        			return Response(server, server.get_root() + server.get_error_page()["403"], this->method, "403", req);
            }
        }
		return Response(server, location, this->method, req);
	}

    ~DELETERequest() {};
};

#endif