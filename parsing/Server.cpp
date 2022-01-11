#include "Server.hpp"
Server::Server()
{
	this->Clear();
}

void						Server::Clear()
{
	_listen.clear();
	_host.clear();
	_client_max_body_size.clear();
	_root.clear();
	_server_name.clear();
	_error_page.clear();
	_location.clear();
	_location_open = 0;
	_server_open = 0;
}
void Server::set_listen(std::string listen) { _listen = listen; }
void Server::set_host(std::string host) { _host = host; }
void Server::set_root(std::string root) { _root = root; }
void Server::set_client_max_body_size(std::string client_max_body_size) { _client_max_body_size = client_max_body_size; }
void Server::set_server_name(std::vector<std::string> server_name) { _server_name = server_name; }
void Server::set_error_page(std::string str_num, std::string str_path)
{
	_error_page[str_num] = str_path;
}
void Server::set_location(Location location) { _location.push_back(location); }
void Server::set_location_open(int location) { _location_open = location; }
void Server::set_server_open(int server) {_server_open = server; }

std::string Server::get_listen() { return _listen; }
std::string Server::get_host() { return _host; }
std::string Server::get_root() { return _root; }
std::string Server::get_client_max_body_size() { return _client_max_body_size; }
std::map<std::string, std::string> Server::get_error_page() { return _error_page; }
std::vector<std::string> Server::get_server_name() { return _server_name; }
std::vector<Location> Server::get_location() { return _location; }
int Server::get_location_open() { return _location_open; }
int Server::get_server_open() { return _server_open; }
