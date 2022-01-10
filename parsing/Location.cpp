#include "Location.hpp"

void Location::set_path(std::string path) { _path = path; }
void Location::set_autoindex(std::string autoindex) { _autoindex = autoindex; }
void Location::set_index(std::vector<std::string> index) { _index = index; }
void Location::set_return(std::string retur) { _return = retur; }
void Location::set_request_method(std::vector<std::string> request_method) { _request_method = request_method; }
void Location::set_fastcgi_pass(std::string fastcgi_pass) { _fastcgi_pass = fastcgi_pass; }
void Location::set_upload_enable(std::string upload_enable) { _upload_enable = upload_enable; }
void Location::set_upload_store(std::string upload_store) { _upload_store = upload_store; }
//		get
std::string Location::get_path() { return _path; }
std::string Location::get_autoindex() { return _autoindex; }
std::vector<std::string> Location::get_index() { return _index; }
std::string Location::get_return() { return _return; }
std::vector<std::string> Location::get_request_method() { return _request_method; }
std::string Location::get_fastcgi_pass() { return _fastcgi_pass; }
std::string Location::get_upload_enable() { return _upload_enable; }
std::string Location::get_upload_store() { return _upload_store; }