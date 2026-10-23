#include "e_pickup.h"

#include "../collision.h"
#include "../entity.h"

void think_running_shoes(Entity* self, Entity* player, f32 dt) {
    // If no-one picked it up after some time, despawn.
    self->m_time_to_live_ms -= dt;
    if (self->m_time_to_live_ms <= 0.0f) {
        self->set_kill_flag();
    } else {
        if (player) {
            if (collision(self, player)) {
                player->m_speed *= self->m_speed_mult;
                self->set_kill_flag();
            }
        }
    }
}
