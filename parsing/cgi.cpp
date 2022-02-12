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
	env_m["REQUEST_METHOD="] = method;
	// env_m["CONTENT_LENGTH="] = "16"; // size of request
	// env_m["CONTENT_TYPE="] = "text/html";
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
	int fds1[2];
	int fds2[2];
	std::string body = "";
	char buffer[1024];
	char *args[] = {"/Users/cabouelw/goinfre/.brew/bin/php-cgi", NULL};

	pipe(fds1);
	char **env = init_env("tst.com", "fname=test&name=chb", status, path, page, method);
	int pid = fork();
	if (!pid)
	{
		fds2[1] = dup(1);
		dup2(fds1[1], 1);
		execve(args[0], args, (char **)env);
	}
	else
	{
		int size = 0;
		int r;
		close(fds1[1]);
		dup2(1, fds2[1]);
		while ((r = read(fds1[0], buffer, sizeof(buffer))))
		{
			body.append(buffer, r);
			if (r < 1024)
				break;
		}
	}
	body.erase(0, body.find("\r\n\r\n"));
	return (body);
}

int main ()
{
	std::string s = runCgi("/Users/cabouelw/Desktop/WebServ", "POST", "index.php", "200");
	std::cout << s << std::endl;
}