#include <assert.h>
#include <stdio.h>

#include <raylib.h>

#include "entity.h"
#include "raymath.h"

Entity *create_player() {
  Entity *player = new Entity(PLAYER, Vector2{100.0f, 100.0f});
  return player;
}

void kill_entity(Entity *entity) {
  assert(entity);
  delete entity;
}

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

  // Init game data.
  Entity *player = create_player();
  Entity *enemy = new Entity(
      ENEMY,
      // position
      Vector2{(float)GetRandomValue(0, 960), (float)GetRandomValue(0, 540)},
      // velocity
      Vector2{0.0f, 0.0f});

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    if (IsKeyDown(KEY_BACKSPACE)) {
      delete player;
      player = create_player();
    }

    // Logic: If player gets close to enemy, the enemy starts to
    //        follow the player.
    if (Vector2Distance(player->m_position, enemy->m_position) <= 60.0f) {
      enemy->follow(player);
    }

    // Update entities.
    player->update();
    enemy->update();

    // Rendition
    BeginDrawing();
    {
      DrawCircleV(player->m_position, 10.0f, RED);
      DrawCircleV(enemy->m_position, 10.0f, YELLOW);
    }
    ClearBackground((Color){90, 100, 255, 0xFF});

    EndDrawing();
  }

  // Cleanup
  CloseWindow(); // Close window and OpenGL context

  return 0;
}
