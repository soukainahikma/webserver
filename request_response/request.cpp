#include "request.hpp"
	Request::Request(){
		is_error_header = false;
	};
	Request::Request(std::string buffer, int port)
	{
		is_error_header = false;
		this->port = port;
		fill_map(buffer);
	};
	Request &Request::operator=(Request &r)
	{
		if (this != &r)
		{
			map_head = r.map_head;
			port = r.port;
			body = r.body;
			myfiles = r.myfiles;
			query_var = r.query_var;
			boundary = r.boundary;
			contentType = r.contentType;
			is_error_header = r.is_error_header;
		}
		return (*this);
	}
	Request::vector_request Request::split_buffer(std::string myText, char c)
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
	void Request::fill_header(std::string buffer)
	{
		vector_request vec = split_buffer(buffer, '\r');
		vector_request head = split_buffer(vec[0].c_str(), ' ');
		if(head.size() != 3)
		{
			is_error_header = true;
			map_head["Method"] = "PUT";
			map_head["URL"] = "/";
			map_head["Protocol_version"] = "HTTP";
			return ;
		}
		this->query_var = "";
		if (head[1].find("?") != std::string::npos)
		{
			this->query_var = head[1].substr(head[1].find("?") + 1);
			head[1] = head[1].substr(0, head[1].find("?"));
		}
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
	void Request::body_info(body_struct &info,std::string str)// see if i have to pass it by reference
	{
		std::vector <std::string> tmp = split_buffer(str,';');
		std::vector<std::string> str_help;
		if (is_error_header)
			return ;
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
	}
	body_struct Request::initializer()
	{
		body_struct info;
		info.body ="";
		info.content_type = "";
		info.name = "";
		info.Content_Disposition = "";
		info.filename = "";
		return(info);
	}

	std::string Request::get_content_type () {
		return (this->contentType);
	}

	void Request::fill_body(void)
	{
		body_struct info;

		size_t found = 0;
		std::map<std::string,std::string>::iterator it= map_head.find("Content-Type");
		this->contentType = "";
		if(it !=map_head.end())
		{
			if((found = it->second.find("; boundary="))!= std::string::npos)
			{
				boundary = "--" + it->second.substr(found+11);
				this->contentType = it->second.substr(0,found);
				std::string tmp;
				size_t start;
				size_t end;
				size_t found;
				size_t cont_ty;
				start = body.find(boundary);
				while (start != std::string::npos)
				{
					if ((end = body.find(boundary, start + 1)) != std::string::npos)
					{
						tmp = body.substr(start + boundary.length(), end - start - boundary.length());
						if ((found = tmp.find("\r\n", 2)) != std::string::npos)
						{
							body_info(info, tmp.substr(1, found - 1));
							if ((cont_ty = tmp.find("Content-Type")) != std::string::npos)
							{
								end = tmp.find("\r\n", cont_ty);
								info.content_type = tmp.substr(cont_ty+14, end - cont_ty-14);
								found = end;
							}
							info.body = tmp.substr(found+4,tmp.length()- found -6);
						}
					}
					start = body.find(boundary, start + 1);
					myfiles.push_back(info);
				}
			}
		}
	}
	void Request::fill_map(std::string s)
	{
		std::string header;
		size_t found;
		if ((found = s.find("\r\n\r\n")) != std::string::npos)
		{
			header = s.substr(0, found);
			body = s.substr(found);
			fill_header(header);
			fill_body();
		}
		else
			fill_header(s);
	}

	std::string Request::getQueryVar() {
		return (this->query_var);
	}

	Request::map_request Request::getRequest() { return (map_head); }

	int Request::get_port() { return (port); }

	std::vector<body_struct> & Request::getBodyStructs() {
		return (myfiles);
	}

	std::string & Request::getBodyString() {
		return (body);
	}

	void Request::map_printer()
	{
		for (std::map<std::string, std::string>::iterator it = map_head.begin(); it != map_head.end(); ++it)
			std::cout<< BLUE <<"|" << it->first<< "|" <<RESET <<" => " << RED <<it->second << RESET<< '\n';
	}
	void Request::vec_printer()
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

	std::string &Request::get_query() {
		return (query); 
	}

	std::string Request::get_boundary () {
		return this->boundary;
	}
