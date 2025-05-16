#include <StarBowler/Sprite.hpp>
#ifndef BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD
namespace Bored:: GAME_NAME
{
	using EntityID = size_t;
	struct Input {
		size_t up, down, left, right, bowl; // The amount of frames each has been pressed
	};
	struct Player /* I want to experiment with detaching game state from input/output, 
		so no data like a Sprite will be here, just trivial state */
	{
		constexpr inline std::string_view entityName{"PlayerBowlingShip"};
		const size_t entityID;
		const size_t spatialMaximum;
		size_t relativePosition1D;
		int8_t tilt;
	};
	void state2D(SpatialProperties2D& spatialProperties2D, const Player& player) const
	{
		spatialProperties2D.rotation = 0.f
		spatialProperties2D.scale = {1.f, 1.f};
		spatialProperties2D.position = static_cast<float>(relativePosition1D) 
				/ static_cast<float>(spatialMaximum);
	}
	void draw2D(Renderer2D& renderer, const Player& player)
	{

	}
}
#endif // BORED_GAMES_STAR_BOWLER_PLAYER_HEADER_HPP_INCLUDE_GUARD

