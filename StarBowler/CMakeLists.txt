project(StarBowler)
include(FetchContent)
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
FetchContent_MakeAvailable(spdlog)
message("spdlog made availible")
FetchContent_MakeAvailable(raylib)
message("Raylib made availible")
FetchContent_MakeAvailable(json)
message("Nlohmann JSON made availible")
FetchContent_MakeAvailable(cpplocate)
message("cpplocate made availible")
add_compile_definitions(GAME_NAME=${PROJECT_NAME})
add_executable (${PROJECT_NAME} "Main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE 
	${PROJECT_SOURCE_DIR}/include 
	${raylib_SOURCE_DIR}/src
	${raylib3d_SOURCE_DIR}
	${cpplocate_SOURCE_DIR}/source/cpplocate/include
	${json_SOURCE_DIR}/include
	${spdlog_SOURCE_DIR}/include
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib_static spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
file(GLOB CPPLOCATE ${cpplocate_BINARY_DIR}/cpplocate*)
file(COPY ${CPPLOCATE}  DESTINATION ${PROJECT_BINARY_DIR})
message("Copied cpplocate shared files to ${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC cpplocate::cpplocate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

