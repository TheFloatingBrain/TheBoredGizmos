#include <StarBowler/Config.hpp>

using namespace Bored;

int main(int argc, char** args)
{
	std::cout << gameName << " is the name of the game.\n";
	std::cout << resourceDirectory << " is the resource path.\n";
	std::cout << "Resource exists: " << std::filesystem::exists(resourceDirectory) << "\n";
	return 0;
}

