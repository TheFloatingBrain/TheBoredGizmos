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
			const Vector2 start = {0.f, 0.f};
			const size_t xTiles = 1;
			const size_t yTiles = 1;
			size_t currentFrame = 0;
			enum class XOrder { LR = 1, RL = -1 } xOrder = XOrder::LR;
			enum class YOrder { TB = 1, BT = -1 } yOrder = YOrder::TB;
			enum class CoordinateOrder { XY, YX } coordinateOrder = CoordinateOrder::XY;
			inline bool currentTileAbs(auto& from) const noexcept
			{
				[[unlikely]]
				if(currentFrame > (xTiles * yTiles))
				{
					spdlog::critical(
						"(Animation Error) currentFrame ({}) out of bounds ({} * {} = {})",
						currentFrame,
						xTiles,
						yTiles,
						xTiles * yTiles
					);
					return false;
				}
				[[unlikely]]
				if(coordinateOrder == CoordinateOrder::YX) {
					spdlog::critical("(Not Implemented) YX Coordinate Order");
					return false;
				}
				from.x = (currentFrame % xTiles);
				from.y = (currentFrame - from.x) / xTiles;
				return true;
			}
			inline Vector2u currentTileAbs() const noexcept
			{
				Vector2u currentTile;
				currentTileAbs(currentTile);
				return currentTile;
			}
			inline bool setSourceRectStartPosition(Rectangle& source) const noexcept
			{
				const bool success = currentTileAbs(source); /* Minor optimization, re-use the rectangle's 
				x and y instead of returning a new vector */
				[[unlikely]]
				if(success == false) return false;
				if(coordinateOrder == CoordinateOrder::XY)
				{
					source.x = source.width * source.x * static_cast<int8_t>(xOrder) + start.x;
					source.y = source.height * source.y * static_cast<int8_t>(yOrder) + start.y;
					return true;
				}
			}
			inline Rectangle sourceRectStartPosition() const noexcept
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
		inline bool setSourceRect(Rectangle& source) const noexcept
		{
			[[unlikely]] if(animations.contains(currentAnimation) == false) {
				spdlog::critical("Sprite does not contain animation {}", currentAnimation);
				return false;
			}
			const auto& animation = animations.at(currentAnimation);
			source.width = width;
			source.height = height;
			return animation.setSourceRectStartPosition(source);
		}
		inline Rectangle sourceRect() const noexcept
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
			animations.insert(std::pair{animationName, animationResult.result});
		}
		return Sprite{
			.width = from["width"],
			.height = from["height"],
			.animations = animations,
			.currentAnimation = jsonAnimations.begin().key()
		};
	}

	namespace Internal
	{
		template<typename Sprite_PT>
		requires(std::is_trivially_copyable_v<Sprite_PT> == true 
				|| std::is_trivially_copyable_v<typename Sprite_PT::Animation> == true) /* This is about the best 
				we can do at compile time as imperfect as this heuristic is*/
		inline thread_local std::map<std::string_view, Sprite_PT> protoSpriteCache;
	}

	struct SpriteAtlas
	{
		Texture2D texture;
		json map;
		SpriteAtlas(Texture2D texture_, json map_) : texture(texture_), map(map_), owns(true) {}
		SpriteAtlas(SpriteAtlas&& other) : texture(other.texture), map(other.map), owns(true) { other.owns = false; }
		SpriteAtlas(SpriteAtlas& other) = delete;


		Result<Sprite, Error> retrieveSprite(std::string_view spriteName)
		{
			if(Internal::protoSpriteCache<Sprite>.contains(spriteName) == true) {
				Sprite copy = Internal::protoSpriteCache<Sprite>.at(spriteName);
				return copy;
			}
			if(map.contains(spriteName) == false)
				return Error::SpriteNotFound(spriteName.data());
			auto result = spriteFromJson(spriteName.data(), map[spriteName]);
			if(result == false)
				return result.error;
			Sprite sprite = result.result;
			Internal::protoSpriteCache<Sprite>.insert({spriteName, sprite});
			return sprite;
		}
		~SpriteAtlas()
		{
			if(owns == true) {
				UnloadTexture(texture);
				owns = false;
			}
		}
		protected: 
			bool owns;
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

	struct SpatialProperties2D
	{
		Vector2 position;
		float rotation;
		Vector2 scale = {1.f, 1.f};
	};

	struct Renderer2D
	{
		constexpr static const Color defaultTint = WHITE;
		SpriteAtlas atlas;
		Vector2 screenScale = {1.f, 1.f};
		inline void draw(const Sprite& sprite, const Vector2& position, const Color& tint = defaultTint
		) const { DrawTextureRec(atlas.texture, sprite.sourceRect(), position, tint); }
		inline void draw(
			const Sprite& sprite, 
			const SpatialProperties2D& properties, 
			Rectangle& source, 
			const Color& tint = defaultTint
		) const
		{
			sprite.setSourceRect(source);
			Rectangle destination{
				.x = source.x, 
				.y = source.y,
				.width = properties.scale.x * screenScale.x * sprite.width,
				.height = properties.scale.y * screenScale.y * sprite.height
			};
			Vector2 origin = {
				.x = properties.scale.x * screenScale.x * sprite.width / 2.f,
				.y = properties.scale.y * screenScale.y * sprite.height / 2.f
			};
			DrawTexturePro(
				atlas.texture, 
				source, 
				destination, 
				origin, 
				properties.rotation, 
				tint
			);
		}
		inline void draw(
			const Sprite& sprite,
			const SpatialProperties2D& properties,
			const Color& tint = defaultTint
		) const {
			Rectangle source;
			draw(sprite, properties, source, tint);
		}
		inline bool draw(
				const std::ranges::range auto& sprites, 
				const std::ranges::range auto& spatialProperties2D, 
				const Color& tint = defaultTint
			) const
		{
			if(sprites.size() != spatialProperties2D.size())
				return false;
			Rectangle sourceRect;
			for(size_t ii = 0; ii < sprites.size(); ++ii)
				draw(sprites[ii], spatialProperties2D[ii], sourceRect, tint);
			return true;
		}
	};
}

#endif // BORED_GAMES_STAR_BOWLER_SPRITE_HEADER_HPP_INCLUDE_GUARD

