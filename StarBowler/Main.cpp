#include <StarBowler/Sprite.hpp>
using namespace Bored:: GAME_NAME;

int main(int argc, char** args)
{
	std::cout << gameName << " is the name of the game.\n";
	std::cout << resourceDirectory << " is the resource path.\n";
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, gameName.data());
	auto atlasResult = loadSpriteAtlas(gameName, "Spritesheet");
	if(atlasResult == false) {
		std::cerr << to_string(atlasResult.error) << "\n";
	}
	auto atlas = atlasResult.result;
	auto shipSpriteResult = atlas.retrieveSprite("Ship");
	if(shipSpriteResult == false) {
		std::cerr << to_string(shipSpriteResult.error) << "\n";
	}
	else { std::cout << "Success!\n"; }
	CloseWindow();
	return 0;
}

