
cmake_minimum_required(VERSION 3.30.2)
project(pointer_entities VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_executable(pointer_entities
# 'Engine' code
    main.cpp
    entity.cpp
    collision.cpp
    utils.cpp

# Game specific code
    ./game/game.cpp
    ./game/e_enemy.cpp
    ./game/e_turret.cpp
    ./game/e_player.cpp
    ./game/e_projectile.cpp
    ./game/e_pickup.cpp
    ./game/g_spawn.cpp
)

target_include_directories(pointer_entities PUBLIC
    ../dependencies/raylib/src/
)

target_link_libraries(pointer_entities
    PRIVATE raylib gamedev_compile_options
)
