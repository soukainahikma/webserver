#include "Location.hpp"

Location::Location()
{
	this->Clear();
}

void						Location::Clear()
{
	_path.clear();
	_autoindex.clear();
	_return.clear();
	_fastcgi_pass.clear();
	_upload_enable.clear();
	_upload_store.clear();
	_index.clear();
	_request_method.clear();
}

void						Location::set_path(std::string path) {  _path = path; }
void						Location::set_autoindex(std::string autoindex) { _autoindex = autoindex; }
void						Location::set_index(std::vector<std::string> index) { _index = index; }
void						Location::set_return(std::string retur) { _return = retur; }
void						Location::set_request_method(std::vector<std::string> request_method) { _request_method	= request_method; }
void						Location::set_fastcgi_pass(std::string fastcgi_pass) { _fastcgi_pass = fastcgi_pass; }
void						Location::set_upload_enable(std::string upload_enable) { _upload_enable = upload_enable; }
void						Location::set_upload_store(std::string upload_store) { _upload_store = upload_store; }
//		get
std::string					Location::get_path() { return _path; }
std::string					Location::get_autoindex() { return _autoindex; }
std::vector<std::string>	Location::get_index() { return _index; }
std::string					Location::get_return() { return _return; }
std::vector<std::string>	Location::get_request_method() { return _request_method; }
std::string					Location::get_fastcgi_pass() { return _fastcgi_pass; }
std::string					Location::get_upload_enable() { return _upload_enable; }
std::string					Location::get_upload_store() { return _upload_store; }

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