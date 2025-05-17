project(LibBored)
remove_definitions(GAME_NAME)
add_compile_definitions(GAME_NAME=${PROJECT_NAME}_test)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable (${PROJECT_NAME}_test ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}_test PRIVATE 
	${CMAKE_SOURCE_DIR}/include 
	${catch2_SOURCE_DIR}/src
	${raylib_SOURCE_DIR}/src
	${raylib3d_SOURCE_DIR}
	${cpplocate_SOURCE_DIR}/source/cpplocate/include
	${json_SOURCE_DIR}/include
	${spdlog_SOURCE_DIR}/include
)
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME}_test PRIVATE raylib_static spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> Catch2WithMain)
file(GLOB CPPLOCATE ${cpplocate_BINARY_DIR}/cpplocate*)
file(COPY ${CPPLOCATE}  DESTINATION ${PROJECT_BINARY_DIR})
message("Copied cpplocate shared files to ${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}_test PUBLIC cpplocate::cpplocate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

