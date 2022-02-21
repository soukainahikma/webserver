#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "../parsing/Server.hpp"
#include "../request_response/request.hpp"

std::string	getdata(Request &req)
{
	std::string newdata = req.getBodyString();

	if (req.getRequest()["Content-Type"].find("multipart/form-data") != std::string::npos)
	{
		newdata = "";
		for (size_t i = 0; i < req.getBodyStructs().size(); i++)
		{
			if(req.getBodyStructs()[i].filename == "")
			{
				if (i != 0)
					newdata += "&";
				newdata += req.getBodyStructs()[i].name + "=" + req.getBodyStructs()[i].body;
			}
		}
	}
	return newdata;
}

char	**init_env(std::string status, std::string path, std::string page, Request &req, std::string &newdata)
{

	setenv("REDIRECT_STATUS", status.c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SCRIPT_NAME", page.c_str(), 1);
	setenv("SCRIPT_FILENAME", page.c_str(), 1);
	setenv("CONTENT_LENGTH", req.getRequest()["Content-Length"].c_str(), 1);
	// setenv("name=chb&tele=dsfsdf", "19", 1);
	setenv("REQUEST_METHOD", req.getRequest()["Method"].c_str(), 1);
	if (req.getRequest()["Content-Type"].find("multipart/form-data") != std::string::npos)
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	else
		setenv("CONTENT_TYPE", req.getRequest()["Content-Type"].c_str(), 1);
	setenv("PATH_INFO", path.c_str(), 1);
	setenv("PATH_TRANSLATED", path.c_str(), 1);
	setenv("QUERY_STRING", req.getQueryVar().c_str(), 1);
	setenv("SERVER_NAME", split(req.getRequest()["Host"], ':')[0].c_str(), 1);
	setenv("HTTP_COOKIE", req.getRequest()["Cookie"].c_str(), 1);
	// setenv("HTTP_COOKIE", "test=value; test2=value2", 1);
	setenv("SERVER_PORT", std::to_string(req.get_port()).c_str(), 1);
	setenv("SERVER_PROTOCOL", "\"HTTP/1.1\"", 1);
	extern char **environ;
	return (environ);
}

std::string get_root(std::string root)
{
	int r = 0;

	if ((r = root.find("Desktop/")) != std::string::npos)
	{
		r += 10;
		while (root[r] != '/')
			r++;
		root.erase(r, root.length());
	}
	return root;
}

std::string runCgi(std::string root, std::string path, std::string page, std::string &status, Request &req)
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
	if (size_read < 6)
		return NULL;
	newdata = getdata(req);
	char **env = init_env(status, path, page, req, newdata);
	fd_old[0] = dup(0);
	fd_old[1] = dup(1);
	pipe(pipefd_data);
	if (req.getRequest()["Method"] != "GET")
		write(pipefd_data[1], newdata.c_str(), newdata.size());
	close(pipefd_data[1]);
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
		root_c = "/goinfre/aboulbaz/.brew/bin/php-cgi";
		args[0] = (char*)(root_c.c_str());
		args[1] = NULL;
	}
	pipe(pipefd);
	int pid = fork();
	if (!pid)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		close(pipefd[0]);

		dup2(pipefd_data[0], 0);
		close(pipefd_data[0]);

		execve(args[0], args, (char **)env);
		status = "500";
	}
	else
	{
		close(pipefd_data[0]);

		close(pipefd[1]);

		bzero(buffer, size_read);
		int r;
		while ((r = read(pipefd[0], buffer, size_read - 1)))
			body.append(buffer, r);

		wait(&pid);
		close(pipefd[0]);

		dup2(fd_old[0], 0);
		dup2(fd_old[1], 1);

		delete [] args;
	}
	body.erase(0, body.find("\r\n\r\n"));
	return (body);
}
