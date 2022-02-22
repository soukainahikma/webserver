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
        std::vector<std::string> indexes;
        std::map<std::string, std::string> errorPages;
		std::map<std::string, std::string> status_map;
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
			std::cout<< MAGENTA << "here" << RESET << std::endl;
			for (i = 0; i < indexes.size(); i++)
			{
				stats = fileCheck(root + path + "/" + indexes[i], this->request.getRequest()["Method"]);
				if (stats == OK || stats == CREATED)
					break;
			}
			std::cout<< BLUE << "here" << RESET << std::endl;
			method = this->request.getRequest()["Method"];
			version = "HTTP/1.1 ";
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

		Response(Server server, std::string filename, std::string req_type, std::string status, Request &req)
		{
			int stats;
			std::string extension;
			int is_deleted;

			std::cout << indexes.size() << std::endl;
			generate_status_map();
			this->request = req;
			this->root = "";
			this->path = "";
			method = req_type;
			version = "HTTP/1.1 ";
			stats = fileCheck(filename, this->request.getRequest()["Method"]);
			this->status = (stats == OK || stats == CREATED) ? status :( (stats == FORBIDDEN) ? "403" : "404");
			this->filename = (stats == OK || stats == CREATED) ? filename : server.get_root() + server.get_error_page()[std::to_string(stats)];
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
			// std::ifstream file;
			// std::string line;
			// std::string	buffer;

			// file.open(filename, std::ios::binary);
			// while (getline(file, line))
			// 	buffer = buffer + line;
			// return (buffer);

			std::ifstream    indexFile(filename, std::ios::binary);
			std::ostringstream buffer;

			buffer << indexFile.rdbuf();
			std::ofstream out ("test_art.jpeg");
			out << buffer.str();
			return(buffer.str());
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

			
			content_type = "Content-Type: text/html";
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
					file_to_send = runCgi(cgi, status, request);
					content_type = "";
				}
				else
				{
					file_to_send = get_file();
					// content_type = "Content-Type: text/" + extension + "\r\n\n\n";
					content_type = "Content-Type: " +  get_content_type() + "\r\n\n\n";
				}
				extension = (extension == "py" || extension == "php") ? "html" : extension;
			}
			std::cout << RED << request.get_boundary() << RESET << std::endl;
			// std::cout << RED << "+++++++++++++++ {  } ++++++++++++++"<< RESET << std::endl << version + status + " " + status_map[this->status] + location_string + content_type + file_to_send << std::endl;
			return(version + status + " " + status_map[this->status] + location_string + content_type + file_to_send);
		}
};
#endif
