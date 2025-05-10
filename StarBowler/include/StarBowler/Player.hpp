#include <StarBowler/Sprite.hpp>
#ifndef BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
namespace Bored:: GAME_NAME
{
	struct Input {
		size_t up, down, left, right, bowl; // The amount of frames each has been pressed
	};
	struct Player
	{
		constexpr inline std::string_view entityName{"PlayerBowlingShip"};
		size_t relativePosition1D;
		int8_t tilt;
		SpatialProperties2D state2D() const {

		}
		protected: 
			SpatialProperties2D spatialProperties2D;
	};
	void draw2D(Renderer2D& renderer, const Player& player)
	{

	}
}
#endif // BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD

