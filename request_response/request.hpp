/* in here i m gonna build the  response structre and class and its content*/
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <map>

class Request
{

	public:
		typedef typename std::map<std::string, std::string> req_map_parsing;
		typedef typename std::string string;
		
	
	private:
		int counter;
		string raw_request;
		req_map_parsing parse;
		/* ****** Request Header paramters ***** */
		/* 
		GET / HTTP/1.1
		Host: localhost:8080
		
		{
			method: GET,
			path: /
			protocole_version: HTTP/1.1
			Host: localhost

		}
		 */

	public:
		Request() {
			counter = 0;
			raw_request = "";
		};

		Request(char *s) {
			counter = 0;
			raw_request = string(s);
		};
		
		Request(Request &req) {
			counter = 0;
			if (this != &req)
			{
				raw_request = req.raw_request;
				parse = req.parse;
			}
		};

		string getRawRequest() {
			return (raw_request);
		}

		req_map_parsing getParsedRequest() {
			return (parse);
		}

		void setRawRequest(string str) {
			raw_request = str;
		}

		void parse() {
			std::cout << raw_request << std::endl;
		}
};
#endif
