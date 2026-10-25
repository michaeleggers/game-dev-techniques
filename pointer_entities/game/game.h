#pragma once

#include "../common.h"
#include "../entity.h"
#include "../window.h"

#include <raylib.h>

#define NUM_TURRETS 5

class Game {

public:
    Game() = default;

    void init(Window window);
    void update_and_render(Window window, f32 dt);
    void shutdown();

    void think(Entity* entity, f32 dt);
    void simulate(Entity* entity, f32 dt);
    void render();

private:
    Entity* m_player = nullptr;
    f32 m_running_shoes_accum = 0.0f;
    Font m_font = {};
    bool m_draw_debug = false;
    Window m_window = {};
};
