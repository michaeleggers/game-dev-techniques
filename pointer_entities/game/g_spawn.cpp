#include "g_spawn.h"

#include "e_enemy.h"
#include "e_pickup.h"
#include "e_player.h"
#include "e_projectile.h"
#include "e_turret.h"

#include <assert.h>

Spawner* Spawner::instance() {
    static Spawner spawner{};
    return &spawner;
}

void Spawner::send_spawn_command(const SpawnCommand& command) {
    assert(m_spawn_count < MAX_SPAWNS_PER_FRAME);
    m_spawn_commands[m_spawn_count++] = command;
}

void Spawner::flush_spawns() {
    for (u32 i = 0; i < m_spawn_count; i++) {
        const SpawnCommand& spawn_cmd = m_spawn_commands[i];
        switch (spawn_cmd.type) {
            case PLAYER: {
                spawn_player(spawn_cmd.position);
            } break;
            case ENEMY: {
                spawn_enemy(spawn_cmd.max_pos_x, spawn_cmd.max_pos_y);
            } break;
            case TURRET: {
                spawn_turret(spawn_cmd.position);
            } break;
            case PROJECTILE: {
                spawn_projectile(spawn_cmd.position, spawn_cmd.target);
            } break;
            case PICKUP_RUNNING_SHOES: {
                spawn_pickup(spawn_cmd.position);
            } break;
            case NULL_ENTITY: {
            } break;
            default: {
                assert(false && "Unknown entity type.");
            }
        }
    }

    m_spawn_count = 0;
}
