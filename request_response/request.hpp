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
		vector_request vec = split_buffer(buffer, '\n');
		vector_request head = split_buffer(vec[0].c_str(), ' ');
		map_head["Method"] = head[0];
		map_head["URL"] = head[1];
		map_head["Protocol_version"] = head[2];
		size_t found;
		for (size_t i = 1; i < vec.size(); i++)
		{
			if ((found = vec[i].find(":")) != std::string::npos)
			{
				map_head[vec[i].substr(0, found)] = vec[i].substr(found + 2);
			}
			else
				break;
		}
	}
	std::vector<body_struct> fill_body(std::string body)
	{
		std::string boundary;
		std::vector<body_struct> myfiles;
		body_struct info;
		int len;
		size_t found = 0;
		std::cout<<BLUE <<body << RESET<< std::endl;
		if((found = body.find("\n")) !=std::string::npos)
			boundary = body.substr(0, found);
		len = found + 1;
			std::cout<< boundary<< std::endl;
		while((found = body.find(boundary,len)) !=std::string::npos)
		{
			info.body = body.substr(len, found);
			len = len+ found;
			myfiles.push_back(info);
			std::cout<<MAGENTA<< info.body << RESET << std::endl;
			std::cout<< GREEN << "-----------------------"<<std::endl;
		}
		return(myfiles);

	}
	void fill_map(char *buffer)
	{
		std::string header;
		std::string body;
		std::string s(buffer);
		size_t found;
		if ((found = s.find("boundary=")) != std::string::npos)
		{
			header = s.substr(0, found);
			body = s.substr(found+ std::string("boundary=").length());
			fill_header(header);
			fill_body(body);
		}
		else
			fill_header(s);
		// printer();
	}

	map_request getRequest() { return (map_head); }

	int get_port() { return (port); }
	void printer()
	{
		for (std::map<std::string, std::string>::iterator it = map_head.begin(); it != map_head.end(); ++it)
			std::cout << it->first << " => " << it->second << '\n';
	}

private:
	map_request map_head;
	int port;
};
#endif
