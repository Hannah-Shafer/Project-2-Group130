cmake_minimum_required(VERSION 3.30)
project(Group_130)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(Group_130
        main.cpp
        trie.cpp
)

target_link_libraries(Group_130
        PRIVATE
        SFML::System
        SFML::Window
        SFML::Graphics
)