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
		Request(){};
		Request(char *buffer){
			fill_map(buffer);
		};
		Request &operator= (Request &r) {
			if (this != &r) {
				map_ = r.map_;
			}
			return (*this);
		}
		vector_request split_buffer(const char *buffer,char c)
		{
			vector_request vec;
			std::string myText(buffer);
			std::istringstream iss(myText);
			std::string line;
			while (std::getline(iss, line, c))
				vec.push_back(line);
			return(vec);
		}

		void fill_map(char *buffer)
		{
			vector_request vec = split_buffer(buffer, '\n');
			vector_request head = split_buffer(vec[0].c_str(),' ');
			map_["Method"] = head[0];
			map_["URL"] = head[1];
			map_["Protocol_version"] = head[2];
			size_t found;
			for (size_t i = 1; i < vec.size(); i++)
			{
				if((found = vec[i].find(":"))!= std::string::npos)
				{
					map_[vec[i].substr(0,found)] = vec[i].substr(found+1);
				}
			}
		}

		map_request	getRequest() {
			return (map_);
		}

	private:
		map_request map_;
};
#endif
