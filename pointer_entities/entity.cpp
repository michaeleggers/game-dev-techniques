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

u32 EntityManager::slot(Entity *entity) const {
  u32 index = (u32)(entity - &m_entities[0]);
  return index;
}
