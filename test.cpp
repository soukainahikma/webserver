#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
void fileCheck(const char *fileName)
{

	if (!access(fileName, F_OK))
	{
		printf("The File %s\t was Found\n", fileName);
	}
	else
	{
		printf("The File %s\t not Found\n", fileName);
	}

	if (!access(fileName, R_OK))
	{
		printf("The File %s\t can be read\n", fileName);
	}
	else
	{
		printf("The File %s\t cannot be read\n", fileName);
	}

	if (!access(fileName, W_OK))
	{
		printf("The File %s\t it can be Edited\n", fileName);
	}
	else
	{
		printf("The File %s\t it cannot be Edited\n", fileName);
	}

	if (!access(fileName, X_OK))
	{
		printf("The File %s\t is an Executable\n", fileName);
	}
	else
	{
		printf("The File %s\t is not an Executable\n", fileName);
	}
}
int main()
{
	std::ifstream file /* ("test.cpp") */;
	file.open("file");
	fileCheck("file");
	// std::cout << file.good() << std::endl;
	return 0;
}
