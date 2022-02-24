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

class DELETERequest : public ARequest
{

public:
    DELETERequest(Request &req, std::vector<Server> &servs);

    Response logic_function(Server server, Location location);

    ~DELETERequest();
};

#endif