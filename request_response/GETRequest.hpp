#ifndef GET_REQUEST_HPP
#define GET_REQUEST_HPP

#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include "ARequest.hpp"
#include "request.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class GETRequest: public ARequest
{
public:
    GETRequest(Request &req, std::vector<Server> &servs): ARequest(req, servs) {
        this->method = "GET";
    };

    Response logic_function(Server server, Location location) {
        if (req.getBodyString().length() > server.get_client_max_body_size())
				return Response(server, server.get_root() + server.get_error_page()["413"], this->method, "413", req);
        return Response(server, location, this->method, req);
    }
    ~GETRequest() {
    };
};

#endif