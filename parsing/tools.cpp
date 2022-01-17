#include "Server.hpp"


void print_error(int i, std::string v)
{
	static std::string err[] = {
		"syntax Error: Missing close brace {" + v + "}",
		"syntax Error: unexpected token {" + v + "}",
		"syntax Error: Missing open brace {" + v + "}",
		"syntax Error: Missing ';' {" + v + "}",
		"syntax Error: too many arguments {" + v + "}",
		"syntax Error: no server to fill with {" + v + "}",
		"syntax Error: Missing arguments to {" + v + "}",
		"syntax Error: permission denied {" + v + "}",
		"syntax Error: duplicated key {" + v + "}",
		"syntax Error: unexpected form (example : NN/NNNM) {" + v + "}",
		"syntax Error: am i a joke to you {" + v + "}",
		"syntax Error: Only accept MB {" + v + "}",
		"syntax Error: unexpected token (on/off) -_- {" + v + "}",
		"syntax Error: unexpected form {" + v + "}",
		"syntax Error: somthing missing in {" + v + "}",
		"syntax Error: duplicated location {" + v + "}"
	};
	throw err[i];
}

std::vector<std::string> split(std::string &str, char c)
{
	size_t pos = 0;
	std::string token;
	std::vector<std::string> spl;
	while ((pos = str.find(c)) != std::string::npos)
	{
		token = str.substr(0, pos);
		spl.push_back(token);
		str.erase(0, pos + 1);
		if (str[0] && str.find(c) == std::string::npos)
		{
			token = str.substr(0, str.length());
			spl.push_back(token);
		}
	}
	if (spl.size() == 0)
		spl.push_back(str);
	return (spl);
}


std::string get_key(std::string &line, int &idx)
{
	int end = 0;
	std::string str;

	// std::cout << line << "|" << idx << "|hna\n";
	if (!line[idx])
		return ("");
	while (line[idx] == ' ' || line[idx] == '\t')
		idx++;
	while (line[end + idx] && line[end + idx] != ' '
		&& line[end + idx] != '\t' && line[end + idx] != '=' &&
			line[end + idx] != '\n' && line[end + idx] != '{' &&
				line[end + idx] != '}')
		end++;
	str = line.substr(idx, end);
	idx += end;
	return (str);
}

std::string get_value(std::string &line, int &idx)
{
	int end = 0;
	std::string str;

	if (!line[idx])
		return ("");
	while (line[idx] == ' ' || line[idx] == '\t')
		idx++;
	while (line[end + idx] && line[end + idx] != ';' &&
		line[end + idx] != '{' && line[end + idx] != '}')
		end++;
	str = line.substr(idx, end);
	return (str);
}

void	print_all(std::vector<Server> &vec_serv, std::map<int,int> &m)
{
	int idx = 0;
	int i = 0;
	int r = 0;
	// std::cout << vec_serv.size() << "\n";
	while (i < vec_serv.size())
	{
		std::cout << "\n\n------------ server " << i + 1 << "---------------\n";
		std::cout << "-----------------------------------\n\n";
		idx = 0;
		std::cout << "server {\n\n";
		std::cout << "	listen  " << vec_serv[i].get_listen() << "\n";
		std::cout << "	host  " << vec_serv[i].get_host() << "\n";
		int p = 0;
				std::cout << "	server_size  " << vec_serv[i].get_server_name().size() << "\n";
			for (std::map<std::string, int>::iterator it = vec_serv[i].get_server_name().begin(); it != vec_serv[i].get_server_name().end(); it++)
			{
				std::cout << "	server_name  " << it->first << "\n";
			}


			
		std::cout << "	client_max_body_size  " << vec_serv[i].get_client_max_body_size() << "\n";
		std::cout << "	error_page  403 " << vec_serv[i].get_error_page()["403"] << "\n";
		std::cout << "	error_page  404 " << vec_serv[i].get_error_page()["404"] << "\n";
		std::cout << "	error_page  500 " << vec_serv[i].get_error_page()["500"] << "\n";
		std::cout << "	error_page  502 " << vec_serv[i].get_error_page()["502"] << "\n";
		std::cout << "	root  " << vec_serv[i].get_root() << "\n";

		int d = 0;
		while (d < vec_serv[i].get_location().size())
		{
			std::cout << "\n\n------------ location  "<< d + 1 << "---------------\n\n";
			std::cout << "location  ";
			if (vec_serv[i].get_location()[d].get_equal())
				std::cout << "= ";
			std::cout << vec_serv[i].get_location()[d].get_path() << "  {\n\n";
			r = 0;
			while (r < vec_serv[i].get_location()[d].get_return().size())
				std::cout << "	return		" << vec_serv[i].get_location()[d].get_return()[r++] << "\n";
			std::cout << "	fastcgi_pass		" << vec_serv[i].get_location()[d].get_fastcgi_pass() << "\n";
			std::cout << "	upload_enable		" << vec_serv[i].get_location()[d].get_upload_enable() << "\n";
			std::cout << "	upload_store		" << vec_serv[i].get_location()[d].get_upload_store() << "\n";
			std::cout << "	autoindex		" << vec_serv[i].get_location()[d].get_autoindex() << "\n";
			int p = 0;
			while (p < vec_serv[i].get_location()[d].get_index().size())
				std::cout << "	index		" << vec_serv[i].get_location()[d].get_index()[p++] << "\n";
			p = -1;
			std::cout << "	allow_method	[";
			while (++p < vec_serv[i].get_location()[d].get_request_method().size())
			{
				std::cout << vec_serv[i].get_location()[d].get_request_method()[p];
				if (p + 1 < vec_serv[i].get_location()[d].get_request_method().size())
					std::cout << ",";
			}
			std::cout << "]\n\n	}\n";
			d++;
		}
		std::cout << "}\n";
		i++;
	}
	int map = 0;
	std::cout << "\n\n------------ports :\n";
	for (std::map<int, int>::iterator i = m.begin(); i != m.end(); i++)
	{
		std::cout << i->first << "|\n";
	}
	
}
