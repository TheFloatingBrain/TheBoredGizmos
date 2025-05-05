#include <iostream>
#include <string>

int main(int argc, char** args)
{
	std::string name;
	std::cout << "What is your name?: ";
	std::getline(std::cin, name);
	std::cout << "Hello " << name << " build succeeded.\n";
	return 0;
}
