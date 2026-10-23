#include "e_projectile.h"

#include "e_common.h"

#include "../collision.h"

void think_projectile(Entity* self, f32 dt, int window_width, int window_height) {

    EntityManager* em = EntityManager::instance();

    // Kill projectile if out of bounds.
    if (self->m_position.x >= window_width + MARGIN || self->m_position.x < -MARGIN ||
        self->m_position.y >= window_height + MARGIN || self->m_position.y < -MARGIN) {
        self->set_kill_flag();
    } else {
        // Projectile has reached its lifetime?
        self->m_time_to_live_ms -= dt;
        if (self->m_time_to_live_ms <= 0.0f) {
            self->set_kill_flag();
        }
        // Adjust direction
        self->m_direction = Vector2Normalize(Vector2Subtract(self->m_target->m_position, self->m_position));
        // Check if it hit other entity.
        for (u32 other_slot = 0; other_slot < em->capacity(); other_slot++) {
            if (!em->is_alive(other_slot)) {
                continue;
            }

            Entity* other = em->at(other_slot);
            if (other->m_type == ENEMY || other->m_type == PLAYER) {
                if (collision(self, other)) {
                    // Hit!
                    // Lower health of target.
                    other->m_health -= 10.0f;
                    // Remove this projectile
                    self->set_kill_flag();
                    break; // Break here or bullet affects multiple overlapping entities.
                }
            }
        }
    }
}
