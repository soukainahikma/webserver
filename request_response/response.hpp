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
std::string runCgi(std::string root, std::string path, std::string page, std::string &status, Request &req);

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
			std::vector<std::string> indexes = location.get_index();
			for (i = 0; i < indexes.size(); i++)
			{
				stats = fileCheck(root + path + "/" + indexes[i], this->request.getRequest()["Method"]);
				if (stats == OK || stats == CREATED)
					break;
			}
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
			filename = (status == "301") ? "" : (stats == OK || stats == CREATED) ? root + path + "/" + indexes[i] : root + server.get_error_page()[std::to_string(stats)];
		}

		Response(Server server, std::string filename, std::string req_type, std::string status, Request &req)
		{
			int stats;
			std::string extension;
			int is_deleted;

			generate_status_map();
			this->request = req;
			this->root = "";
			this->path = "";
			method = req_type;
			version = "HTTP/1.1 ";
			stats = fileCheck(filename, this->request.getRequest()["Method"]);
			this->status = (stats == OK || stats == CREATED) ? status :( (stats == FORBIDDEN) ? "403" : "404");
			this->filename = (stats == OK || stats == CREATED) ? filename : server.get_root() + server.get_error_page()[std::to_string(stats)];
			if (true && this->request.getRequest()["Method"] == "DELETE" && (stats == OK))
			{
				is_deleted = remove(this->filename.c_str());
				if (is_deleted != 0) {
					this->status = "500";
				}
				this->filename = "";
			}
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
			std::string buffer;

			file.open(filename);
			while (getline(file, line))
			{
				buffer = buffer + line;
			}
			file.close();
			return(buffer);
		}

		// std::string extension_extractor(std::string extension) {
			
		// }

		std::string get_header()
		{
			std::string		extension;
			std::string		file_to_send = "";
			
			content_type = "";
			if (status == "301")
				location_string = "Location: " + location_string;
			else if (this->filename != "") {
				extension = get_extension(this->filename);
				if (extension == "py" || extension == "php")
					file_to_send = runCgi(this->root, this->path, this->filename, status, request);
				else
					file_to_send = get_file();
				extension = (extension == "py" || extension == "php") ? "html" : extension;
				content_type = "Content-Type: text/" + extension + "\r\n\n\n";
			}
			return(version + status + " " + status_map[this->status] + location_string + content_type + file_to_send);
		}

};
#endif
