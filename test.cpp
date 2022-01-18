#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file/* ("test.cpp") */;
    file.open("test.cpp");
    std::cout << file.good() << std::endl;
    return 0;
}
