#pragma once

#include "common.h"
#include "entity.h"
#include "window.h"
#include <raylib.h>

class Game {

public:
    Game() = default;

    void init(Window window);
    void update_and_render(Window window, f32 dt);
    void shutdown();

private:
    Entity* m_player = nullptr;
    Entity* m_turrets[4] = {nullptr};
    f32 m_running_shoes_accum = 0.0f;
    Font m_font = {};
    bool m_draw_debug = false;
};
