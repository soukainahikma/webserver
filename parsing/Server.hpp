#ifndef SERVER_HPP
#define  SERVER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include "Location.hpp"

class Server
{
	int									_listen;
	std::string							_host;
	size_t								_client_max_body_size;
	std::string							_root;
	std::map<std::string, int>			_server_name;
	std::map<std::string, std::string>	_error_page;
	std::vector<Location>				_location;
	std::map<std::string, Location>		_map_location;
	int									_location_open;
	int									_server_open;
	std::string							_enable_delete;
	std::string							_enable_upload;
	public:
		Server();
		void	Clear();
		// set
		void	set_listen(std::string&);
		void	set_enable_delete(std::string&);
		void	set_enable_upload(std::string&);
		void	set_host(std::string&);
		void	set_root(std::string&);
		void	set_client_max_body_size(std::string&);
		void	set_server_name(std::vector<std::string>&);
		void	set_error_page(std::string&, std::string&);
		void	set_location(Location&);
		void	set_location_map(Location&);
		void	set_location_open(int);
		void	set_server_open(int);
		//	get
		int									&get_listen();
		std::string							&get_host();
		std::string							&get_enable_delete();
		std::string							&get_enable_upload();
		std::string							&get_root();
		size_t								&get_client_max_body_size();
		std::map<std::string, std::string>	&get_error_page();
		std::map<std::string, int>			&get_server_name();
		std::vector<Location>				&get_location();
		std::map<std::string, Location>		&get_location_map();
		int									&get_location_open();
		int									&get_server_open();

};
std::vector<std::string>	split(std::string &str, char c);
void						print_error(int i, std::string v);
std::string					get_key(std::string &line, int &idx);
std::string					get_value(std::string &line, int &idx);
void						print_all(std::vector<Server> &vec_serv, std::map<int,int> &m);

#endif