
#include <iostream>
#include <string>
#include <sstream>
#include "request_response/request.hpp"
#include <map>
int main(){
    Request req;
	Request::map_request map_;
	map_ = req.fill_map(strdup("GET /index.html HTTP/1.1\nHost: localhost:8080"));
	 for (std::map<std::string,std::string>::iterator it= map_.begin(); it!=map_.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}