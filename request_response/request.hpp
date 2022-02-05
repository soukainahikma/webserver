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
	std::string Content_Disposition;
	std::string name;
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
			body = r.body;
			myfiles = r.myfiles;
		}
		return (*this);
	}
	vector_request split_buffer(std::string myText, char c)
	{
		vector_request vec;
		std::istringstream iss(myText);
		std::string line;
		while (std::getline(iss, line, c))
		{
			if(*line.begin() == '\n')
				line = line.substr(1);
			if(line!="")
				vec.push_back(line);
		}
		return (vec);
	}
	void fill_header(std::string buffer)
	{
		vector_request vec = split_buffer(buffer, '\r');
		vector_request head = split_buffer(vec[0].c_str(), ' ');
		map_head["Method"] = head[0];
		map_head["URL"] = head[1];
		map_head["Protocol_version"] = head[2];
		size_t found;
		for (size_t i = 1; i < vec.size(); i++)
		{
			if ((found = vec[i].find(":")) != std::string::npos)
				map_head[vec[i].substr(0, found)] = vec[i].substr(found + 2);
			else
				break;
		}
	}
	void body_info(body_struct &info,std::string str)// see if i have to pass it by reference
	{
		std::vector <std::string> tmp = split_buffer(str,';');
		std::vector<std::string> str_help;
		for (size_t i = 0; i < tmp.size(); i++)
		{
			if(i == 0)
				str_help = split_buffer(tmp[i],':');
			else
				str_help = split_buffer(tmp[i],'=');
			if(str_help[0] =="Content-Disposition")
				info.Content_Disposition = str_help[1].substr(1);
			if(str_help[0]==" name")
				info.name = str_help[1].substr(1,str_help[1].length() - 2);
			if(str_help[0]==" filename")
				info.filename = str_help[1].substr(1,str_help[1].length() - 2);
		}
		
		//Content-Disposition: form-data; name="test"; filename="index.html"
	}
	body_struct initializer()
	{
		body_struct info;
		info.body ="";
		info.content_type = "";
		info.name = "";
		info.Content_Disposition = "";
		info.filename = "";
		return(info);
	}
	void fill_body(/* std::string body */)
	{
		body_struct info;

		size_t found = 0;
		std::map<std::string,std::string>::iterator it= map_head.find("Content-Type");
		if(it !=map_head.end())
		{
			if((found = it->second.find("; boundary="))!= std::string::npos)
			{
				boundary = "--" + it->second.substr(found+11);
				it->second = it->second.substr(0,found);
				std::vector<std::string> tmp = split_buffer(body,13);
				size_t i = 0;
				while(i < tmp.size())
				{
					info = initializer();
					if(tmp[i] == boundary)
					{
						i++;
						body_info(info,tmp[i]);
						if(tmp[i+1].find("Content-Type")!= std::string::npos)
						{
							info.content_type = tmp[++i].substr(12);
						}
						while(tmp[i+1] != boundary && tmp[i+1] != boundary+"--" && i+1<tmp.size())
						{
							info.body += tmp[i+1] +"\n";
							i++;
						}
						myfiles.push_back(info);
					}
					i++;
				}
			}
		}
	}
	void fill_map(char *buffer)
	{
		std::string header;
		// std::string body;
		std::string s(buffer);
		size_t found;
		if ((found = s.find("\r\n\r\n")) != std::string::npos)
		{
			header = s.substr(0, found);
			body = s.substr(found);
			fill_header(header);
			// std::cout<< MAGENTA << body << RESET << std::endl;
			fill_body();
		}
		else
			fill_header(s);
		// vec_printer();
		// map_printer();
	}

	map_request getRequest() { return (map_head); }

	int get_port() { return (port); }

	std::vector<body_struct> & getBodyStructs() {
		return (myfiles);
	}

	std::string & getBodyString() {
		return (body);
	}

	void map_printer()
	{
		for (std::map<std::string, std::string>::iterator it = map_head.begin(); it != map_head.end(); ++it)
			std::cout<< BLUE <<"|" << it->first<< "|" <<RESET <<" => " << RED <<it->second << RESET<< '\n';
	}
	void vec_printer()
	{
		std::cout << RED << " ************* { HERE IS BODY PRINTER BEGINS ==> " << myfiles.size() << " } *************** " << RESET << std::endl;
		for (size_t i = 0; i < myfiles.size(); i++)
		{
			std::cout << MAGENTA<< "this is file number " << WHITE <<i << std::endl;
			std::cout << YELLOW << "File name "<< RESET<< myfiles[i].filename<<std::endl;
			std::cout << YELLOW << "content type "<< RESET<< myfiles[i].content_type<<std::endl;
			std::cout << YELLOW << "body "<< RESET<< myfiles[i].body<<std::endl; 
			std::cout << YELLOW << "name "<< RESET<< myfiles[i].name<<std::endl; 
			std::cout << YELLOW << "Content_Disposition "<< RESET<< myfiles[i].Content_Disposition<<std::endl; 
		}
		if (myfiles.size() == 0)
			std::cout << body << std::endl;
		std::cout << RED << " ************* { HERE IS BODY PRINTER ENDS } *************** " << RESET << std::endl;
	}


private:
	map_request map_head;
	std::string body;
	int port;
	std::string boundary;
	std::vector<body_struct> myfiles; // if (|Content-Type| => multipart/form-data) myfiles| myfiles.size() != 0 
};
#endif
