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
  f32 start_time = 0.0f;
  f32 end_time = GetTime();
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    f32 frame_time_ms = (end_time - start_time) * 1000.0f;
    start_time = GetTime();
    game.update_and_render(frame_time_ms);
    end_time = GetTime();
  }

  // Cleanup
  game.shutdown();
  CloseWindow(); // Close window and OpenGL context

  return 0;
}
