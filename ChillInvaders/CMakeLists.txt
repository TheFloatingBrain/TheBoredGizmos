project(ChillInvaders)
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_Declare(
    cpplocate
    GIT_REPOSITORY https://github.com/cginternals/cpplocate
    GIT_TAG v2.3.0
)
FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(cpplocate)
add_executable (${PROJECT_NAME} "Main.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${SFML_SOURCE_DIR}/ ${cpplocate_SOURCE_DIR}//source/cpplocate/include)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics cpplocate::cpplocate)

