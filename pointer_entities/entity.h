#pragma once

#include "common.h"
#include <raylib.h>
#include <raymath.h>

#define ENEMY_SPEED (1.0f)
#define PLAYER_SPEED (2.0f)

enum EntityType { PLAYER, ENEMY, TURRET };

class Entity {
public:
  Entity() = default; // trivial Ctor. Does nothing.
  Entity(EntityType type);
  Entity(EntityType type, const Vector2 &position);
  Entity(EntityType type, const Vector2 &position, const Vector2 &velocity);
  ~Entity() = default;

  void follow(Entity *target);
  void unfollow();
  void update();

  Vector2 m_position;
  Vector2 m_velocity;
  Entity *m_target;
  EntityType m_type;
};

constexpr u32 MAX_ENTITIES = 128;
constexpr u32 MAX_FREE_SLOTS = 32;

class EntityManager {

public:
  static EntityManager *instance();

  Entity *create_entity(EntityType type,
                        const Vector2 &position = Vector2{0.0f, 0.0f},
                        const Vector2 &velocity = Vector2{0.0f, 0.0f});

  void kill_entity(Entity *entity);

private:
  EntityManager() = default; // Private because Singleton class.

  Entity m_entities[MAX_ENTITIES];
  bool m_alive[MAX_ENTITIES];
};
