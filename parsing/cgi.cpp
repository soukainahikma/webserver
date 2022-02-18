#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "../parsing/Server.hpp"
#include "../request_response/request.hpp"

char	**init_env(std::string status, std::string path, std::string page, Request &req)
{

	setenv("REDIRECT_STATUS", status.c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SCRIPT_NAME", page.c_str(), 1);
	setenv("SCRIPT_FILENAME", page.c_str(), 1);
	setenv("REQUEST_METHOD", req.getRequest()["Method"].c_str(), 1);
	setenv("CONTENT_TYPE", "text/html", 1);
	setenv("PATH_INFO", path.c_str(), 1);
	setenv("PATH_TRANSLATED", path.c_str(), 1);
	// setenv("QUERY_STRING", query.c_str(), 1);
	// (void)query;
	setenv("SERVER_NAME", split(req.getRequest()["Host"], ':')[0].c_str(), 1);
	setenv("SERVER_PORT", std::to_string(req.get_port()).c_str(), 1);
	setenv("SERVER_PROTOCOL", "\"HTTP/1.1\"", 1);
	/*
	char **env = new char*[19];
	std::string tmp = "";
	std::map<std::string,std::string> env_m;
	
	env_m["REDIRECT_STATUS="] = status;
	env_m["GATEWAY_INTERFACE="] = "CGI/1.1";
	// env_m["SCRIPT_NAME="] = "/Users/cabouelw/Desktop/WebServ/index.php";
	env_m["SCRIPT_FILENAME="] = page;
	// std::cerr << env_m["SCRIPT_FILENAME="] << "\n";
	env_m["REQUEST_METHOD="] = req.getRequest()["Method"];
	// env_m["CONTENT_LENGTH="] = "16"; // size of request
	env_m["CONTENT_TYPE="] = "text/html";
	env_m["PATH_INFO="] = path;
	env_m["PATH_TRANSLATED="] = path;
	env_m["QUERY_STRING="] = req.get_query();
	// env_m["REMOTE_USER="] = ;
	env_m["SERVER_NAME="] = serv_name ; // req.getRequest()["Host"].split(":")[0];
	env_m["SERVER_PORT="] = std::to_string(req.get_port()); // port
	env_m["SERVER_PROTOCOL="] = "\"HTTP/1.1\"";
	// env_m["SERVER_SOFTWARE="] = ;
	int j = 0;
	for (std::map<std::string,std::string>::const_iterator i = env_m.begin(); i != env_m.end(); i++)
	{
		tmp = i->first + i->second;
		env[j] = strdup(tmp.c_str());
		j++;
	}
	env[j] = NULL;
	*/
	extern char **environ;
	return (environ);
}

std::string runCgi(std::string root, std::string path, std::string page, std::string &status, Request &req)
{
	int pipefd[2];
	char **args;
	std::string body = "";
	int size_read = 1024;
	char buffer[size_read];
	if (size_read < 6)
		return NULL;
	char **env = init_env(status, path, page, req);
	if (page.find(".py") != std::string::npos)
	{
		args = new char*[3];
		args[0] = (char*)"/usr/bin/python";
		args[1] = (char*)(page).c_str();
		args[2] = NULL;
	}
	else
	{
		args = new char*[2];
		args[0] = (char*)"/Users/aboulbaz/goinfre/.brew/bin/php-cgi";
		args[1] = NULL;
	}
	pipe(pipefd);
	int pid = fork();
	if (!pid)
	{
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		execve(args[0], args, (char **)env);
		status = "403";
	}
	else
	{
		wait(&pid);
		int r;
		bzero(buffer, size_read);
		while ((r = read(pipefd[0], buffer, size_read - 1)))
		{
			body += buffer;
			if (r < size_read - 1)
				break;
			bzero(buffer, size_read);
		}
		close(pipefd[1]);
		close(pipefd[0]);
		delete [] args;
	}
	body.erase(0, body.find("\r\n\r\n"));
	return (body);
}

// int main ()
// {
// 	Request req;
// 	std::cout << "------ index.py ------" << std::endl;
// 	std::string s = runCgi("/Users/aboulbaz/Desktop/webserver/pages/python_cgi/", "GET", "index.py", "200", req);
// 	std::cout << s << std::endl;
// 	// std::cout << "------ index.php ------" << std::endl;
// 	// std::string s1 = runCgi("/Users/aboulbaz/Desktop/webserver", "POST", "index.php", "200");
// 	// std::cout << s1 << std::endl;
// }