#include "./request_response/request.hpp"
#include <iostream>
#include<map>
#include <cstring>
#include <unistd.h>
int main()
{
	char *str = "123456\0 123456";
	write(1,str,14);
	// std::cout<< str << " " << str[9] << std::endl;
}

