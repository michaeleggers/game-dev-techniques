#pragma once

#include <raylib.h>
#include <raymath.h>

#define ENEMY_SPEED (1.5f)
#define PLAYER_SPEED (2.0f)

enum EntityType { PLAYER, ENEMY };

class Entity {
public:
  Entity();
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
