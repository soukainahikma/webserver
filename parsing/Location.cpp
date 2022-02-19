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
	_enable_delete.clear();
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
	size_t idx = 0;
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
		else
			print_error(1, request_method);
		if (idx > 3 || g > 1 || p > 1 || d > 1)
			print_error(8, request_method);
		if (splt[idx].find(' ') != std::string::npos)
			print_error(1, request_method);
		idx++;
	}
	_request_method	= splt;
}

void	Location::set_fastcgi_pass(std::string fastcgi_pass)
{
	if (fastcgi_pass.compare("on") && fastcgi_pass.compare("off"))
		print_error(12, "fastcgi_pass	" + fastcgi_pass);
	if (!_fastcgi_pass.empty())
		print_error(8, "fastcgi_pass	" + fastcgi_pass);
	_fastcgi_pass = fastcgi_pass;
}

void	Location::set_upload_enable(std::string upload_enable)
{
	if (upload_enable.compare("on") && upload_enable.compare("off"))
		print_error(12, "upload_enable	" + upload_enable);
	if (!_upload_enable.empty())
		print_error(8, "upload_enable	" + upload_enable);
	_upload_enable = upload_enable;
}

void	Location::set_enable_delete(std::string enable_delete)
{
	if (enable_delete.compare("on") && enable_delete.compare("off"))
		print_error(12, "enable_delete	" + enable_delete);
	if (!_enable_delete.empty())
		print_error(8, "enable_delete	" + enable_delete);
	_enable_delete = enable_delete;
}

void	Location::set_upload_store(std::string upload_store)
{
	if (!_upload_store.empty())
		print_error(8, "upload_store	" + upload_store);
	_upload_store = upload_store;
}
//		get
std::string					&Location::get_path() { return _path; }
std::string					&Location::get_autoindex() { return _autoindex; }
std::vector<std::string>	&Location::get_index() { return _index; }
std::vector<std::string>	&Location::get_return() { return _return; }
std::vector<std::string>	&Location::get_request_method() { return _request_method; }
std::string					&Location::get_fastcgi_pass() { return _fastcgi_pass; }
std::string					&Location::get_upload_enable() { return _upload_enable; }
std::string					&Location::get_enable_delete() { return _enable_delete; }
std::string					&Location::get_upload_store() { return _upload_store; }
bool						Location::get_equal() { return _equal; }