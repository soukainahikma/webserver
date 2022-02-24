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
		ARequest(Request &req, std::vector<Server> &servs);

		bool checkAllowedMethods (std::vector<std::string> &allowed_mehods, std::string method);

		virtual Response logic_function(Server server, Location location) = 0;

		std::string server_name_extractor();

		Response returnedResponse ();

		size_t get_num_of_servers_per_port();
		
		std::string getQuery();
		
		virtual ~ARequest();

	protected:
		Request req;
		std::vector<Server> servs;
		std::string method;
};

#endif