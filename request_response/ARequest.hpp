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

        std::string server_name_extractor() {
            size_t found;
            std::string server_name;

            server_name = req.getRequest()["Host"];
            if ((found = server_name.find("\r"))!= std::string::npos)
                    server_name = server_name.substr(0,found);	
            if ((found = server_name.find(":"))!= std::string::npos)
                    server_name = server_name.substr(0,found);
            return (server_name);
        }

        Response returnedResponse () {
            Request::map_request req_map = req.getRequest();
            std::string server_name;
            int k = -1;

            server_name = server_name_extractor();
            std::map<std::string, std::string> defaultErrorPages;
            for (size_t i = 0; i < servs.size(); i++)
            {
                if (req.get_port() == servs[i].get_listen())
                {
                    if (k == -1)
                    {
                        k = i;
                        defaultErrorPages = servs[k].get_error_page();
                    }
                    if (req.getRequest()["Method"] != "GET" && this->method == "GET")
                        return Response(servs[k], servs[k].get_root() + defaultErrorPages["400"], this->method, "400", req);
                    std::vector<Location> locations = servs[i].get_location();
                    for (size_t j = 0; j < locations.size(); j++)
                    {   
                        if (req_map["URL"] == locations[j].get_path() || req_map["URL"] == locations[j].get_path() + "/")
                        {
                            if (checkAllowedMethods(locations[j].get_request_method(), this->method))
                                if (req.getBodyString().length() > servs[i].get_client_max_body_size())
			                        return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["413"], this->method, "413", req);
                                return logic_function(servs[i], locations[j]);
                            return Response(servs[i], servs[i].get_root() + servs[i].get_error_page()["405"], this->method, "405", req);
                        }
                    }
                    return Response(servs[i], servs[i].get_root() + req_map["URL"], this->method, this->method == "POST" ? "201" : "200", req);
                }
            }
            k = k == -1 ? 0 : k;
            return Response(servs[k], servs[k].get_root() + defaultErrorPages["404"], this->method, "404", req);
        }
        ~ARequest() {
        };
    std::string getQuery() {
        return (this->req.getQueryVar());
    }

    protected:
        Request req;
        std::vector<Server> servs;
        std::string method;
};

#endif