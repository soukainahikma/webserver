/* in here i m gonna build the  response structre and class and its content*/
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <sstream>
class Request
{
	public:	
	typedef std::string string;
	typedef std::map<string,string> map_request;
	typedef std::vector<string> vector_request;


	private:
		map_request req;
	public:
		Request(){};
		vector_request split_buffer(const char *buffer,char c)
		{
			vector_request vec;
		    std::string myText(buffer);
    		std::istringstream iss(myText);
    		std::string line;
    		while (std::getline(iss, line, c))
    		{
    		    vec.push_back(line);
    		}
			return(vec);
		}
		map_request fill_map(char *buffer)
		{
			map_request map_;
			vector_request vec = split_buffer(buffer, '\n');
			vector_request head = split_buffer(vec[0].c_str(),' ');
			map_["Method"] = head[0];
			map_["URL"] = head[1];
			map_["Protocol_version"] = head[2];
			vector_request host = split_buffer(vec[1].c_str(),' ');
			map_[host[0]] = host[1];
			// this section is going to be needed later
			// vector_request tmp;
			// for (size_t i = 1; i < vec.size(); i++)
			// {
			// 	tmp = split_buffer(vec[i].c_str(),':');
			// 	map_[tmp[0]] = tmp[1];
			// }
			return(map_);
		}

		
};
#endif
