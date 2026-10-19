
cmake_minimum_required(VERSION 3.30.2)
project(game-dev-techniques VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json

message("### Starting to build ###")

if (CMAKE_BUILD_TYPE STREQUAL "")
    message("### CMAKE_BUILD_TYPE not set. Setting to Debug. ###")
    set(CMAKE_BUILD_TYPE Debug)
endif()


add_subdirectory(./dependencies/raylib/)

add_subdirectory(./pointer_entities/)

