project(StarBowler)
remove_definitions(GAME_NAME)
add_compile_definitions(GAME_NAME=${PROJECT_NAME})
add_executable (${PROJECT_NAME} "Main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_SOURCE_DIR}/include 
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

