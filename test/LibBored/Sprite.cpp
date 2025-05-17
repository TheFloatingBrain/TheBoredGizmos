#include <LibBored/Sprite.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace Bored:: GAME_NAME ;

TEST_CASE("Sprite Atlas", "[sprite_atlas]") {
	spdlog::set_level(spdlog::level::trace);
	SetTraceLogCallback(log);
	spdlog::info("{} is the name of the game.", gameName);
	spdlog::info("{} is the resource path.", resourceDirectory.string());
	const float screenWidth = 64;
	const float screenHeight = 64;
	SetConfigFlags(FLAG_WINDOW_HIDDEN);
	InitWindow(screenWidth, screenHeight, gameName.data());
	SetTargetFPS(1);
	auto atlasResult = loadSpriteAtlas(gameName, "Sprite-0001");
	REQUIRE(atlasResult == true);
	Renderer2D renderer{std::move(atlasResult.result), Vector2{1.f, 1.f}};
	auto testSpriteResult = renderer.atlas.retrieveSprite("Test");
	REQUIRE(testSpriteResult == true);
	auto testSprite = testSpriteResult.result;
	Rectangle sourceRect = {0};
	REQUIRE(testSprite.setSourceRect(sourceRect) == true);
	REQUIRE(sourceRect.x == 0);
	REQUIRE(sourceRect.y == 0);
	REQUIRE(sourceRect.width == 32);
	REQUIRE(sourceRect.height == 32);
}

