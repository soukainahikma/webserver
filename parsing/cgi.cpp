#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include "../parsing/Server.hpp"
#include "../request_response/request.hpp"

std::string	getdata(Request &req)
{
	// std::string newdata = req.getBodyString();

	std::string newdata = req.getBodyString();
	if (req.getRequest()["Method"] == "GET")
		newdata = req.getQueryVar();
	if (req.getRequest()["Content-Type"].find("multipart/form-data") != std::string::npos)
		newdata = req.getBodyString();
	else {
		size_t pos = 0;
		if ((pos = newdata.find_first_of("\n\n")) != std::string::npos)
			newdata.erase(0, pos + 3);
		if (req.getRequest()["Content-Type"].find("multipart/form-data") != std::string::npos)
		{
			newdata = "";
			for (size_t i = 0; i < req.getBodyStructs().size() - 1; i++)
			{
				if(req.getBodyStructs()[i].filename == "")
				{
					if (i != 0)
						newdata += "&";
					newdata += req.getBodyStructs()[i].name + "=" + req.getBodyStructs()[i].body;
				}
			}
		}
	}
	return newdata;
}

char	**init_env(std::string status, std::string path, std::string page, Request &req, std::string &newdata)
{
	setenv("SCRIPT_FILENAME", page.c_str(), 1);
	setenv("SCRIPT_NAME", (page.erase(0, page.find_last_of("/") + 1)).c_str(), 1);
	setenv("REMOTEaddr", std::to_string(req.get_port()).c_str(), 1);
	setenv("REQUEST_METHOD", req.getRequest()["Method"].c_str(), 1);
	if (req.getRequest()["Method"] == "GET")
	{
		setenv("QUERY_STRING", newdata.c_str(), 1);
		setenv("REQUEST_URI", (path + newdata).c_str(), 1);
	}
	else
	{
		setenv("REQUEST_URI", path.c_str(), 1);
		setenv("QUERY_STRING", "", 1);
	}
	setenv("SERVER_SOFTWARE", "webserv/1.0", 1);
	setenv("CONTENT_LENGTH", req.getRequest()["Content-Length"].c_str(), 1);
	setenv("CONTENT_TYPE", (req.getRequest()["Content-Type"]).c_str(), 1);
	setenv("PATH_INFO", path.c_str(), 1);
	setenv("REDIRECT_STATUS", status.c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SERVER_PROTOCOL", req.getRequest()["Protocol_version"].c_str(), 1);
	setenv("PATH_TRANSLATED", path.c_str(), 1);
	setenv("SERVER_NAME", split(req.getRequest()["Host"], ':')[0].c_str(), 1);
	setenv("HTTP_COOKIE", req.getRequest()["Cookie"].c_str(), 1);
	setenv("SERVER_PORT", NumberToString(req.get_port()).c_str() , 1);
	extern char **environ;
	return (environ);
}

std::string get_root()
{
	int r = 0;

	char path[256];
	getcwd(path, 256);
	return path;
}

void	fill_binary_cgi(t_cgi &cgi, char **&args)
{
	if (cgi.page.find(".py") != std::string::npos)
	{
		args = new char*[3];
		args[0] = (char*)"/usr/bin/python";
		args[1] = (char*)(cgi.page).c_str();
		args[2] = NULL;
	}
	else if (cgi.page.find(".php") != std::string::npos)
	{
		args = new char*[2];
		std::string user = getenv("USER");
		user = "/Users/" + user + "/goinfre/.brew/bin/php-cjg";
		// args[0] = (char*)user.c_str();
		args[0] = (char*)user.c_str();
		args[1] = NULL;
	}
	else
		throw ;// ma3raftch hno ndir
}

std::string runCgi(t_cgi &cgi, std::string &status, Request &req)
{
	int pipefd[2];
	int pipefd_data[2];
	int fd_old[2];
	char **args;

	std::string body = "";
	int size_read = 1024;
	std::string newdata = "";
	char buffer[size_read];
	std::string root_c;
	newdata = getdata(req);
	char **env = init_env(status, cgi.path, cgi.page, req, newdata);
	fd_old[0] = dup(0);
	fd_old[1] = dup(1);
	fill_binary_cgi(cgi, args);
	pipe(pipefd_data);
	pipe(pipefd);
	int pid = fork();
	if (!pid)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		close(pipefd[0]);

		dup2(pipefd_data[0], 0);
		close(pipefd_data[0]);
		close(pipefd_data[1]);

		execve(args[0], args, (char **)env);
		exit(1);
	}
	else
	{
		if (req.getRequest()["Method"] != "GET")
			write(pipefd_data[1], newdata.c_str(), newdata.size());
		close(pipefd_data[1]);
		close(pipefd_data[0]);

		close(pipefd[1]);

		bzero(buffer, size_read);
		int r;
		while ((r = read(pipefd[0], buffer, size_read - 1)))
			body.append(buffer, r);
		close(pipefd[0]);
		// wait(&pid);
		int test;
		waitpid(pid, &test, 0);
		if (WIFEXITED(test))
		{
			test = WEXITSTATUS(test);
			status = (test) ? "500" : status;
		}
		dup2(fd_old[0], 0);
		dup2(fd_old[1], 1);

		delete [] args;
	}
	return (body);
}
