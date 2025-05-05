#include <iostream>
#include <string_view>
#include <filesystem>
#include <cpplocate/cpplocate.h>
#define TO_STRING_MACRO_INTERNAL(TO_STRINGIFY) #TO_STRINGIFY
#define TO_STRING_MACRO(TO_STRINGIFY) TO_STRING_MACRO_INTERNAL(TO_STRINGIFY)
#define GAME_NAME_STRING TO_STRING_MACRO(GAME_NAME)

namespace Bored {
	constexpr inline const auto gameName = std::string_view{GAME_NAME_STRING};

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
	const inline const auto resourceDirectory = findResourceDirectory();
}
