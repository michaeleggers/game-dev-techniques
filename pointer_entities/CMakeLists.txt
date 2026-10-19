
cmake_minimum_required(VERSION 3.30.2)
project(pointer_entities VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_executable(pointer_entities
    main.cpp
    entity.cpp
    game.cpp
)

target_include_directories(pointer_entities PUBLIC
    ../dependencies/raylib/src/
)

target_link_libraries(pointer_entities
    PRIVATE raylib gamedev_compile_options
)
