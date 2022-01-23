#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <sstream>

#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"	   /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"	   /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"	   /* Cyan */
#define WHITE "\033[37m"   /* White */

struct body_struct
{
	std::string filename;
	std::string content_type;
	std::string body;
};

class Request
{
public:
	typedef std::string string;
	typedef std::map<string, string> map_request;
	typedef std::vector<string> vector_request;
	Request(){};
	Request(char *buffer, int port)
	{
		this->port = port;
		fill_map(buffer);
	};
	Request &operator=(Request &r)
	{
		if (this != &r)
		{
			map_head = r.map_head;
			port = r.port;
		}
		return (*this);
	}
	vector_request split_buffer(std::string myText, char c)
	{
		vector_request vec;
		std::istringstream iss(myText);
		std::string line;
		while (std::getline(iss, line, c))
			vec.push_back(line);
		return (vec);
	}
	void fill_header(std::string buffer)
	{
		vector_request vec = split_buffer(buffer, 13);
		vector_request head = split_buffer(vec[0].c_str(), ' ');
		map_head["Method"] = head[0];
		map_head["URL"] = head[1];
		map_head["Protocol_version"] = head[2];
		size_t found;
		for (size_t i = 1; i < vec.size(); i++)
		{
			if ((found = vec[i].find(":")) != std::string::npos)
				map_head[vec[i].substr(1, found-1)] = vec[i].substr(found + 2);
			else
				break;
		}
	}
	void fill_body(std::string body)
	{
		body_struct info;

		size_t found = 0;
		std::cout<<BLUE<< "|"<<body<< "|" << RESET << std::endl;
		std::map<std::string,std::string>::iterator it= map_head.find("Content-Type");
		if(it !=map_head.end())
		{
			if((found = it->second.find("; boundary="))!= std::string::npos)
			{
				boundary = "--" + it->second.substr(found+11);
				it->second = it->second.substr(0,found);
			}
		}
		int len =boundary.size()+6;
		while((found = body.find(boundary,len))!=std::string::npos)
		{
			info.body = body.substr(len,found - len-2);
			len = len + found-4 ;
			std::cout << "|"<<info.body <<"|" <<std::endl;
		}
	}
	void fill_map(char *buffer)
	{
		std::string header;
		std::string body;
		std::string s(buffer);
		size_t found;
		if ((found = s.find("\r\n\r\n")) != std::string::npos)
		{
			header = s.substr(0, found);
			body = s.substr(found);
			fill_header(header);
			fill_body(body);

			// std::cout << BLUE << s<< RESET <<std::endl;
			// std::cout<< YELLOW << header <<RESET << std::endl;
			// std::cout<< MAGENTA << body << RESET << std::endl;
			// map_printer();
		}
		else
			fill_header(s);
		// map_printer();
	}

	map_request getRequest() { return (map_head); }

	int get_port() { return (port); }
	void map_printer()
	{
		// std::map<std::string,std::string>::iterator it= map_head.find("Method");
		// 	std::cout<< it->first << it->second<<std::endl;
		for (std::map<std::string, std::string>::iterator it = map_head.begin(); it != map_head.end(); ++it)
			std::cout<< BLUE <<"|" << it->first<< "|" <<RESET <<" => " << RED <<it->second << RESET<< '\n';
	}
	void vec_printer()
	{
		for (size_t i = 0; i < myfiles.size(); i++)
		{
			std::cout << MAGENTA<< "this is file number " << WHITE <<i << std::endl;
			std::cout << YELLOW << "File name "<< RESET<< myfiles[i].filename<<std::endl;
			std::cout << YELLOW << "content type "<< RESET<< myfiles[i].content_type<<std::endl;
			std::cout << YELLOW << "body "<< RESET<< myfiles[i].body<<std::endl;
		}
		
	}
private:
	map_request map_head;
	std::vector<body_struct> myfiles;
	int port;
	std::string boundary;
};
#endif
