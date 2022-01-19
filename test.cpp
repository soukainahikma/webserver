#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file/* ("test.cpp") */;
    file.open("test");
    std::cout << file.fail() << std::endl;
    return 0;
}
