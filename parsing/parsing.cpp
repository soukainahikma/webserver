#include "Server.hpp"

void print_error(int i, std::string v)
{
	static std::string err[] = {
		"error0",
		"error1",
		"error2",
		"error3"
	};
	std::cout << v << "----";
	throw err[i];
}

std::vector<std::string> split(std::string str, char c)
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

void check_brace(std::string &line, Server &serv, Location &locat)
{
	int idx = 0;
	int i = 0;
	std::string key;
	std::string value;

	while (line[idx] && line[idx] != '{')
		idx++;
	if (line[idx] == '{')
	{
		key = get_key(line, i);
		value = get_value(line, i);
		if (key.empty() && serv.get_server_open() == 1)
		{
			serv.set_server_open(2);
		}
		else if (key.empty() && serv.get_location_open() == 1)
		{
			serv.set_location_open(2);
		}
		else if (!key.compare("server"))
		{
			if (!value.empty() || serv.get_server_open())
				print_error(0, key);
			serv.set_server_open(2);
		}
		else if (!key.compare("location"))
		{
			// std::cout << value << serv.get_location_open() <<  "|\n";
			if (value.empty() || serv.get_location_open())
			{
				// std::cout << "sdfsdf\n";
				print_error(1, key);
			}
			serv.set_location_open(2);
			locat.set_path(value);
		}
		line.erase(0, idx + 1);
	}
	else if (!line[idx] && (line.find("server") != std::string::npos || line.find("location") != std::string::npos))
	{
		key = get_key(line, i);
		value = get_value(line, i);
		if (!key.compare("server") && value.empty())
		{
			if (serv.get_server_open()){
				// std::cout << "sdfsdf\n";
				print_error(3, key);
			}
			serv.set_server_open(1);
			line.clear();
		}
		else if (!key.compare("location") && !value.empty())
		{
			if (serv.get_location_open()){
				// std::cout << "sdfsdf\n";
				print_error(3, key);
			}
			locat.set_path(value);
			serv.set_location_open(1);
			line.clear();
		}
		else if (!key.compare("server") || !key.compare("location"))
			print_error(3, key);
	}
}


void fill_location(std::string key, std::string value, Server &serv, Location &locat)
{
	if (!key.compare("autoindex"))
		locat.set_autoindex(value);
	else if (!key.compare("index"))
	{
		std::vector<std::string> splt;
		splt = split(value, ' ');
		locat.set_index(splt);
	}
	else if (!key.compare("request_method"))
	{
		// std::vector<std::string> splt;
		// value.erase(value.find('['), 1);
		// value.erase(value.find(']'), 1);
		// splt = split(value, ',');
		// std::cout << value << "|" << splt[0] << "\n";
		locat.set_request_method(value);
	}
	else if (!key.compare("fastcgi_pass"))
		locat.set_fastcgi_pass(value);
	else if (!key.compare("upload_enable"))
		locat.set_upload_enable(value);
	else if (!key.compare("upload_store"))
		locat.set_upload_store(value);
	else if (!key.compare("return"))
	{
		std::vector<std::string> s = split(value, ' ');
			if (s.size() > 2)
				print_error(1, value);
		locat.set_return(s);
	}
	else
		print_error(3, key);
}

int find_last(std::string s)
{
	int idx = s.length() -1;
	while (s[idx])
	{
		if (s[idx] != ' ' && s[idx] != '\t')
		{
			if (s[idx] != ';')
				return (1);
			else
				return (0);
			std::cout << "|" << s[idx] << "|\n";
		}

		idx--;
	}
	return (0);
}

void fill_server(std::string key, std::string value, Server &serv, Location &locat, std::vector<Server> &vec_serv, std::string &line, std::map<int,int> &m)
{
	if (serv.get_server_open() == 2)
	{
		if (!key.empty() && find_last(line))
			print_error(0, line);
		// std::cout << line << "--------------" << key << "\n";
		if (!key.compare("listen"))
		{
			serv.set_listen(value);
			m[serv.get_listen()] = serv.get_listen();
		}
		else if (!key.compare("server_name"))
		{
			std::vector<std::string> s = split(value, ' ');
			serv.set_server_name(s);
		}
		else if (!key.compare("client_max_body_size"))
			serv.set_client_max_body_size(value);
		else if (!key.compare("host"))
			serv.set_host(value);
		else if (!key.compare("error_page"))
		{
			std::vector<std::string> s = split(value, ' ');
			if (s.size() > 2)
				print_error(1, value);
			serv.set_error_page(s[0], s[1]);
		}
		else if (!key.compare("root"))
			serv.set_root(value);
		else if (serv.get_location_open() == 2 && line.find("}") != std::string::npos)
		{
			// std::cout << "sgsg\n";
			serv.set_location(locat);
			locat.Clear();
			serv.set_location_open(0);
		}
		else if (serv.get_server_open() == 2 && line.find("}") != std::string::npos)
		{
			serv.set_server_open(0);
			if (serv.get_listen() == 80)
				m[80] = 80;
			vec_serv.push_back(serv);
			serv.Clear();
		}
		else if (serv.get_location_open() == 2)
			fill_location(key, value, serv, locat);
		else
		{
			// std::cout << key << "|" << value << "hna\n";
			print_error(3, key);
		}
	}
	else
		print_error(2, key);
}

std::vector<Server> parsing(std::string file, std::map<int,int> &m)
{
	std::string line;
	std::ifstream myfile(file);
	std::vector<std::string> splt;
	std::vector<Server> vec_serv;
	Server serv;
	Location locat;
	std::string key;
	std::string value;

	int idx;
	int pos;
	int i = 0;
	int nbline = 0;
	int size;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if ((pos = line.find("#")) != std::string::npos)
				line.erase(pos, line.length());
			check_brace(line, serv, locat);
			splt = split(line, ';');
			i = 0;
			// std::cout << splt.size() << "\n";
			// exit(0);
			size = splt.size();
			while (i < size)
			{
				idx = 0;
				// std::cout << splt[i] << "\n";
				key = get_key(splt[i], idx);
				value = get_value(splt[i], idx);
				if ((!key.empty() && !value.empty()) || line.find("}") != std::string::npos)
					fill_server(key, value, serv, locat, vec_serv, line, m);
				else if (!key.empty() && value.empty())
					print_error(0, key);
				// std::cout << key << "|" << value << "\n";
				i++;
			}
		}
		if (serv.get_server_open())
			print_error(1, "opned");
		myfile.close();
	}
	else
	{
		std::cout << "Error file: ";
		throw file;
	}
	return (vec_serv);
}

void	print_all(std::vector<Server> &vec_serv, std::map<int,int> m)
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
		std::cout << "server {\n";
		std::cout << "listen  " << vec_serv[i].get_listen() << "\n";
		std::cout << "host  " << vec_serv[i].get_host() << "\n";
		int p = 0;
				std::cout << "server_size  " << vec_serv[i].get_server_name().size() << "\n";
			for (std::map<std::string, int>::iterator it = vec_serv[i].get_server_name().begin(); it != vec_serv[i].get_server_name().end(); it++)
			{
				std::cout << "server_name  " << it->first << "\n";
			}


			
		std::cout << "client_max_body_size  " << vec_serv[i].get_client_max_body_size() << "\n";
		std::cout << "error_page  403 " << vec_serv[i].get_error_page()["403"] << "\n";
		std::cout << "error_page  404 " << vec_serv[i].get_error_page()["404"] << "\n";
		std::cout << "error_page  500 " << vec_serv[i].get_error_page()["500"] << "\n";
		std::cout << "error_page  502 " << vec_serv[i].get_error_page()["502"] << "\n";
		std::cout << "root  " << vec_serv[i].get_root() << "\n";

		int d = 0;
		while (d < vec_serv[i].get_location().size())
		{
			std::cout << "\n\n------------ location  "<< d + 1 << "---------------\n\n";
			std::cout << "location  ";
			if (vec_serv[i].get_location()[d].get_equal())
				std::cout << "= ";
			std::cout << vec_serv[i].get_location()[d].get_path() << "{\n";
			r = 0;
			while (r < vec_serv[i].get_location()[d].get_return().size())
				std::cout << "return  " << vec_serv[i].get_location()[d].get_return()[r++] << "\n";
			std::cout << "fastcgi_pass  " << vec_serv[i].get_location()[d].get_fastcgi_pass() << "\n";
			std::cout << "upload_enable  " << vec_serv[i].get_location()[d].get_upload_enable() << "\n";
			std::cout << "upload_store  " << vec_serv[i].get_location()[d].get_upload_store() << "\n";
			std::cout << "autoindex  " << vec_serv[i].get_location()[d].get_autoindex() << "\n";
			int p = 0;
			while (p < vec_serv[i].get_location()[d].get_index().size())
				std::cout << "index  " << vec_serv[i].get_location()[d].get_index()[p++] << "\n";
			p = 0;
			while (p < vec_serv[i].get_location()[d].get_request_method().size())
				std::cout << "request_method  " << vec_serv[i].get_location()[d].get_request_method()[p++] << "\n";
			std::cout << "}\n";
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

// int main()
// {
// 	try
// 	{
// 		std::map<int,int> m;
// 		std::vector<Server> vec_serv = parsing("webserv.conf", m);
// 		print_all(vec_serv, m);
// 	}
// 	catch(std::string e)
// 	{
// 		std::cerr << e << '\n';
// 	}
// }
