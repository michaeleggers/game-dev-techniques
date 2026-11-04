
#include "game_api.h"

#include "../engine/window.h"
#include "../game/game.h"

static Game g_game;

void g_init(Window window)
{
    g_game.init(window);
}

void g_update_and_render(Window window, f32 dt)
{
    g_game.update_and_render(window, dt);
}

void g_shutdown()
{
    g_game.shutdown();
}

void g_render()
{
    g_game.render();
}

void g_update_and_render(Game& game, Window widnow, f32 dt) {}

GameAPI get_game_api()
{
    GameAPI api{};

    api.init              = g_init;
    api.update_and_render = g_update_and_render;
    api.shutdown          = g_shutdown;
    api.render            = g_render;

    return api;
}
