# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
include(FetchContent)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TheBoredGizmos")

if(${FIND_RESOURCE_METHOD} STREQUAL "SOURCE")
	add_compile_definitions(RESOURCE_DIRECTORY=${PROJECT_SOURCE_DIR})
	add_compile_definitions(RESOURCE_USE_BUNDLE=0)
	add_compile_definitions(RESOURCE_USE_EXECUTABLE=0)
elseif(${FIND_RESOURCE_METHOD} STREQUAL "BUNDLE")
	add_compile_definitions(RESOURCE_USE_BUNDLE=1)
	add_compile_definitions(RESOURCE_USE_EXECUTABLE=0)
elseif(${FIND_RESOURCE_METHOD} STREQUAL "EXECUTABLE")
	add_compile_definitions(RESOURCE_USE_BUNDLE=0)
	add_compile_definitions(RESOURCE_USE_EXECUTABLE=1)
else()
	message(FATAL_ERROR "Must specifiy FIND_RESOURCE_METHOD either as SOURCE, BUNDLE, or EXECUTABLE")
endif()

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.15.0
	GIT_PROGRESS TRUE
)
message("Fetched Spdlog")
FetchContent_Declare(
	raylib
	GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
	GIT_TAG "master"
	GIT_PROGRESS TRUE
)
message("Fetched Raylib")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
message("Fetched Nlohmann JSON")
FetchContent_Declare(
    cpplocate
    GIT_REPOSITORY https://github.com/cginternals/cpplocate
    GIT_TAG v2.3.0
)
message("Fetched cpplocate")
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.8.1
)
message("Fetched catch2")
FetchContent_MakeAvailable(spdlog)
message("spdlog made availible")
FetchContent_MakeAvailable(raylib)
message("Raylib made availible")
FetchContent_MakeAvailable(json)
message("Nlohmann JSON made availible")
FetchContent_MakeAvailable(cpplocate)
message("cpplocate made availible")
FetchContent_MakeAvailable(catch2)
message("catch2 made availible")

# Include sub-projects.
add_subdirectory ("test/LibBored")
add_subdirectory ("source/StarBowler")
