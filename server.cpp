#include "server/socket.hpp"
#include "request_response/response.hpp"
int main()
{
	response resp;
	server_socket mysocket;
	int new_socket = 0;
	int valread;
	char buffer[1024] = {0};
	mysocket.set_server();
	while(1)
	{
		if((new_socket = mysocket.accept_socket())<0)
		{
			perror("failed to accept: ");
			return(0);
		}
		valread = read(new_socket, buffer, 1024);
		const char *hello = resp.get_header("pages/info.html").c_str();;
		send(new_socket, hello, strlen(hello), 0);
		resp.header_cleaner();
		close(new_socket);
	}
	return 0;
}