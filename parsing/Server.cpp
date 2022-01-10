#include "Server.hpp"
Server::Server()
{
    _location_open = false;
}
void Server::set_listen(std::string listen) { _listen = listen; }
void Server::set_host(std::string host) { _host = host; }
void Server::set_root(std::string root) { _root = root; }
void Server::set_client_max_body_size(std::string client_max_body_size) { _client_max_body_size = client_max_body_size; }
void Server::set_server_name(std::vector<std::string> server_name) { _server_name = server_name; }
void Server::set_error_page(std::vector<std::string> error_page) { _error_page = error_page; }
void Server::set_location(Location location) { _location.push_back(location); }
void Server::set_location_open(bool location) { _location_open = location; }

std::string Server::get_listen() { return _listen; }
std::string Server::get_host() { return _host; }
std::string Server::get_root() { return _root; }
std::string Server::get_client_max_body_size() { return _client_max_body_size; }
std::vector<std::string> Server::get_error_page() { return _error_page; }
std::vector<std::string> Server::get_server_name() { return _server_name; }
std::vector<Location> Server::get_location() { return _location; }
bool Server::get_location_open() { return _location_open; }
