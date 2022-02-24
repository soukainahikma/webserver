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
    POSTRequest(Request &req, std::vector<Server> &servs);
    Response logic_function(Server server, Location location) ;
    ~POSTRequest();
};

#endif