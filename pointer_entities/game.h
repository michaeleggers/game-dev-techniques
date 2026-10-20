#pragma once

#include "common.h"
#include "entity.h"

class Game {

public:
    Game() = default;

    void init();
    void update_and_render(f32 dt);
    void shutdown();

private:
    Entity* m_player = nullptr;
    Entity* m_turrets[4] = {nullptr};
    f32 m_running_shoes_accum = 0.0f;
};
