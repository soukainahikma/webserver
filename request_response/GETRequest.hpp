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

class GETRequest : public ARequest
{
public:
    GETRequest(Request &req, std::vector<Server> &servs);
    Response logic_function(Server server, Location location);
    ~GETRequest();
};

#endif