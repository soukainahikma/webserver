#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "Server.hpp"

char	**init_env(std::string serv_name, std::string query, std::string status, std::string path, std::string page, std::string method)
{
	char **env = new char*[19];
	std::string tmp = "";
	std::map<std::string,std::string> env_m;

	env_m["REDIRECT_STATUS="] = status;
	env_m["GATEWAY_INTERFACE="] = "CGI/1.1";
	// env_m["SCRIPT_NAME="] = "/Users/cabouelw/Desktop/WebServ/index.php";
	env_m["SCRIPT_FILENAME="] = path + "/" + page;
	// std::cerr << env_m["SCRIPT_FILENAME="] << "\n";
	env_m["REQUEST_METHOD="] = method;
	// env_m["CONTENT_LENGTH="] = "16"; // size of request
	env_m["CONTENT_TYPE="] = "text/html";
	env_m["PATH_INFO="] = path;
	env_m["PATH_TRANSLATED="] = path;
	env_m["QUERY_STRING="] = query;
	// env_m["REMOTE_USER="] = ;
	env_m["SERVER_NAME="] = serv_name;
	env_m["SERVER_PORT="] = "8080"; // port
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
	return (env);
}

std::string runCgi(std::string path, std::string method, std::string page, std::string status)
{
	int pipefd[2];
	char **args;
	std::string body = "";
	int size_read = 24;
	char buffer[size_read];
	if (page.find(".py") != std::string::npos)
	{
		args = new char*[3];
		args[0] = (char*)"/usr/bin/python";
		args[1] = (char*)("../" + page).c_str();
		args[2] = NULL;
	}
	else
	{
		args = new char*[2];
		args[0] = (char*)"/Users/cabouelw/goinfre/.brew/bin/php-cgi";
		args[1] = NULL;
	}
	pipe(pipefd);
	char **env = init_env("tst.com", "fname=test&name=chb", status, path, page, method);
	int pid = fork();
	if (!pid)
	{
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		execve(args[0], args, (char **)env);
	}
	else
	{
		wait(&pid);
		size_t r;
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

int main ()
{
	std::cout << "------ index.py ------" << std::endl;
	std::string s = runCgi("/Users/cabouelw/Desktop/WebServ", "POST", "index.py", "200");
	std::cout << s << std::endl;
	std::cout << "------ index.php ------" << std::endl;
	std::string s1 = runCgi("/Users/cabouelw/Desktop/WebServ", "POST", "index.php", "200");
	std::cout << s1 << std::endl;
}