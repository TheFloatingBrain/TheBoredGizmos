#include <iostream>
#ifndef BORED_GAMES_STAR_BOWLER_CONFIG_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_STAR_BOWLER_CONFIG_HEADER_HPP_INCLUDE_GUARD
#include <string_view>
#include <filesystem>
#include <format>
#include <fstream>
#include <cpplocate/cpplocate.h>
#define TO_STRING_MACRO_INTERNAL(TO_STRINGIFY) #TO_STRINGIFY
#define TO_STRING_MACRO(TO_STRINGIFY) TO_STRING_MACRO_INTERNAL(TO_STRINGIFY)
#define GAME_NAME_STRING TO_STRING_MACRO(GAME_NAME)

namespace Bored
{
	constexpr inline const auto gameName = std::string_view{GAME_NAME_STRING};

	namespace Detail
	{
		#ifdef RESOURCE_DIRECTORY
		constexpr inline const auto resourceDirectoryString = std::string_view{TO_STRING_MACRO(RESOURCE_DIRECTORY)};
		inline const std::filesystem::path findResourceDirectory() {
			const static thread_local auto path = std::filesystem::path(resourceDirectoryString) / "resource";
			return path;
		}
		#else
		#if defined(RESOURCE_USE_BUNDLE) && RESOURCE_USE_BUNDLE == 1
		inline const std::filesystem::path findResourceDirectory() {
			const static thread_local auto path = std::filesystem::path(cpplocate::getBundlePath()) / "resource";
			return path;
		}
		#else
		#if defined(RESOURCE_USE_EXECUTABLE) && RESOURCE_USE_EXECUTABLE == 1
		inline const std::filesystem::path findResourceDirectory() {
			const static thread_local auto path = std::filesystem::path(cpplocate::getExecutablePath()).get_parent() / "resource";
			return path;
		}
		#endif
		#endif
		#endif
	}
	const inline thread_local auto resourceDirectory = Detail::findResourceDirectory();
	const inline thread_local auto resourceDirectoryName = resourceDirectory.stem();
	#ifndef DEFAULT_IMAGE_FILE_EXTENSION
		thread_local inline const auto defaultImageExtension = std::string{".png"};
	#else
		thread_local inline const auto defaultImageExtension = std::string{DEFAULT_IMAGE_FILE_EXTENSION};
	#endif
	inline std::string stringDot(std::string before, std::string after) {
		static thread_local const auto dot = std::string{"."};
		return before + dot + after;
	}
		
	inline auto toResourcePath(const std::filesystem::path& from)
	{
		if(resourceDirectory.root_path() == from.root_path() 
				&& from.is_absolute() == true)
			return from;
		if(*from.begin() == resourceDirectory.stem())
			return resourceDirectory.parent_path() / from;
		return resourceDirectory / from;
		return from;
	}
	
	struct Error
	{
		enum class ErrorKind
		{
			FileNotFound,
			FileIOError, 
			SpriteNotFound, 
			AnimationNotFound, 
			SpriteAttributeError, 
			AnimationAttributeError, 
			RectAttributeError
		};
		ErrorKind errorKind;
		std::string info;
		static Error FileNotFound(std::filesystem::path path) {
			return Error{.errorKind = Error::ErrorKind::FileNotFound, .info = path.string()};
		}
		static Error FileIOError(std::filesystem::path path) {
			return Error{.errorKind = Error::ErrorKind::FileIOError, .info = path.string()};
		}
		static Error SpriteNotFound(std::string spriteName) {
			return Error{.errorKind = Error::ErrorKind::SpriteNotFound, .info = spriteName};
		}
		static Error AnimationNotFound(std::string animationName) {
			return Error{.errorKind = Error::ErrorKind::AnimationNotFound, .info = animationName};
		}
		static Error SpriteAttributeError(std::string spriteName, std::string attribute) {
			return Error{.errorKind = Error::ErrorKind::SpriteAttributeError, .info = stringDot(spriteName, attribute)};
		}
		static Error AnimationAttributeError(std::string animationName, std::string attribute) {
			return Error{.errorKind = Error::ErrorKind::AnimationAttributeError, .info = stringDot(animationName, attribute)};
		}
		static Error RectAttributeError(std::string attribute) {
			return Error{.errorKind = Error::ErrorKind::RectAttributeError, .info = attribute};
		}
		constexpr std::string_view error_kind_to_string() const
		{
				switch(errorKind)
				{
				case ErrorKind::FileIOError:
					return "FileIOError";
				case ErrorKind::FileNotFound:
					return "FileNotFound";
				case ErrorKind::SpriteNotFound:
					return "SpriteNotFound";
				case ErrorKind::AnimationNotFound:
					return "AnimationNotFound";
				case ErrorKind::SpriteAttributeError:
					return "SpriteAttributeError";
				case ErrorKind::AnimationAttributeError:
					return "AnimationAttributeError";
				case ErrorKind::RectAttributeError:
					return "RectAttributeError";
				default:
					return "UnknownError";
			}
		}
	};
	constexpr inline auto errorFormatString = std::string_view{"({}) {}"};
	inline std::string to_string(
			const Error& error, 
			const std::string_view formatString = errorFormatString
	) { return std::vformat(formatString, std::make_format_args(error.error_kind_to_string(), error.info)); }
	
	template<typename Success_PT, typename Error_PT>
	struct Result
	{
		enum class Status { Success, Error };
		union {
			Success_PT result;
			Error_PT error;
		};
		const Status status;
		Result(Error_PT error_) : error(error_), status(Status::Error) {}
		Result(Success_PT result_) : result(result_), status(Status::Success) {}
		~Result()
		{
			if(status == Status::Success)
				result.~Success_PT();
			else
				error.~Error();
		}
		operator bool() { return status == Status::Success; }
	};
	
	inline Result<std::string, Error> readTextFile(std::filesystem::path filePath)
	{
		if(std::filesystem::exists(filePath) == false)
			return Error::FileNotFound(filePath);
		std::stringstream buffer;
		std::ifstream stream(filePath.c_str());
		while(stream.eof() == false)
		{
			std::string line;
			if(stream.good() == false)
				return Error::FileIOError(filePath);
			std::getline(stream, line);
			buffer << line;
		}
		return buffer.str();
	}

}
#endif // BORED_GAMES_STAR_BOWLER_CONFIG_HEADER_HPP_INCLUDE_GUARD

