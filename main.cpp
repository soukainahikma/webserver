
#include <iostream>
#include <string>
#include <sstream>
#include "request_response/request.hpp"
#include <map>
int main(){
    Request req;
	Request::map_request map_;
	req.fill_map(strdup("POST / HTTP/1.1\nUser-Agent: PostmanRuntime/7.25.0\nAccept: */*\nPostman-Token: 1b0ebeaf-ce26-4614-b10c-c23431de3f7c\nHost: localhost:8080\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\nContent-Type: multipart/form-data; boundary=--------------------------859267994824015089444533\nContent-Length: 165"));
	map_ = req.getRequest();
	 for (std::map<std::string,std::string>::iterator it= map_.begin(); it!=map_.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}