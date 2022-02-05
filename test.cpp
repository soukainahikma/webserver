#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int main(int ac, char **argv){
    struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0)    
        return 1;
	if (fileStat.st_mode & S_IRUSR)
        std::cout << "read " << std::endl;
    if (fileStat.st_mode & S_IWUSR)
        std::cout << "write" << std::endl;
    if (fileStat.st_mode & S_IXUSR)
        std::cout << "execute " << std::endl;
}
