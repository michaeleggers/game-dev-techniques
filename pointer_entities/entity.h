#pragma once

#include "common.h"
#include <raylib.h>
#include <raymath.h>

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

  Vector2 m_position;
  Vector2 m_velocity;
  Entity *m_target;
  EntityType m_type;
  f32 m_speed = 0.0f;
  Color m_color = YELLOW;
};

constexpr u32 MAX_ENTITIES = 128;

class EntityManager {

public:
  static EntityManager *instance();

  Entity *create_entity(EntityType type,
                        const Vector2 &position = Vector2{0.0f, 0.0f},
                        const Vector2 &velocity = Vector2{0.0f, 0.0f});

  void kill_entity(Entity *entity);

  u32 capacity() const;
  bool is_alive(u32 slot) const;
  Entity *at(u32 slot);

private:
  EntityManager() = default; // Private because Singleton class.

  Entity m_entities[MAX_ENTITIES];

  // ` = {}`: Not strictly necessary at this point. But
  //          if EntityManager was to become a real
  //          Ctor this array would not default to 0 (=false).
  bool m_alive[MAX_ENTITIES] = {};
};
