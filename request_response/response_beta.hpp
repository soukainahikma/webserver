/* in here i m gonna build the  response structre and class and its content*/
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

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
		
		Response(std::string root, std::string path, std::vector<std::string> &indexes, std::map<std::string, std::string> errorPages)
		{
			std::ifstream file;
			size_t i;

			this->root = root;
			this->path = path;
			for (i = 0; i < indexes.size(); i++)
			{
				file.open(root + path + "/" + indexes[i]);
				if (!file.fail())
					break;
			}
			version = "HTTP/1.1 ";
			status = !file.fail() ? "200 " : "404 ";
			status_message = !file.fail() ? " OK\n" : "KO\n";
			content_type = "Content-Type: text/html\r\n\n\n";
			filename = !file.fail() ? root + path + "/" + indexes[i] : errorPages["404"];
			std::cout << "While construction {1} => " << filename << std::endl;
			file.close();
		}

		Response(std::string root, std::string filename, std::map<std::string, std::string> errorPages)
		{
			std::ifstream file;
	
			this->root = "";
			this->path = "";
			version = "HTTP/1.1 ";
            file.open(filename);
			status = !file.fail() ? "200 " : "404 ";
			status_message = !file.fail() ? " OK\n" : "KO\n";
			this->filename = !file.fail() ? filename : root + errorPages["404"];
			std::cout << "While construction {2} => " << this->filename << std::endl;
			content_type = "Content-Type: text/html\r\n\n\n";
			file.close();
		}

		std::string get_file()
		{
			std::ifstream file;
			std::string line;
			std::string buffer;

			file.open(filename);
			std::cout << "In get_file => "<< filename << std::endl;
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
