#include "entity.h"

#include <assert.h>

#include <raylib.h>

Entity::Entity()
    : m_position(Vector2{0.0f, 0.0f}), m_velocity(Vector2{0.0f, 0.0f}),
      m_target(nullptr), m_type(ENEMY) {}

Entity::Entity(EntityType type, const Vector2 &position)
    : m_type(type), m_position(position), m_velocity(Vector2{0.0f, 0.0f}),
      m_target(nullptr) {}

Entity::Entity(EntityType type, const Vector2 &position,
               const Vector2 &velocity) {
  m_type = type;
  m_position = position;
  m_velocity = velocity;
  m_target = nullptr;
}

void Entity::follow(Entity *target) { m_target = target; }

void Entity::unfollow() {
  if (m_target) {
    m_target = nullptr;
  }
}

void Entity::update() {
  switch (m_type) {

  case ENEMY: {
    if (m_target) {
      Vector2 dir = Vector2Subtract(m_target->m_position, m_position);
      dir = Vector2Normalize(dir);
      m_velocity = dir;
    }
    m_velocity = Vector2Scale(m_velocity, ENEMY_SPEED);
    m_position = Vector2Add(m_position, m_velocity);
  } break;

  case PLAYER: {
    // Reset velocity to 0 so that if no key
    // is being pressed the player stands still.
    m_velocity = Vector2{0.0f, 0.0f};

    if (IsKeyDown(KEY_A)) {
      m_velocity.x = -1;
    }
    if (IsKeyDown(KEY_D)) {
      m_velocity.x = 1;
    }
    if (IsKeyDown(KEY_W)) {
      m_velocity.y = -1;
    }
    if (IsKeyDown(KEY_S)) {
      m_velocity.y = 1;
    }
    m_velocity = Vector2Normalize(m_velocity);
    m_velocity = Vector2Scale(m_velocity, PLAYER_SPEED);

    // Logic
    m_position = Vector2Add(m_position, m_velocity);
  } break;

  default: {
    assert(false && "Unknown entity type!");
  }
  }
}
