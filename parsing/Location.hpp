
#ifndef LOCATION_HPP
#define  LOCATION_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Location
{
	std::string					_path;
	std::string					_autoindex;
	std::vector<std::string>	_index;
	std::vector<std::string>	_return;
	std::vector<std::string>	_request_method;
	std::string					_fastcgi_pass;
	std::string					_upload_enable;
	std::string					_upload_store;
	std::map<int,std::string>	_return_map;
	bool						_equal;
	public:
		Location();
		void	Clear();
		// set
		void	set_path(std::string);
		void	set_equal(bool);
		void	set_autoindex(std::string);
		void	set_index(std::vector<std::string>&);
		void	set_return(std::vector<std::string>&);
		void	set_request_method(std::string);
		void	set_fastcgi_pass(std::string);
		void	set_upload_enable(std::string);
		void	set_upload_store(std::string);
		void	set_return_map(std::vector<std::string>&);
		//		get
		std::string					&get_path();
		std::string					&get_autoindex();
		std::vector<std::string>	&get_index();
		std::vector<std::string>	&get_return();
		std::vector<std::string>	&get_request_method();
		std::string					&get_fastcgi_pass();
		std::string					&get_upload_enable();
		std::string					&get_upload_store();
		std::map<int,std::string>	&get_return_map();
		bool						get_equal();
};
void						print_error(int i, std::string v);
std::vector<std::string>	split(std::string &str, char c);
std::string					trim(const std::string &s);
#endif
