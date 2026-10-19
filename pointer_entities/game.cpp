#include <raylib.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "common.h"
#include "entity.h"
#include "game.h"
#include "raymath.h"

#define MAX_ENEMIES 4
#define PLAYER_SPEED 2.0f
#define TURRET_FIRE_SPEED_MS 1000.0f

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

  m_turrets[0] = em->create_entity(TURRET, Vector2{10.0f, 10.0f});
  m_turrets[1] = em->create_entity(TURRET, Vector2{900.0f, 10.0f});
  m_turrets[2] = em->create_entity(TURRET, Vector2{10.0f, 500.0f});
  m_turrets[3] = em->create_entity(TURRET, Vector2{900.0f, 500.0f});
  m_turrets[0]->m_color = GREEN;
  m_turrets[1]->m_color = GREEN;
  m_turrets[2]->m_color = GREEN;
  m_turrets[3]->m_color = GREEN;
  m_turrets[0]->m_cooldown = TURRET_FIRE_SPEED_MS;
  m_turrets[1]->m_cooldown = TURRET_FIRE_SPEED_MS;
  m_turrets[2]->m_cooldown = TURRET_FIRE_SPEED_MS;
  m_turrets[3]->m_cooldown = TURRET_FIRE_SPEED_MS;
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
    if (entity->m_type == ENEMY) {
      if (Vector2Distance(m_player->m_position, entity->m_position) <= 60.0f) {
        entity->follow(m_player);
      }
    } else if (entity->m_type == PROJECTILE) {
      // Kill projectile if out of bounds.
      if (entity->m_position.x >= 1000 || entity->m_position.x < -10.0f ||
          entity->m_position.y >= 1000 || entity->m_position.y < -10) {
        em->kill_entity(entity);
      }

      // Check if it hit player
      f32 radius2 = (entity->m_scale + m_player->m_scale);
      radius2 *= radius2;
      if (Vector2DistanceSqr(entity->m_position, m_player->m_position) <=
          radius2) {
        // Hit!
        // Remove this projectile
        em->kill_entity(entity);
        // Lower health of player
        m_player->m_health -= 10.0f;
      }
    }
  }

  // Also let the player *think* (react to hits).
  if (m_player->m_health <= 0.0f) {
    // TODO: Kill player entity.
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

    case TURRET: {
      entity->m_accumulator += dt;
      if (entity->m_accumulator >= entity->m_cooldown) {
        Entity *projectile = em->create_entity(PROJECTILE, entity->m_position);
        projectile->m_target = m_player;
        projectile->m_speed = 1.0f;
        projectile->m_scale = 5.0f;
        projectile->m_color = PINK;
        projectile->m_velocity = Vector2Subtract(
            projectile->m_target->m_position, entity->m_position);
        projectile->m_velocity = Vector2Normalize(projectile->m_velocity);
        entity->m_accumulator = 0.0f;
      }
    } break;

    case PROJECTILE: {
      // Move projectile.
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
      DrawCircleV(entity->m_position, entity->m_scale, entity->m_color);
    }
  }

  EndDrawing();
}

void Game::shutdown() {
  // Nothing to clean up at this point.
}
