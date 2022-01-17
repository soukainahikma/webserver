#include "Location.hpp"

Location::Location()
{
	this->Clear();
}

void	Location::Clear()
{
	_path.clear();
	_autoindex.clear();
	_return.clear();
	_fastcgi_pass.clear();
	_upload_enable.clear();
	_upload_store.clear();
	_index.clear();
	_request_method.clear();
	_equal = false;
}

void	Location::set_path(std::string path)
{
	path = trim(path);
	if (path[0] == '=')
	{
		int i = 1;
		if (path[i] == '=')
			print_error(1, path);
		set_equal(true);
		while (path[i] == ' ' || path[i] == '\t')
			i++;
		path.erase(0, i);
	}
	if (path.find(" ") != std::string::npos)
		print_error(1, path);
	_path = path;
}

void Location::set_equal(bool equal) { _equal = equal; }

void	Location::set_autoindex(std::string autoindex)
{
	if (!get_autoindex().empty())
		print_error(8, "autoindex");
	autoindex = trim(autoindex);
	if (autoindex.compare("on") && autoindex.compare("off"))
		print_error(12, autoindex);
	_autoindex = autoindex;
}
void	Location::set_index(std::vector<std::string> &index)
{
	if (get_index().size())
		print_error(8, "index");
	_index = index;
}

void	Location::set_return(std::vector<std::string> &retur)
{
	int i = 0;
	if (retur.size() != 2)
		print_error(1, "return");
	while (retur[0][i])
	{
		if (!std::isdigit(retur[0][i]))
			print_error(1, retur[0]);
		i++;
	}
	_return = retur;
}

int check_merhods(std::string request_method)
{
	int idx = 1;
	request_method = trim(request_method);
	if (request_method[0] != '[')
		return (1);
	while (request_method[idx] && request_method[idx] != ']' && request_method[idx] != '[')
		idx++;
	if (request_method[idx + 1] || (request_method[idx] != ']' && request_method[idx] != '['))
		return (1);
	
	return (0);
}

void	Location::set_request_method(std::string request_method)
{
	if (get_request_method().size())
		print_error(8, request_method);
	int g, p, d;
	if (check_merhods(request_method))
		print_error(13, request_method);
	request_method.erase(request_method.find('['), 1);
	request_method.erase(request_method.find(']'), 1);
	std::vector<std::string> splt = split(request_method, ',');
	int idx = 0;
	g = 0;
	p = g;
	d = g;
	while (idx < splt.size())
	{
		splt[idx] = trim(splt[idx]);
		if (!splt[idx].compare("GET"))
			g++;
		else if (!splt[idx].compare("POST"))
			p++;
		else if (!splt[idx].compare("DELETE"))
			d++;
		else{
			std::cout << "|" << g << "|"  << p << "|" <<  d << "|\n";
			print_error(1, request_method);
		}
		if (idx > 3 || g > 1 || p > 1 || d > 1)
			print_error(8, request_method);
		if (splt[idx].find(' ') != std::string::npos)
			print_error(1, request_method);
		idx++;
	}
	_request_method	= splt;
}

void	Location::set_return_map(std::vector<std::string> &ret)
{
	int i = 0;
	if (ret.size() != 2)
		print_error(1, "return " + ret[0] + " " + ret[1]);
	while (ret[0][i])
	{
		if (!std::isdigit(ret[0][i]))
			print_error(1, "return " + ret[0] + " " + ret[1]);
		i++;
	}
	if (_return_map[std::stoi(ret[0])].empty())
		_return_map[std::stoi(ret[0])] = ret[1];
}

void						Location::set_fastcgi_pass(std::string fastcgi_pass) { _fastcgi_pass = fastcgi_pass; }
void						Location::set_upload_enable(std::string upload_enable) { _upload_enable = upload_enable; }
void						Location::set_upload_store(std::string upload_store) { _upload_store = upload_store; }
//		get
std::string					&Location::get_path() { return _path; }
std::string					&Location::get_autoindex() { return _autoindex; }
std::vector<std::string>	&Location::get_index() { return _index; }
std::vector<std::string>	&Location::get_return() { return _return; }
std::vector<std::string>	&Location::get_request_method() { return _request_method; }
std::string					&Location::get_fastcgi_pass() { return _fastcgi_pass; }
std::string					&Location::get_upload_enable() { return _upload_enable; }
std::string					&Location::get_upload_store() { return _upload_store; }
std::map<int, std::string>	&Location::get_return_map() { return _return_map; }
bool						Location::get_equal() { return _equal; }

/*
Location::set_path(std::string path) { std::cout << "set path =" << path << "\n"; _path = path; }
void						Location::set_autoindex(std::string autoindex) {std::cout << "set autoindex=" << autoindex << "\n"; _autoindex = autoindex; }
void						Location::set_index(std::vector<std::string> index) {std::cout << "set index=" << index[0] << "\n"; _index = index; }
void						Location::set_return(std::string retur) {std::cout << "set return=" << retur << "\n"; _return = retur; }
void						Location::set_request_method(std::vector<std::string> request_method) {std::cout << "set request_method=" << request_method[0] << "\n"; _request_method	= request_method; }
void						Location::set_fastcgi_pass(std::string fastcgi_pass) {std::cout << "set fastcgi_pass=" << fastcgi_pass << "\n"; _fastcgi_pass = fastcgi_pass; }
void						Location::set_upload_enable(std::string upload_enable) {std::cout << "set upload_enable=" << upload_enable << "\n"; _upload_enable = upload_enable; }
void						Location::set_upload_store(std::string upload_store) {std::cout << "set upload_store=" << upload_store << "\n"; _upload_store = upload_store; }
//		get
*/