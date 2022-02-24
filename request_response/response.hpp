#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include "../parsing/Server.hpp"
#include <cstring>
#include <sstream>
#include "request.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#define OK 200
#define CREATED 201

#define MOVED_PERMANENTLY 301

#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define PAYLOAD_TOO_LARGE 413
#define NOT_FOUND 404
#define NOT_AUTHORIZED 405
#define IS_NOT_AUTO_INDEXED -100

int fileCheck(std::string fileName, std::string req_type);
std::string runCgi(t_cgi &cgi, std::string &status, Request &req);

class Response
{
private:
	std::string version;
	std::string status;
	std::string content_type;
	std::string content_length;
	std::string response_page;
	std::string root;
	std::string server_root;
	std::string path;
	std::string filename;
	std::string location_string;
	std::string method;
	std::string url;
	std::string auto_index_content_page;
	std::vector<std::string> indexes;
	std::map<std::string, std::string> errorPages;
	std::map<std::string, std::string> status_map;
	bool is_autoindex;
	Request request;

public:
	Response(Server server, Location location, Request &req);

	Response(Server server, std::string filename, std::string req_type, std::string status, Request &req);
	std::string get_error_page(std::string status);

	void generate_autoindex(Server &server);

	void generate_status_map();

	static std::string get_extension(std::string myText);

	std::string get_file();

	std::string get_content_type();

	std::string get_header();
};
#endif
