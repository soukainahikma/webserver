/* in here i m gonna build the  response structre and class and its content*/
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <fstream>

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
		std::string buffer;
		std::string version;
		std::string status;
		std::string status_message;
		std::string content_type;
		std::string content_length;
		std::string response_page;
		std::string filename;

	
	public:
		Response()
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = "200 ";
			status_message = " OK\n";
			content_type = "Content-Type: text/html\r\n\n\n";
		}

		Response(int status_code, std::string file)
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = std::to_string(status_code) + " ";
			status_message = " OK\n";
			if (status_code >= 400)
			{
				status_message = " KO\n";
				filename = file;
			}
			content_type = "Content-Type: text/html\r\n\n\n";
		}

		Response(std::string filename)
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = "200 ";
			status_message = " OK\n";
			content_type = "Content-Type: text/html\r\n\n\n";
			this->filename = filename;
		}
		std::string get_file()
		{
			std::ifstream file;
			std::string line;
			file.open(filename);
			// do not forget to check the open permission
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
		void header_cleaner()
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = "200 ";
			status_message = " OK\n";
			content_type = "Content-Type: text/html\r\n\n\n";
		}

};
#endif
