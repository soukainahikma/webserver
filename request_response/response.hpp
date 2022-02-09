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

int fileCheck(std::string fileName, std::string req_type);
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
		std::string status_message;
		std::string content_type;
		std::string content_length;
		std::string response_page;
		std::string root;
		std::string path;
		std::string filename;
        std::vector<std::string> indexes;
        std::map<std::string, std::string> errorPages;
    
	public:
		Response(Server server, Location location, std::string req_type)
		{
			size_t i;
			int stats;
			std::string extension;

			root = server.get_root();
			path = location.get_path();
			std::vector<std::string> indexes = location.get_index();
			for (i = 0; i < indexes.size(); i++)
			{
				stats = fileCheck(root + path + "/" + indexes[i], req_type);
				if (stats == OK || stats == CREATED)
					break;
			}
			version = "HTTP/1.1 ";
			status = (stats == OK || stats == CREATED) ? "201 " : "404 ";
			status_message = (stats == OK || stats == CREATED) ? " OK\n" : "KO\n";
			filename = (stats == OK || stats == CREATED) ? root + path + "/" + indexes[i] : root + errorPages[std::to_string(stats)];
			content_type = "Content-Type: text/" + get_extension(this->filename) + "\r\n\n\n";
		}

		Response(Server server, std::string filename, std::string req_type, std::string status)
		{
			std::ifstream file;
			int stats;
			std::string extension;
	
			this->root = "";
			this->path = "";
			version = "HTTP/1.1 ";
			stats = fileCheck(filename, req_type);
			this->status = (stats == OK || stats == CREATED) ? status + " " : "404 ";
			status_message = this->status == "404 " ? "KO\n" : "OK\n";
			this->filename = (stats == OK || stats == CREATED) ? filename : server.get_root() + server.get_error_page()[std::to_string(stats)];
			content_type = "Content-Type: text/" + get_extension(this->filename) + "\r\n\n\n";
			file.close();
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
			// std::cout << "In get_file => "<< filename << std::endl;
			while (getline(file, line))
			{
				buffer = buffer + line;
			}
			file.close();
			return(buffer);
		}

		std::string get_header()
		{
			return(version + status + status_message + content_type + get_file());
		}

		/* void header_cleaner()
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = "200 ";
			status_message = " OK\n";
			content_type = "Content-Type: text/html\r\n\n\n";
		} */

};
#endif
