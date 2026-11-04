#pragma once

#include "../engine/common.h"
#include "../engine/window.h"

struct GameAPI
{
    void (*init)(Window window);
    void (*update_and_render)(Window window, f32 dt);
    void (*render)();
    void (*shutdown)();
};

GameAPI get_game_api();
