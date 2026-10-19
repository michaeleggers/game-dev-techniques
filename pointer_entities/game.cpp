#include <raylib.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "common.h"
#include "entity.h"
#include "game.h"

#define PLAYER_SPEED 2.0f

static f32 rand_between(f32 min, f32 max) {
  f32 r = (f32)rand() / (f32)RAND_MAX;
  f32 range = max - min;
  return r * range + min;
}

void Game::init() {
  EntityManager *em = EntityManager::instance();

  m_player = em->create_entity(PLAYER);
  m_player->m_speed = PLAYER_SPEED;
  m_player->m_color = RED;

  for (u32 i = 0; i < MAX_ENEMIES; i++) {
    Entity *enemy =
        em->create_entity(ENEMY, Vector2{(f32)GetRandomValue(0, 960),
                                         (f32)GetRandomValue(0, 540)});
    enemy->m_speed = rand_between(0.8f, 1.8f);
    enemy->m_color = YELLOW;
  }
}

void Game::update_and_render(f32 dt) {

  // Logic: If player gets close to enemy, the enemy starts to
  //        follow the player.
  EntityManager *em = EntityManager::instance();

  // 1st pass: *think*.
  for (u32 i = 0; i < em->capacity(); i++) {
    // Not interested in dead entities.
    if (!em->is_alive(i)) {
      continue;
    }

    Entity *entity = em->at(i);

    // We want the enemies.
    if (entity->m_type != ENEMY) {
      continue;
    }

    if (Vector2Distance(m_player->m_position, entity->m_position) <= 60.0f) {
      entity->follow(m_player);
    }
  }

  // 2nd pass: update positions.
  for (u32 i = 0; i < em->capacity(); i++) {
    if (!em->is_alive(i)) {
      continue;
    }

    Entity *entity = em->at(i);
    switch (entity->m_type) {

    case PLAYER: {
      // Reset velocity to 0 so that if no key
      // is being pressed the player stands still.
      entity->m_velocity = Vector2{0.0f, 0.0f};

      if (IsKeyDown(KEY_A)) {
        entity->m_velocity.x = -1;
      }
      if (IsKeyDown(KEY_D)) {
        entity->m_velocity.x = 1;
      }
      if (IsKeyDown(KEY_W)) {
        entity->m_velocity.y = -1;
      }
      if (IsKeyDown(KEY_S)) {
        entity->m_velocity.y = 1;
      }
      entity->m_velocity = Vector2Normalize(entity->m_velocity);
      entity->m_velocity = Vector2Scale(entity->m_velocity, entity->m_speed);

      // Logic
      entity->m_position = Vector2Add(entity->m_position, entity->m_velocity);
    } break;

    case ENEMY: {
      if (entity->m_target) {
        Vector2 dir =
            Vector2Subtract(entity->m_target->m_position, entity->m_position);
        dir = Vector2Normalize(dir);
        entity->m_velocity = dir;
      }
      entity->m_velocity = Vector2Scale(entity->m_velocity, entity->m_speed);
      entity->m_position = Vector2Add(entity->m_position, entity->m_velocity);
    } break;

    default: {
      assert(false && "Unknown entity type!");
    }
    }
  }

  // Rendition
  BeginDrawing();
  ClearBackground((Color){90, 100, 255, 0xFF});
  {
    for (u32 i = 0; i < em->capacity(); i++) {
      if (!em->is_alive(i)) {
        continue;
      }

      Entity *entity = em->at(i);
      DrawCircleV(entity->m_position, 10.0f, entity->m_color);
    }
  }

  EndDrawing();
}

void Game::shutdown() {
  // Nothing to clean up at this point.
}
