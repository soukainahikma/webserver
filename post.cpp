#include "./request_response/request.hpp"
#include <iostream>
#include<map>
#include <cstring>
int main()
{
	char *buffer =strdup("POST / HTTP/1.1\nHost: localhost:3000\nContent-Type: multipart/form-data; boundary=--------------------------918282565863101620614950\nCache-Control: no-cache\nPostman-Token: 846339bc-40f4-9dde-1933-b17ecc45951c \n\n----------------------------918282565863101620614950\nContent-Disposition: form-data; name=\"index\"; filename=\"index.html\"\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<body>\n\n<h1 style=\"text-align:center ;color:green\">Welcome to our WEB SERVER</h1>\n\n</body>\n</html>\n----------------------------918282565863101620614950--");
	Request req(buffer,8080);
	// std::cout<< buffer<<std::endl;
	// std::map<std::string,std::string> mymap = req.getRequest();
	req.map_printer();
	// for (std::map<std::string,std::string>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    	// std::cout << it->first << " => " << it->second << '\n';


}
/*
POST  HTTP/1.1
Host: localhost:3000
Content-Type: multipart/form-data; boundary=--------------------------918282565863101620614950
Cache-Control: no-cache
Postman-Token: 846339bc-40f4-9dde-1933-b17ecc45951c 

----------------------------918282565863101620614950
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




/* 
POST / HTTP/1.1
Host: localhost:3000
Content-Type: multipart/form-data; boundary=--------------------------918282565863101620614950
Cache-Control: no-cache
Postman-Token: 846339bc-40f4-9dde-1933-b17ecc45951c 

----------------------------918282565863101620614950
Content-Disposition: form-data; name=\"index\"; filename=\"index.html\"
Content-Type: text/html

<!DOCTYPE html>
<html>
<body>

<h1 style=\"text-align:center ;color:green\">Welcome to our WEB SERVER</h1>

</body>
</html>
----------------------------918282565863101620614950-- */