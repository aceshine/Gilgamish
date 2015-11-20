#include <iostream>

#include "main.hpp"

extern int static_main();
extern int dynamic_main();

int main(int argc, char* argv[])
{
    std::cout << "main" << std::endl;
	static_main();
	dynamic_main();
    return 0;
}
