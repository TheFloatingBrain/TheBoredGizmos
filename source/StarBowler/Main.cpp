#include <StarBowler/Player.hpp>
using namespace Bored;
using namespace Bored:: GAME_NAME;

int main(int argc, char** args)
{
	spdlog::set_level(spdlog::level::trace);
	SetTraceLogCallback(log);
	spdlog::info("{} is the name of the game.", gameName);
	spdlog::info("{} is the resource path.", resourceDirectory.string());
	const float screenWidth = 240;
	const float screenHeight = 240;
	const float virtualScreenWidth = 64 * 8;
	const float virtualScreenHeight = 64 * 8;
	const Vector2 scale{
		virtualScreenWidth / screenWidth, 
		virtualScreenHeight / screenHeight
	};
	spdlog::trace("Virtual Scaling Factors x {} y {}", scale.x, scale.y);
	InitWindow(screenWidth, screenHeight, gameName.data());
	SetTargetFPS(60);
	auto atlasResult = loadSpriteAtlas(gameName, "Spritesheet");
	if(atlasResult == false) {
		spdlog::critical(to_string(atlasResult.error));
		return 1;
	}
	Renderer2D renderer{std::move(atlasResult.result), scale};
	auto playerShipSpriteResult = renderer.atlas.retrieveSprite(Player::entityName);
	if(playerShipSpriteResult == false) {
		spdlog::critical(to_string(playerShipSpriteResult.error));
		return 1;
	}
	auto playerShipSprite = playerShipSpriteResult.result;
	spdlog::trace("Player Ship Sprite w {} h {}", playerShipSprite.width, playerShipSprite.height);
	Camera2D camera = {
		.offset = {screenWidth / 2.0f, screenHeight / 2.0f},
		.target = {0.0f, 0.0f},
		.rotation = 0.0f,
		.zoom = 1.0f,
	};

	while(WindowShouldClose() == false)
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
			BeginMode2D(camera);
				renderer.draw(playerShipSprite, SpatialProperties2D{.position{0, 0},.rotation{180}});
				DrawCircle(0, 0, 2.f, RED);
			EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

