#include "response.hpp"
Response::Response(Server server, Location location, Request &req)
{
	size_t i;
	int stats;
	std::string extension;

	generate_status_map();
	this->errorPages = server.get_error_page();
	root = server.get_root();
	server_root = server.get_root();
	path = location.get_path();
	url = req.getRequest()["URL"];
	method = this->request.getRequest()["Method"];
	location_string = "";
	this->request = req;

	this->filename = "";
	std::vector<std::string> indexes = location.get_index();
	is_autoindex = false;
	version = req.getRequest()["Protocol_version"] + " ";
	if ((location.get_return().size() == 0 && location.get_autoindex() != "on") && this->request.getRequest()["Method"] == "GET" && location.get_upload_enable() != "on")
		generate_autoindex(server);
	else
	{
		for (i = 0; i < indexes.size(); i++)
		{
			stats = fileCheck(root + path + "/" + indexes[i], this->request.getRequest()["Method"]);
			if (stats == OK || stats == CREATED)
				break;
		}

		if (location.get_return().size() != 0 || !(url.c_str()[url.size() - 1] == '/'))
		{
			status = location.get_return().size() != 0 ? location.get_return()[0] : "301";
			location_string = location.get_return().size() != 0 ? location.get_return()[1] : url + "/";
		}
		else
			status = ((stats == OK || stats == CREATED) ? (stats == OK ? "200" : "201") : ((stats == FORBIDDEN) ? "403" : "404"));
		if (indexes.size() > 0)
			this->filename = (status == "301") ? root + get_error_page("301") : (stats == OK || stats == CREATED) ? root + path + "/" + indexes[i]
																						: root + get_error_page(NumberToString(stats));
	}
}

Response::Response(Server server, std::string filename, std::string req_type, std::string status, Request &req)
{
	int stats;
	std::string extension;

	this->errorPages = server.get_error_page();
	is_autoindex = false;
	url = req.getRequest()["URL"];
	generate_status_map();
	this->request = req;
	this->root = "";
	this->server_root = server.get_root();
	this->path = "";
	method = req_type;
	version = req.getRequest()["Protocol_version"] + " ";
	stats = fileCheck(filename, this->request.getRequest()["Method"]);
	if (stats == IS_NOT_AUTO_INDEXED)
		generate_autoindex(server);
	else
	{
		this->status = (stats == OK || stats == CREATED) ? status : ((stats == FORBIDDEN) ? "403" : "404");
		this->filename = (stats == OK || stats == CREATED) ? filename : server.get_root() + get_error_page(NumberToString(stats));
	}
}

std::string Response::get_error_page(std::string status)
{
	if (this->errorPages.find(status) != this->errorPages.end())
		return this->errorPages.find(status)->second;
	return "/default_error_pages/" + status + ".html";
}

void Response::generate_autoindex(Server &server)
{
	struct dirent *pDirent;
	DIR *pDir;

	std::string whole_path = server.get_host() + ":" + NumberToString(server.get_listen());
	std::string file_content;
	pDir = opendir((server.get_root() + url).c_str());
	if (pDir == NULL)
	{
		this->status = "403";
		this->filename = server.get_root() + get_error_page(this->status);
		is_autoindex = false;
		return;
	}
	else if (this->request.getRequest()["Method"] != "GET")
	{
		this->status = "405";
		this->filename = server.get_root() + get_error_page(this->status);
		is_autoindex = false;
		return;
	}
	is_autoindex = true;
	auto_index_content_page = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title></head><body><h1 style=\"text-align: center;\">Auto Index Page</h1><ul>";
	while ((pDirent = readdir(pDir)) != NULL)
	{
		auto_index_content_page += "<li><a href=\"" + (this->request.getRequest()["URL"] == "/" ? "" : this->request.getRequest()["URL"]) + "/";
		auto_index_content_page.append(pDirent->d_name, pDirent->d_namlen);
		auto_index_content_page += "\">";
		auto_index_content_page.append(pDirent->d_name, pDirent->d_namlen);
		auto_index_content_page.append("</a></li>");
	}
	auto_index_content_page += "</ul></body></html>";
	this->status = "200";
	closedir(pDir);
}

void Response::generate_status_map()
{
	status_map["200"] = "OK\n";
	status_map["201"] = "Created\n";
	status_map["202"] = "Accepted\n";
	status_map["301"] = "Moved Permanently\n";
	status_map["400"] = "Bad Request\n";
	status_map["401"] = "Unauthorized\n";
	status_map["403"] = "Forbidden\n";
	status_map["404"] = "Not Found\n";
	status_map["405"] = "Method Not Allowed\n";
	status_map["413"] = "Payload Too Large\n";
	status_map["500"] = "Internal Server Error\n";
}

std::string Response::get_extension(std::string myText)
{
	std::vector<std::string> vec;
	std::istringstream iss(myText);
	std::string line;
	while (std::getline(iss, line, '.'))
	{
		if (line != "")
			vec.push_back(line);
	}
	return (vec[vec.size() - 1]);
}

std::string Response::get_file()
{
	std::ifstream file;
	std::string line;
	std::string buffer;
	std::ostringstream streambuff;

	file.open(filename, std::ios::binary);
	if (file.is_open())
	{
		streambuff << file.rdbuf();
		buffer = streambuff.str();
		file.close();
	}
	else
	{
		status = "500";
		file.open(this->errorPages["500"], std::ios::binary);
		streambuff << file.rdbuf();
		buffer = streambuff.str();
		file.close();
	}
	return (buffer);
}

std::string Response::get_content_type()
{
	std::string content_type;
	std::string extension = get_extension(this->filename);

	if (extension == "html" || extension == "css")
		return "text/" + extension;
	else if (extension == "js")
		return "text/javascript";
	return "*/*";
}

std::string Response::get_header()
{
	std::string extension;
	std::string file_to_send = "";
	t_cgi cgi;
	std::string cookieResponse = "";

	content_type = "Content-type: text/html; charset=UTF-8\r\n\r\n";
	if (is_autoindex)
	{
		return (version + status + " " + status_map[this->status] + content_type + auto_index_content_page);
	}
	if (status == "301")
		location_string = "Location: " + location_string + "\n";
	else if (this->filename != "")
	{
		get_content_type();
		extension = get_extension(this->filename);
		if (extension == "py" || extension == "php")
		{
			cgi.root = this->root;
			cgi.path = this->path;
			cgi.page = this->filename;
			file_to_send = runCgi(cgi, status, request);
			if (file_to_send != "")
				content_type = "";
			if (status == "500")
			{
				this->filename = this->server_root + get_error_page("500");
				content_type = "Content-type: text/html; charset=UTF-8\r\n\r\n";
				file_to_send = get_file();
			}
		}
		else
		{
			file_to_send = get_file();
			content_type = "Content-Type: " + get_content_type() + "\r\n\r\n";
		}
		extension = (extension == "py" || extension == "php") ? "html" : extension;
	}
	return (version + status + " " + status_map[this->status] + location_string + content_type + file_to_send);
}

//  curl http://www.example.com:5000 --resolve www.example.com:5000:127.0.0.1  -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "test=delete.txt"