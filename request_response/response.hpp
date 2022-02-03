/* in here i m gonna build the  response structre and class and its content*/
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<unistd.h>

// /* ************ Successful responses ******** */

// #define OK 200
// #define CREATED 201

// /* *********** Redirection messages ********* */

// #define MOVED_PERMANENTLY 301

// /* ********** Client error responses ******** */

// #define BAD_REQUEST 400
// #define UNAUTHORIZED 401
// #define FORBIDDEN 403
// #define NOT_FOUND 404

// int fileCheck(const char *fileName)
// {

// 	if (!access(fileName, F_OK))
// 	{
// 		if (!access(fileName, R_OK))
// 			return OK;
// 		return FORBIDDEN;
// 	}
// 	return NOT_FOUND;
// 	// else
// 	// {
// 	// 	printf("The File %s\t cannot be read\n", fileName);
// 	// }

// 	// if (!access(fileName, W_OK))
// 	// {
// 	// 	printf("The File %s\t it can be Edited\n", fileName);
// 	// }
// 	// else
// 	// {
// 	// 	printf("The File %s\t it cannot be Edited\n", fileName);
// 	// }

// 	// if (!access(fileName, X_OK))
// 	// {
// 	// 	printf("The File %s\t is an Executable\n", fileName);
// 	// }
// 	// else
// 	// {
// 	// 	printf("The File %s\t is not an Executable\n", fileName);
// 	// }
// }

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
			status_message = status_code >= 400 ? " KO\n" : " OK\n";
			filename = file;
			content_type = "Content-Type: text/html\r\n\n\n";
		}

		Response(std::string filename)
		{
			buffer = "";
			version = "HTTP/1.1 ";
			// status = "200 ";
			// status_message = " OK\n";
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
