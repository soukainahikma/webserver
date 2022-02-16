#ifndef A_HANDLER_HPP
#define A_HANDLER_HPP
#include "request.hpp"
#include "response.hpp"
#include "../server/socket.hpp"
#include "../parsing/Server.hpp"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

class ARequest
{
    public:
        ARequest(Request &req, std::vector<Server> &servs) {
            this->req = req;
            this->servs = servs;
        };

        bool checkAllowedMethods (std::vector<std::string> &allowed_mehods, std::string method) {
            for (size_t i = 0; i < allowed_mehods.size(); i++)
            {
                if (allowed_mehods[i] == method)
                    return (true);
            }
            return (false);
        };

        virtual Response logic_function(Server server, Location location) = 0;

        Response returnedResponse () {
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
            std::map<std::string, std::string> defaultErrorPages = servs[0].get_error_page();

            for (size_t i = 0; i < servs.size(); i++)
            {
                if (req.get_port() == servs[i].get_listen() && servs[i].get_server_name()[server_name])
                {
                    if (k == -1)
                        k = i;
                    std::vector<Location> locations = servs[i].get_location();
                    for (size_t j = 0; j < locations.size(); j++)
                    {
                        if (req_map["URL"] == locations[j].get_path())
                        {
                            if (checkAllowedMethods(locations[j].get_request_method(), this->method))
                                return logic_function(servs[i], locations[j]);
                            return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["405"], this->method, "405");
                        }
                    }
                    return Response(servs[i], servs[i].get_root() + req_map["URL"], this->method, "200");
                }
            }
            k = k == -1 ? 0 : k;
            return Response(servs[k], servs[k].get_root() + defaultErrorPages["404"], this->method, "404");
        }

        ~ARequest() {

        };
    protected:
        Request req;
        std::vector<Server> servs;
        std::string method;
};

#endif