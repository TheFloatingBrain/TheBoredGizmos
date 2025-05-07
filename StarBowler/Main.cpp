#include <StarBowler/Sprite.hpp>
using namespace Bored:: GAME_NAME;

int main(int argc, char** args)
{
	std::cout << gameName << " is the name of the game.\n";
	std::cout << resourceDirectory << " is the resource path.\n";
	const int screenWidth = 800;
	const int screenHeight = 450;
	SetTraceLogCallback(log);
	InitWindow(screenWidth, screenHeight, gameName.data());
	auto atlasResult = loadSpriteAtlas(gameName, "Spritesheet");
	if(atlasResult == false) {
		spdlog::critical(to_string(atlasResult.error));
		return 1;
	}
	auto atlas = atlasResult.result;
	auto playerShipSpriteResult = atlas.retrieveSprite("PlayerShip");
	if(playerShipSpriteResult == false) {
		spdlog::critical(to_string(playerShipSpriteResult.error));
		return 1;
	}
	auto playerShipSprite = playerShipSpriteResult.result;
	Renderer2D renderer{atlas};
	Camera2D camera = {0};
	while(WindowShouldClose() == false)
	{
		ClearBackground(WHITE);
		BeginMode2D(camera);
			renderer.draw(playerShipSprite, Vector2{screenWidth / 2, screenHeight / 2});
		EndMode2D();
	}
	CloseWindow();
	return 0;
}

