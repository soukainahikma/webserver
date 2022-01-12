#include "Server.hpp"

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \t");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

Server::Server()
{
	this->Clear();
}

void						Server::Clear()
{
	_listen = 80;
	_host = "127.0.0.1";
	_client_max_body_size.clear();
	_root.clear();
	_server_name.clear();
	_error_page.clear();
	_location.clear();
	_location_open = 0;
	_server_open = 0;
}

void Server::set_listen(std::string listen)
{
	if (get_listen() == 80)
		return ;
	listen = trim(listen);
	if (listen.length() != 2 && listen.length() != 4)
		print_error(1, "listen");
	int idx = 0;
	while (listen[idx])
	{
		if (!std::isdigit(listen[idx]))
			print_error(1, "listen");
		idx++;
	}
	_listen = atoi(listen.c_str());
}

void Server::set_host(std::string host)
{
	if (get_host().compare("127.0.0.1"))
		return ;
	host = trim(host);
	if (host.find(" ") != std::string::npos)
		print_error(2, host);
	std::vector<std::string> splt = split(host, '.');
	int idx = 0;
	int j = 0;
	if (splt.size() != 4)
		print_error(1, host);
	while (idx < splt.size())
	{
		j = 0;
		while (splt[idx][j])
		{
			if (!std::isdigit(splt[idx][j]))
				print_error(3, host);
			j++;
		}
		if (std::stoi(splt[idx]) < 0 || std::stoi(splt[idx]) > 255)
			print_error(0, host);
		idx++;
	}
	_host = host;
}

void Server::set_root(std::string root)
{
	root = trim(root);
	if (root.find(" ") != std::string::npos)
		print_error(1, root);
	_root = root;
}

void Server::set_client_max_body_size(std::string client_max_body_size)
{
	client_max_body_size = trim(client_max_body_size);
	int j = 0;
	while (client_max_body_size[j])
	{
		if (!std::isdigit(client_max_body_size[j]))
			if (client_max_body_size[j] != 'm' || client_max_body_size[j + 1])
				print_error(3, client_max_body_size);
		j++;
	}
	_client_max_body_size = client_max_body_size;
}

void Server::set_server_name(std::vector<std::string> server_name) { _server_name = server_name; }

void Server::set_error_page(std::string str_num, std::string str_path)
{
	int j = 0;
	while (str_num[j])
	{
		if (!std::isdigit(str_num[j]))
			print_error(3, str_num);
		j++;
	}
	if (_error_page[str_num].empty())
		_error_page[str_num] = str_path;
}
void Server::set_location(Location location) { _location.push_back(location); }
void Server::set_location_open(int location) { _location_open = location; }
void Server::set_server_open(int server) {_server_open = server; }

int			Server::get_listen() { return _listen; }
std::string Server::get_host() { return _host; }
std::string Server::get_root() { return _root; }
std::string Server::get_client_max_body_size() { return _client_max_body_size; }
std::map<std::string, std::string> Server::get_error_page() { return _error_page; }
std::vector<std::string> Server::get_server_name() { return _server_name; }
std::vector<Location> Server::get_location() { return _location; }
int Server::get_location_open() { return _location_open; }
int Server::get_server_open() { return _server_open; }
