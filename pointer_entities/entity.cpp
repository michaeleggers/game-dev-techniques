#include "entity.h"

#include <assert.h>
#include <raylib.h>
#include <stdio.h>

Entity::Entity(EntityType type)
    : m_type(type), m_position(Vector2{0.0f, 0.0f}),
      m_velocity(Vector2{0.0f, 0.0f}), m_target(nullptr) {}

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
    m_speed = 0.0f;
    m_velocity = Vector2{0.0f, 0.0f};
  }
}

void Entity::update() {
  switch (m_type) {

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
    m_velocity = Vector2Scale(m_velocity, m_speed);

    // Logic
    m_position = Vector2Add(m_position, m_velocity);
  } break;

  case ENEMY: {
    if (m_target) {
      Vector2 dir = Vector2Subtract(m_target->m_position, m_position);
      dir = Vector2Normalize(dir);
      m_velocity = dir;
    }
    m_velocity = Vector2Scale(m_velocity, m_speed);
    m_position = Vector2Add(m_position, m_velocity);
  } break;

  default: {
    assert(false && "Unknown entity type!");
  }
  }
}

EntityManager *EntityManager::instance() {
  static EntityManager instance{};
  return &instance;
}

Entity *EntityManager::create_entity(EntityType type, const Vector2 &position,
                                     const Vector2 &velocity) {
  u32 free = 0;
  for (; free < MAX_ENTITIES; free++) {
    if (!m_alive[free]) {
      m_alive[free] = true;
      break;
    }
  }

  if (free >= MAX_ENTITIES) {
    assert(false && "Entity Manager: no more space left for more entities.");
    return nullptr;
  }

  Entity *entity = &m_entities[free];
  *entity = Entity(type, position, velocity);

  printf("[Entity Manager]: Created new entity at index: %d\n", free);

  return entity;
}

void EntityManager::kill_entity(Entity *entity) {
  u32 slot_number = (u32)(entity - &m_entities[0]);
  printf("[Entity Manager]: Killing entity at index: %d\n", slot_number);
  m_alive[slot_number] = false;
}

u32 EntityManager::capacity() const { return MAX_ENTITIES; }

bool EntityManager::is_alive(u32 slot) const {
  assert(slot < MAX_ENTITIES);
  return m_alive[slot];
}

Entity *EntityManager::at(u32 slot) {
  assert(slot < MAX_ENTITIES);
  return &m_entities[slot];
}
