
cmake_minimum_required(VERSION 3.30.2)
project(game-dev-techniques VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json

# --- Common compiler settings for our own code ---
# C++20 without exceptions, RTTI or the STL.
add_library(gamedev_compile_options INTERFACE)
if(MSVC)
    target_compile_options(gamedev_compile_options INTERFACE /W4 /GR- /EHs-c- /utf-8 /permissive-)
    target_compile_definitions(gamedev_compile_options INTERFACE _HAS_EXCEPTIONS=0 _CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
    target_compile_options(gamedev_compile_options INTERFACE
        -Wall -Wextra -Wshadow -Wno-unused-parameter -Wno-missing-field-initializers
        -fno-exceptions -fno-rtti)
endif()

message("### Starting to build ###")

if (CMAKE_BUILD_TYPE STREQUAL "")
    message("### CMAKE_BUILD_TYPE not set. Setting to Debug. ###")
    set(CMAKE_BUILD_TYPE Debug)
endif()


add_subdirectory(./dependencies/raylib/)

add_subdirectory(./pointer_entities/)

