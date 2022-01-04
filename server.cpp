#include "request.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 8080
int main()
{
	int server_fd;// this will hold the socket file descriptor in the socket creation
	int new_socket;//this will hold the the newsocket accepted by the original socket
	int valread; // this is gonna read from the new socket;
	struct sockaddr_in address; // this is part of the socket structure(i need to go read the file again) have to check if i need to implement it
	int opt  = 1;// i will need it in setsockopt
	int addrlen = sizeof(address);// need it as a parameter in the function accept to accept the new socket
	char buffer[1024] = {0};//to read from
	request req;/// this the requested thing from the client to be sent
	// AF_INET         2               /* internetwork: UDP, TCP, etc. */
	  //SOCK_STREAM/* stream socket */
	  // ask why we put 0 in the protocole argument
	if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0)
	{
		 perror("cannot create socket"); 
		return(0);
	}
	/* The server might set some socket options (this is optional, but you 
	can see that the server code above does just that to be able to re-use the same 
	address over and over again if you decide to kill and re-start the server right away). (still do not know why should i use it)*/
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	{
		perror("setsockopt");
		return(0);
	}
	/* htonl and htons convets a long or short int to a network representation */
	memset((char *)&address, 0, sizeof(address)); 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 
	address.sin_port = htons(PORT);
	// now that the struct sockaddr is setup we can bind, here we specified the port we re gonna listen through
	if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0)
	{
		perror("bind failed");
		return(0);
	}
	// now that our socket has an adress and its bind we need to connect it
	//for now we set the second parameter to 3(backlog) then we see why
	if(listen(server_fd,3) < 0)
	{
		perror("listening failed: ");
		return(0);
	}
	// now that the connection is successful we need to accept the upcommint requests
	while(1)
	{
	if((new_socket = accept(server_fd,(struct sockaddr*)& address,(socklen_t *)&addrlen))<0)
	{
		perror("failed to accept: ");
		return(0);
	}
	valread = read(new_socket, buffer, 1024);
	// std::cout << buffer;
	const char *hello = req.get_header("info.html").c_str();;
	// strcpy(hello, req.get_header("info.html").c_str());
	send(new_socket, hello, strlen(hello), 0);
	//reset the header
	// reset hello
	req.header_cleaner();
	// hello  = strdup("");
	close(new_socket);
	}
	return 0;
}