#include "./request_response/request.hpp"
#include <iostream>
#include<map>
#include <cstring>
int main()
{
	std::string buffer ="POST / HTTP/1.1\r\nUser-Agent: PostmanRuntime/7.25.0\nAccept: */*\nPostman-Token: bb0ab5d3-23ca-4782-984e-c396fc7b55a0\nHost: localhost:3000\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\nContent-Type: multipart/form-data; boundary=--------------------------113141188081680465308854\nContent-Length: 329\n\n----------------------------113141188081680465308854\nContent-Disposition: form-data; name=\"index\"; filename=\"index.html\"\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<body>\n\n<h1 style=\"text-align:center ;color:green\">Welcome to our WEB SERVER</h1>\n\n</body>\n</html>\n----------------------------113141188081680465308854--\n\nHTTP/1.1 200  OK\nContent-Type: text/html\n\n\n";
	// Request req(buffer,8080);
	// std::cout<< buffer<<std::endl;
	// std::map<std::string,std::string> mymap = req.getRequest();
	size_t found;
	if((found = buffer.find("\n\n"))!= std::string::npos)
		std::cout<< buffer.substr(0,found)<<std::endl;
				
	// for (std::map<std::string,std::string>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    	// std::cout << it->first << " => " << it->second << '\n';


}
/* ----------------------------918282565863101620614950
Content-Disposition: form-data; name="index"; filename="index.html"
Content-Type: text/html

<!DOCTYPE html>
<html>
<body>

<h1 style="text-align:center ;color:green">Welcome to our WEB SERVER</h1>

</body>
</html>
----------------------------918282565863101620614950--

HTTP/1.1 200  OK
Content-Type: text/html


 */