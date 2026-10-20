#include <assert.h>
#include <stdio.h>

#include "game.h"
#include "window.h"
#include <raylib.h>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;

int main(int argc, char** argv) {

    // Init System
    Window window;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pointer Entities");
    SetTargetFPS(60);
    // ToggleFullscreen();
    window.width = GetScreenWidth();
    window.height = GetScreenHeight();
    SetWindowSize(window.width, window.height);

    // Create Game
    Game game{};
    game.init(window);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        game.update_and_render(window, GetFrameTime() * 1000.0f);
    }

    // Cleanup
    game.shutdown();
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
