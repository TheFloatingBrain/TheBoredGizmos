#include <StarBowler/Config.hpp>
#ifndef BORED_GAMES_STAR_BOWLER_SPRITE_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_STAR_BOWLER_SPRITE_HEADER_HPP_INCLUDE_GUARD

namespace Bored:: GAME_NAME
{
	struct Vector2u { size_t x, y; };
	struct Sprite
	{
		struct Animation
		{
			Vector2 start = {0.f, 0.f};
			size_t xTiles = 1, yTiles = 1, currentFrame = 0;
			enum class XOrder { LR = 1, RL = -1 } xOrder = XOrder::LR;
			enum class YOrder { TB = 1, BT = -1 } yOrder = YOrder::TB;
			enum class CoordinateOrder { XY, YX } coordinateOrder = CoordinateOrder::XY;
			inline void currentTileAbs(auto& from) const 
			{
				if(coordinateOrder == CoordinateOrder::YX)
					throw NotImplemented("YX Coordinate Order");
				from.x = (currentFrame % xTiles);
				from.y = (currentFrame - from.x) / xTiles;
			}
			inline Vector2u currentTileAbs() const
			{
				Vector2u currentTile;
				currentTileAbs(currentTile);
				return currentTile;
			}
			inline void setSourceRectStartPosition(Rectangle& source) const
			{
				currentTileAbs(source); /* Minor optimization, re-use the rectangle's 
				x and y instead of returning a new vector */
				if(coordinateOrder == CoordinateOrder::XY) {
					source.x = source.width * source.x * static_cast<int8_t>(xOrder) + start.x;
					source.y = source.height * source.y * static_cast<int8_t>(yOrder) + start.y;
				}
			}
			inline Rectangle sourceRectStartPosition() const
			{
				Rectangle source;
				setSourceRectStartPosition(source);
				return source;
			}
		};
		using AnimationsType = std::map<std::string, Animation>;
		size_t width, height;
		AnimationsType animations;
		std::string currentAnimation;
		inline void setSourceRect(Rectangle& source) const
		{
			const auto& animation = animations.at(currentAnimation);
			source.width = width;
			source.height = height;
			animation.setSourceRectStartPosition(source);
		}
		inline Rectangle sourceRect() const
		{
			Rectangle source;
			setSourceRect(source);
			return source;
		}
	};

	inline Result<Rectangle, Error> rectFromJson(const json& from)
	{
		if(from.contains("x") == false)
			return Error::RectAttributeError("x");
		if(from.contains("y") == false)
			return Error::RectAttributeError("y");
		if(from.contains("width") == false)
			return Error::RectAttributeError("width");
		if(from.contains("height") == false)
			return Error::RectAttributeError("height");
		return Rectangle{
			.x = from["x"],
			.y = from["y"],
			.width = from["width"],
			.height = from["height"]
		};
	}

	inline Result<Sprite::Animation, Error> animationFromJson(std::string animationName, const json& from)
	{
		if(from.contains("x") == false)
			return Error::AnimationAttributeError(animationName, "x");
		if(from.contains("y") == false)
			return Error::AnimationAttributeError(animationName, "y");
		if(from.contains("x_tiles") == false)
			return Error::AnimationAttributeError(animationName, "x_tiles");
		if(from.contains("y_tiles") == false)
			return Error::AnimationAttributeError(animationName, "y_tiles");
		Sprite::Animation::XOrder xOrder = Sprite::Animation::XOrder::LR;
		Sprite::Animation::YOrder yOrder = Sprite::Animation::YOrder::TB;
		Sprite::Animation::CoordinateOrder coordinateOrder = Sprite::Animation::CoordinateOrder::XY;
		if(from.contains("x_order") == true)
			xOrder = (from["x_order"] == "RL") ? Sprite::Animation::XOrder::RL : xOrder;
		if(from.contains("y_order") == true)
			yOrder = (from["y_order"] == "BT") ? Sprite::Animation::YOrder::BT : yOrder;
		if(from.contains("coordinate_order") == true)
			coordinateOrder = (from["coordinate_order"] == "YX") ? Sprite::Animation::CoordinateOrder::YX : coordinateOrder;
		return Sprite::Animation{
			.start = {from["x"], from["y"]},
			.xTiles = from["x_tiles"],
			.yTiles = from["y_tiles"],
			.xOrder = xOrder,
			.yOrder = yOrder,
			.coordinateOrder = coordinateOrder
		};
	}

	inline Result<Sprite, Error> spriteFromJson(std::string spriteName, const json& from)
	{
		if(from.contains("width") == false)
			return Error::SpriteAttributeError(spriteName, "width");
		if(from.contains("height") == false)
			return Error::SpriteAttributeError(spriteName, "width");
		if(from.contains("Animations") == false)
			return Error::SpriteAttributeError(spriteName, "Animations");
		if(from["Animations"].size() <= 0)
			return Error::SpriteAttributeError(spriteName, "Animations (Empty)");
		const auto& jsonAnimations = from["Animations"];
		Sprite::AnimationsType animations;
		for(
			auto animationIterator = jsonAnimations.begin();
			animationIterator != jsonAnimations.end();
			++animationIterator
			)
		{
			auto animationName = animationIterator.key();
			auto animationResult = animationFromJson(
				stringDot(spriteName, animationName),
				animationIterator.value()
			);
			if(animationResult == false)
				return animationResult.error;
			animations[animationName] = animationResult.result;
		}
		return Sprite{
			.width = from["width"],
			.height = from["height"],
			.animations = animations,
			.currentAnimation = jsonAnimations.begin().key()
		};
	}

	struct SpriteAtlas
	{
		Texture2D texture;
		json map;
		Result<Sprite, Error> retrieveSprite(std::string_view spriteName)
		{
			if(map.contains(spriteName) == false)
				return Error::SpriteNotFound(spriteName.data());
			return spriteFromJson(spriteName.data(), map[spriteName]);
		}
		~SpriteAtlas() {
			UnloadTexture(texture);
		}
	};

	Result<SpriteAtlas, Error> loadSpriteAtlas(
			const std::filesystem::path directory,
			const std::string name,
			const std::string imageExtension = defaultImageExtension
		)
	{
		auto imagePath = toResourcePath(directory / (name + imageExtension));
		auto mapPath = toResourcePath(directory / (name + ".json"));
		if(std::filesystem::exists(imagePath) == false)
			return Error::FileNotFound(imagePath);
		auto imagePathString = imagePath.string();
		const char* c_strImagePath = imagePathString.c_str();
		Texture atlasTexture = LoadTexture(c_strImagePath);
		if(IsTextureValid(atlasTexture) == false)
			return Error::FileIOError(c_strImagePath);
		auto mapResult = readTextFile(mapPath);
		if(mapResult == false) return mapResult.error;
		auto atlas = SpriteAtlas{
			atlasTexture,
			json::parse(mapResult.result)
		};
		return atlas;
	}

	struct Renderer2D
	{
		constexpr static const Color defaultTint = WHITE;
		SpriteAtlas atlas;
		void draw(const Sprite& sprite, const Vector2& position, const Color& tint = defaultTint
		) const { DrawTextureRec(atlas.texture, sprite.sourceRect(), position, tint); }
		bool draw(
				const std::ranges::range auto& sprites, 
				const std::ranges::range auto& positions, 
				const Color& tint = defaultTint
			) const
		{
			if(sprites.size() != positions.size())
				return false;
			Rectangle sourceRect;
			for(size_t ii = 0; ii < sprites.size(); ++ii) {
				sprites[ii].setSourceRect(sourceRect);
				DrawTextureRec(atlas.texture, sourceRect, positions[ii], tint);
			}
			return 0;
		}
	};
}

#endif // BORED_GAMES_STAR_BOWLER_SPRITE_HEADER_HPP_INCLUDE_GUARD

