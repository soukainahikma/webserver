#include "./request_response/request.hpp"
#include <iostream>
#include<map>
#include <cstring>
#include <unistd.h>
std::vector<std::string> fill_vec(std::string str)
{
	std::vector<std::string> result;
	std::string body;
	std::string info;
	std::string content;
	std::string tmp;
	size_t start;
	size_t end;
	size_t found;
	
	start = str.find("----WebKitFormBoundary7MA4YWxkTrZu0gW");
	while(start != std::string::npos)
	{
		if((end = str.find("----WebKitFormBoundary7MA4YWxkTrZu0gW",start+1))!= std::string::npos)
		{
			tmp = str.substr(start+38,end - start -38);
			if((found = tmp.find("\r\n",2))!= std::string::npos)
			{
				info = tmp.substr(1,found-1);
				if((found = tmp.find("Content-Type"))!= std::string::npos)
				{
					end = tmp.find("\r\n",found);
					content = tmp.substr(found, end -found );
					std::cout<<"|"<< content<<"|" << std::endl;
					found = end;
				}
				body = tmp.substr(found);
				std::cout<<"|"<< info<<"|" << std::endl;
				std::cout<<"|"<< body<<"|" << std::endl;
			}
		}
		start = str.find("----WebKitFormBoundary7MA4YWxkTrZu0gW",start+1);
	}
	return(result);
}
int main()
{
	std::string str = "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data;\r\nContent-Type: text/html\r\n name=\"hello\" =test\r\n----WebKitFormBoundary7MA4YWxkTrZu0gW--";
	fill_vec(str);
}



/* 
if(tmp[i+1].find("Content-Type")!= std::string::npos)
{
	info.content_type = tmp[++i].substr(12);
}
				} */