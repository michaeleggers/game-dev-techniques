
cmake_minimum_required(VERSION 3.30.2)
project(pointer_entities VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_library(engine
# Engine code
    ./engine/entity.cpp
    ./engine/collision.cpp
    ./engine/utils.cpp
    ./interface/game_api.cpp
)

add_executable(engine_runtime
# Engine executable that implements the engine
    ./engine/main.cpp
)

add_library(pointer_entities
# Game specific code
    ./game/game.cpp
    ./game/e_enemy.cpp
    ./game/e_turret.cpp
    ./game/e_player.cpp
    ./game/e_projectile.cpp
    ./game/e_pickup.cpp
    ./game/g_spawn.cpp
    ./game/g_collision.cpp
)

include_directories(pointer_entities PUBLIC
    ../dependencies/raylib/src/
)

# link_libraries(PRIVATE gamedev_compile_options)

target_link_libraries(engine_runtime PRIVATE raylib gamedev_compile_options engine pointer_entities)
