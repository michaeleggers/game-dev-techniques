#include <assert.h>
#include <stdio.h>

#include <raylib.h>

#include "common.h"
#include "entity.h"
#include "raymath.h"

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
  EntityManager *em = EntityManager::instance();

  Entity *player = em->create_entity(PLAYER);
  Entity *enemy = em->create_entity(
      ENEMY, Vector2{(f32)GetRandomValue(0, 960), (f32)GetRandomValue(0, 540)});
  Entity *enemy2 = em->create_entity(
      ENEMY, Vector2{(f32)GetRandomValue(0, 960), (f32)GetRandomValue(0, 540)});
  em->kill_entity(enemy2);
  Entity *enemy3 = em->create_entity(
      ENEMY, Vector2{(f32)GetRandomValue(0, 960), (f32)GetRandomValue(0, 540)});

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Logic: If player gets close to enemy, the enemy starts to
    //        follow the player.
    if (Vector2Distance(player->m_position, enemy->m_position) <= 60.0f) {
      enemy->follow(player);
    }
    if (Vector2Distance(player->m_position, enemy2->m_position) <= 60.0f) {
      enemy2->follow(player);
    }
    if (Vector2Distance(player->m_position, enemy3->m_position) <= 60.0f) {
      enemy3->follow(player);
    }

    // Update entities.
    player->update();
    enemy->update();
    enemy2->update();
    enemy3->update();

    // Rendition
    BeginDrawing();
    {
      DrawCircleV(player->m_position, 10.0f, RED);
      DrawCircleV(enemy->m_position, 10.0f, YELLOW);
      DrawCircleV(enemy2->m_position, 10.0f, YELLOW);
      DrawCircleV(enemy3->m_position, 10.0f, YELLOW);
    }
    ClearBackground((Color){90, 100, 255, 0xFF});

    EndDrawing();
  }

  // Cleanup
  CloseWindow(); // Close window and OpenGL context

  return 0;
}
