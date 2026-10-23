#include "e_turret.h"

#include "../common.h"
#include "../entity.h"
#include "e_common.h"

Entity* spawn_turret(const Vector2& pos) {
    EntityManager* em = EntityManager::instance();
    Entity* turret = em->create_entity(TURRET, pos);
    turret->m_color = BLACK;
    turret->m_scale = TURRET_SIZE;
    turret->m_cooldown = TURRET_FIRE_SPEED_MS;

    return turret;
}

void think_turret(Entity* self, f32 dt) {

    EntityManager* em = EntityManager::instance();

    self->m_accumulator += dt;
    if (self->m_accumulator >= self->m_cooldown) {
        // Check nearby entities and fire at them
        f32 shortest_distance = TURRET_RANGE * TURRET_RANGE;
        Entity* closest = nullptr;
        for (u32 other_slot = 0; other_slot < em->capacity(); other_slot++) {
            // Don't shoot the dead.
            if (!em->is_alive(other_slot)) {
                continue;
            }

            Entity* other = em->at(other_slot);

            // Not interested in other turrets, pickups or bullets.
            if (other->m_type == TURRET || other->m_type == PICKUP_RUNNING_SHOES || other->m_type == PROJECTILE) {
                continue;
            }

            f32 distance = Vector2DistanceSqr(self->m_position, other->m_position);
            if (distance < shortest_distance) {
                shortest_distance = distance;
                closest = other;
            }
        }

        // FIRE! There is an entity of interest within range.
        if (closest && (shortest_distance < TURRET_RANGE * TURRET_RANGE)) {
            // printf("[TURRET] with id:%d is firing.\n", em->slot(self));
            Entity* projectile = em->create_entity(PROJECTILE, self->m_position);
            projectile->m_speed = PROJECTILE_SPEED;
            projectile->m_target = closest;
            projectile->m_scale = 5.0f;
            projectile->m_color = PINK;
            projectile->m_time_to_live_ms = PROJECTILE_TIME_TO_LIVE_MS;
            projectile->m_direction = Vector2Subtract(closest->m_position, self->m_position);
            projectile->m_direction = Vector2Normalize(projectile->m_direction);
            self->m_accumulator = 0.0f;
        }
    }
}
