#pragma once

#include "../entity.h"

class Spawner
{
  public:
    struct SpawnCommand
    {
        EntityType type        = NULL_ENTITY;
        Vector2    position    = Vector2{ 0.0f, 0.0f };
        u32        target_slot = MAX_ENTITIES; // Init with illegal slot.
        int        max_pos_x   = 0;
        int        max_pos_y   = 0;
    };

    static Spawner* instance();

    void send_spawn_command(const SpawnCommand& command);
    void flush_spawns();

  private:
    Spawner() = default;
    SpawnCommand m_spawn_commands[ MAX_SPAWNS_PER_FRAME ];
    u32          m_spawn_count = 0;
};
