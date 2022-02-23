/* in here i m gonna build the  response structre and class and its content*/
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

/* ************ Successful responses ******** */

#define OK 200
#define CREATED 201

/* *********** Redirection messages ********* */

#define MOVED_PERMANENTLY 301

/* ********** Client error responses ******** */

#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define PAYLOAD_TOO_LARGE 413
#define NOT_FOUND 404
#define NOT_AUTHORIZED 405

int			fileCheck(std::string fileName, std::string req_type);
std::string runCgi(t_cgi &cgi, std::string &status, Request &req);

class Response
{
	private:
		/* ****** Response Header paramters ***** */

		/* 
			{
				protocole_version: HTTP/1.1
				status_code: 200,
				status_message: OK
				content_type: "Content-Type: text/html\r\n\n\n"
				(OPTIONAL) content_type: "Content-Type: application/json\r\n\n\n"
			}
		 */
		std::string version;
		std::string status;
		std::string content_type;
		std::string content_length;
		std::string response_page;
		std::string root;
		std::string path;
		std::string filename;
		std::string location_string;
		std::string method;
		std::string auto_index_content_page;
        std::vector<std::string> indexes;
        std::map<std::string, std::string> errorPages;
		std::map<std::string, std::string> status_map;
		bool	is_autoindex;
		Request request;
    
	public:
		Response(Server server, Location location, std::string req_type, Request &req)
		{
			size_t i;
			int stats;
			std::string extension;
			std::string url;
			generate_status_map();
			root = server.get_root();
			path = location.get_path();
			url = req.getRequest()["URL"];
			this->request = req;
	
			this->filename = "";
			std::vector<std::string> indexes = location.get_index();
			is_autoindex = false;
			version = "HTTP/1.1 ";
			if (location.get_autoindex() != "on" && this->request.getRequest()["Method"] == "GET" && location.get_upload_enable() != "on")
				generate_autoindex(server);
			else
			{
				for (i = 0; i < indexes.size(); i++)
				{
					stats = fileCheck(root + path + "/" + indexes[i], this->request.getRequest()["Method"]);
					if (stats == OK || stats == CREATED)
						break;
				}
				// std::cout<< BLUE << "here" << RESET << std::endl;
				method = this->request.getRequest()["Method"];
				location_string = "";
				if (location.get_return().size() != 0 || !(url.c_str()[url.size() - 1] == '/'))
				{ 
					status = location.get_return().size() != 0 ?location.get_return()[0] : "301";
					location_string = location.get_return().size() != 0 ? location.get_return()[1] : url + "/";
				}
				else
					status = ((stats == OK || stats == CREATED) ? (stats == OK ? "200" : "201"):( (stats == FORBIDDEN) ? "403" : "404"));
				if (indexes.size() > 0)
					this->filename = (status == "301") ? "" : (stats == OK || stats == CREATED) ? root + path + "/" + indexes[i] : root + server.get_error_page()[std::to_string(stats)];

			}
		}

		Response(Server server, std::string filename, std::string req_type, std::string status, Request &req)
		{
			int stats;
			std::string extension;
			int is_deleted;

			is_autoindex = false;
			generate_status_map();
			this->request = req;
			this->root = "";
			this->path = "";
			method = req_type;
			version = "HTTP/1.1 ";
			// std::cout << GREEN << filename << RESET << std::endl;
			stats = fileCheck(filename, this->request.getRequest()["Method"]);
			this->status = (stats == OK || stats == CREATED) ? status :( (stats == FORBIDDEN) ? "403" : "404");
			this->filename = (stats == OK || stats == CREATED) ? filename : server.get_root() + server.get_error_page()[std::to_string(stats)];
		}

		void generate_autoindex(Server &server) {
	        struct dirent *pDirent;
	        DIR *pDir;

			is_autoindex = true;
			std::string whole_path = server.get_host() + ":" + std::to_string(server.get_listen());

			std::string file_content;
			// Ensure we can open directory.
	
			pDir = opendir ((root + path).c_str());
			if (pDir == NULL) {
				this->status = "403";
				this->filename =  server.get_error_page()["403"];
				return ;
			}

			// Process each entry.
			auto_index_content_page = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title></head><body><h1 style=\"text-align: center;\">Auto Index Page</h1><ul>";
			while ((pDirent = readdir(pDir)) != NULL) {
				// std::string str = pDirent->d_name;
				auto_index_content_page += "<li><a href=\"";
				auto_index_content_page.append(pDirent->d_name,pDirent->d_namlen);
				auto_index_content_page += "\">";
				auto_index_content_page.append(pDirent->d_name,pDirent->d_namlen);
				auto_index_content_page.append("</a></li>");
			}
			auto_index_content_page += "</ul></body></html>";
			this->status = "200";
			closedir (pDir);
		}

		void generate_status_map() {
			status_map["200"] ="OK\n";
			status_map["201"] ="Created\n";
			status_map["202"] ="Accepted\n";
			status_map["301"] ="Moved Permanently\n";
			status_map["400"] ="Bad Request\n";
			status_map["401"] ="Unauthorized\n";
			status_map["403"] ="Forbidden\n";
			status_map["404"] ="Not Found\n";
			status_map["405"] ="Method Not Allowed\n";
			status_map["413"] ="Payload Too Large\n";
			status_map["500"] ="Internal Server Error\n";
		}

		static std::string get_extension(std::string myText)
		{
			std::vector<std::string> vec;
			std::istringstream iss(myText);
			std::string line;
			while (std::getline(iss, line, '.'))
			{
				if(line!="")
					vec.push_back(line);
			}
			return (vec[vec.size() - 1]);
		}

		std::string get_file()
		{
			std::ifstream file;
			std::string line;
			std::string	buffer;
			std::ostringstream streambuff;

			file.open(filename, std::ios::binary);
			if (file.is_open())
			{
				streambuff << file.rdbuf();
				buffer = streambuff.str();
				file.close();
			}
			else
				std::cout << RED << "NOT OPEN \n"  << RESET;
			return (buffer);
		}

		std::string get_content_type () {
			std::string content_type;
			std::string extension = get_extension(this->filename);

			if (extension == "html" || extension == "css")
				return "text/" + extension;
			else if (extension == "js")
				return "text/javascript";
			return "*/*";
		}

		std::string get_header()
		{
			std::string		extension;
			std::string		file_to_send = "";
			t_cgi			cgi;
			std::string cookieResponse = "";

			
			content_type = "Content-Type: text/html\r\n\r\n";
			if (is_autoindex) {
				return (version + status + " " + status_map[this->status] + content_type + auto_index_content_page);
			}
			if (status == "301")
				location_string = "Location: " + location_string +"\n";
			else if (this->filename != "") {
				get_content_type();
				extension = get_extension(this->filename);
				if (extension == "py" || extension == "php")
				{
					cgi.root = this->root;
					cgi.path = this->path;
					cgi.page = this->filename;
					std::cout<< MAGENTA<< "here" << RESET<<std::endl; 
					std::cout << GREEN << request.getBodyString() << RESET << std::endl;
					std::cerr << RED << "**************1**************" << RESET << "\n";
					file_to_send = runCgi(cgi, status, request);
					std::cerr << RED << "*************2***************" << RESET << "\n";
					content_type = "";
				}
				else
				{
					file_to_send = get_file();
					content_type = "Content-Type: " +  get_content_type() + "\r\n\r\n";
				}
				extension = (extension == "py" || extension == "php") ? "html" : extension;
			}
			std::cout << GREEN << "HERE {}" << RESET << std::endl;
			return(version + status + " " + status_map[this->status] + location_string + content_type + file_to_send);
		}
};
#endif
