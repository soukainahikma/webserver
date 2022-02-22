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
        return Response(server, location, this->method, req);
    }
    ~GETRequest() {
    };
};

#endif