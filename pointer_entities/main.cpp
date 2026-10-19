#include <assert.h>
#include <stdio.h>

#include "game.h"
#include <raylib.h>

int main(int argc, char **argv) {

  // Init System
  const int screenWidth = 960;
  const int screenHeight = 540;
  InitWindow(screenWidth, screenHeight, "Pointer Entities");
  SetTargetFPS(60);
  // ToggleFullscreen();
  int scr_width = GetScreenWidth();
  int scr_height = GetScreenHeight();
  SetWindowSize(scr_width, scr_height);

  // Create Game
  Game game{};
  game.init();

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    game.update_and_render(16.0f); // TODO: Measure dt.
  }

  // Cleanup
  game.shutdown();
  CloseWindow(); // Close window and OpenGL context

  return 0;
}
