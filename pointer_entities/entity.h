#pragma once

#include "common.h"
#include <raylib.h>
#include <raymath.h>

enum EntityType { NULL_ENTITY = 0, PLAYER, ENEMY, TURRET, PROJECTILE, PICKUP_RUNNING_SHOES };

class Entity {
public:
    Entity() = default;
    Entity(EntityType type, const Vector2& position = Vector2{0.0f, 0.0f},
           const Vector2& direction = Vector2{0.0f, 0.0f});
    ~Entity() = default;

    void follow(Entity* target);
    void unfollow();
    void set_kill_flag();
    bool is_kill_flag_set() const;

    Vector2 m_position = Vector2{0.0f, 0.0f};
    Vector2 m_direction = Vector2{0.0f, 0.0f};
    Entity* m_target = nullptr;
    EntityType m_type = NULL_ENTITY;
    f32 m_speed = 0.0f;
    Color m_color = YELLOW;
    f32 m_cooldown = 0.0f;
    f32 m_scale = 10.0f;
    f32 m_accumulator = 0.0f;
    f32 m_health = 100.0f;
    f32 m_speed_mult = 1.1f;
    f32 m_time_to_live_ms = 10000.0f;
    bool m_kill_flag = false;
};

constexpr u32 MAX_ENTITIES = 256;
constexpr u32 MAX_SPAWNS_PER_FRAME = 128;

class EntityManager {

    struct SpawnCommand {
        EntityType type;
        Vector2 position;
        u32 target_slot;
    };

public:
    static EntityManager* instance();

    Entity* create_entity(EntityType type, const Vector2& position = Vector2{0.0f, 0.0f},
                          const Vector2& velocity = Vector2{0.0f, 0.0f});
    void send_spawn_command(const SpawnCommand& command);

    void kill_entity(Entity* entity);

    u32 capacity() const;
    bool is_alive(u32 slot) const;
    Entity* at(u32 slot);
    u32 slot(const Entity* entity) const;

private:
    EntityManager() = default; // Private because Singleton class.

    Entity m_entities[MAX_ENTITIES];

    // ` = {}`: Not strictly necessary at this point. But
    //          if EntityManager was to become a real
    //          Ctor this array would not default to 0 (=false).
    bool m_alive[MAX_ENTITIES] = {};

    SpawnCommand m_spawn_commands[MAX_SPAWNS_PER_FRAME];
    u32 m_spawn_count = 0;
};
