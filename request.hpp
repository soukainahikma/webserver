/* in here i m gonna build the  request structre and class and its content*/
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <fstream>
class request
{
	private:
		std::string buffer;
		std::string version;
		std::string status;
		std::string status_message;
		std::string content_type;
		std::string content_length;//we will see if its optional
		std::string requested_page;
	public:
		request()
		{
			buffer = "";
			version = "HTTP/1.1 ";
			status = "200 ";
			status_message = " OK\n";
			content_type = "Content-Type: text/html\r\n\n\n";
		}
		std::string get_file(std::string filename)
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
		std::string get_header(std::string filename)
		{
			return(version + status + status_message + content_type + get_file(filename));
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
