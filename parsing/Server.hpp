#ifndef SERVER_HPP
#define  SERVER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Location.hpp"

class Server
{
	std::string					_listen;
	std::string					_host;
	std::string					_client_max_body_size;
	std::string					_root;
	std::vector<std::string>	_server_name;
	std::vector<std::string>	_error_page;
	std::vector<Location>		_location;
	bool						_location_open;
	public:
		// set
		Server();
		void	set_listen(std::string);
		void	set_host(std::string);
		void	set_root(std::string);
		void	set_client_max_body_size(std::string);
		void	set_server_name(std::vector<std::string>);
		void	set_error_page(std::vector<std::string>);
		void	set_location(Location);
		void	set_location_open(bool);
		//	get
		std::string					get_listen();
		std::string					get_host();
		std::string					get_root();
		std::string					get_client_max_body_size();
		std::vector<std::string>	get_error_page();
		std::vector<std::string>	get_server_name();
		std::vector<Location>		get_location();
		bool						get_location_open();

};

std::vector<Server> parsing(std::string file);

#endif