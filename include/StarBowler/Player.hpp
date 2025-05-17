#include <LibBored/Sprite.hpp>
#ifndef BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
namespace Bored:: GAME_NAME
{
	using EntityID = size_t;
	enum class GameMode { D3, D2 };
	const std::string& to_string(GameMode mode)
	{
		thread_local const std::string D2{"2D"};
		thread_local const std::string D3{"2D"};
		if(mode == GameMode::D3) return D3;
		return D2;
	}
	struct Input {
		size_t up, down, left, right, bowl; // The amount of frames each has been pressed
	};
	struct Player /* I want to experiment with detaching game state from input/output, 
		so no data like a Sprite will be here, just trivial state */
	{
		constexpr static std::string_view entityName{"PlayerBowlingShip"};
		const size_t entityID;
		const size_t spatialMaximum;
		const size_t hardpointOffset;
		size_t relativePosition1D;
		int8_t tilt;
	};
	inline Player makePlayer(
		GameMode gameMode, 
		const json& entities, 
		const size_t entityID, 
		const size_t spatialMaximum, 
		size_t initialRelativePosition1D = 0,
		int8_t initialTilt = 0
	) /* Seperated this out so we can still have aggigate 
		initialization and simple data struture*/
	{
		const auto dimensionalPropertiesTag = std::format("{} Properties", to_string(gameMode));
		const auto& dimensionalProperties = entities[Player::entityName][dimensionalPropertiesTag];
		return Player{
			entityID,
			spatialMaximum,
			dimensionalProperties["hardpointOffset"],
			initialRelativePosition1D,
			initialTilt
		};
	}
	void state2D(SpatialProperties2D& spatialProperties2D, const Player& player)
	{
		spatialProperties2D.rotation = 0.f;
		spatialProperties2D.scale = {1.f, 1.f};
		spatialProperties2D.position.x = static_cast<float>(player.relativePosition1D) 
				/ static_cast<float>(player.spatialMaximum);
	}
	void draw2D(Renderer2D& renderer, const Player& player)
	{

	}
}
#endif // BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD

