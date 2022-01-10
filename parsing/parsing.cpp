#include "Server.hpp"

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

	if (!line[idx])
		return ("");
	while (line[idx] == ' ' || line[idx] == '\t')
		idx++;
	while (line[end + idx] && line[end + idx] != ' ' && line[end + idx] != '\t' && line[end + idx] != '\n')
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
	while (line[end + idx] && line[end + idx] != ';')
		end++;
	str = line.substr(idx, end);
	return (str);
}

void	fill_location(std::string key, std::string value, Server &serv, Location &locat)
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
		std::vector<std::string> splt;
		value.erase(value.find('['), 1);
		value.erase(value.find(']'), 1);
		splt = split(value, ',');
		// std::cout << value << "|" << splt[0] << "\n";
		locat.set_request_method(splt);
	}
	else if (!key.compare("fastcgi_pass"))
		locat.set_fastcgi_pass(value);
	else if (!key.compare("upload_enable"))
		locat.set_upload_enable(value);
	else if (!key.compare("upload_store"))
		locat.set_upload_store(value);
	else if (!key.compare("return"))
		locat.set_return(value);
	else if (!key.compare("}"))
		serv.set_location_open(false);
}

void	set_value(std::string key, std::string value, Server &serv, Location &locat, std::vector<Server> &vec_serv)
{
	if (!key.compare("listen"))
		serv.set_listen(value);
	else if (!key.compare("host"))
		serv.set_host(value);
	else if (!key.compare("server_name"))
	{
		std::vector<std::string> splt;
		splt = split(value, ' ');
		serv.set_server_name(splt);
	}
	else if (!key.compare("client_max_body_size"))
		serv.set_client_max_body_size(value);
	else if (!key.compare("error_page"))
	{
		std::vector<std::string> splt;
		splt = split(value, ' ');
		serv.set_error_page(splt);
	}
	else if (!key.compare("root"))
		serv.set_root(value);
	else if (!key.compare("location"))
	{
		locat.set_path(value);
		serv.set_location_open(true);
	}
	else if (!key.compare("}") && serv.get_location_open())
	{
		serv.set_location_open(false);
		serv.set_location(locat);
		// std::cout << "daz "<< serv.get_location().size()<< "\n";
	}
	else if (!key.compare("}") && !serv.get_location_open())
	{
		// std::cout << "daz02  " << serv.get_location().size()<< "\n";
		vec_serv.push_back(serv);
	}
	else if (serv.get_location_open())
		fill_location(key, value, serv, locat);
}

std::vector<Server> parsing(std::string file)
{
	std::string					line;
	std::ifstream				myfile(file);
	std::vector<std::string>	splt;
	std::vector<Server>			vec_serv;
	Server						serv;
	Location					locat;
	std::string					key;
	std::string					value;
	int							idx;
	int i = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			splt = split(line, ';');
			i = 0;
			while (i < splt.size())
			{
				idx = 0;
				key = get_key(splt[i], idx);
				value = get_value(splt[i], idx);
				set_value(key, value, serv, locat, vec_serv);
				i++;
			}
		}
		// std::cout << vec_serv[0].get_server_name()[0] << "|" << vec_serv[0].get_location()[4].get_request_method()[0] <<  "\n";
		myfile.close();
	}
	return (vec_serv);
}

/* int main()
{
	parsing("webserv.conf");
	return (0);
} */