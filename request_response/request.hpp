#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <sstream>

#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"	   /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"	   /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"	   /* Cyan */
#define WHITE "\033[37m"   /* White */

struct body_struct
{
	std::string filename;
	std::string content_type;
	std::string Content_Disposition;
	std::string name;
	std::string body;
};

typedef struct s_cgi
{
	std::string root;
	std::string path;
	std::string page;
} t_cgi;

class Request
{
public:
	typedef std::string string;
	typedef std::map<string, string> map_request;
	typedef std::vector<string> vector_request;
	Request();
	Request(std::string buffer, int port);
	Request &operator=(Request &r);
	vector_request split_buffer(std::string myText, char c);
	void fill_header(std::string buffer);
	void body_info(body_struct &info, std::string str);
	body_struct initializer();
	std::string get_content_type();

	void fill_body(void);
	void fill_map(std::string s);

	std::string getQueryVar();

	map_request getRequest();

	int get_port();

	std::vector<body_struct> &getBodyStructs();

	std::string &getBodyString();

	void map_printer();
	void vec_printer();

	std::string &get_query();

	std::string get_boundary();

private:
	std::string query_var;
	map_request map_head;
	bool is_error_header;
	std::string body;
	int port;
	std::string boundary;
	std::string query;
	std::string contentType;
	std::vector<body_struct> myfiles; // if (|Content-Type| => multipart/form-data) myfiles| myfiles.size() != 0
};
#endif
/*
POST / HTTP/1.1
Host: 0.0.0.0:5000
Transfer-Encoding: chunkedContent-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW

----WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="bxvbxcb"

cxbxcbxcb
----WebKitFormBoundary7MA4YWxkTrZu0gW
*/