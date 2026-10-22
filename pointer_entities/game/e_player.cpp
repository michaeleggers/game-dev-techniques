
#include "e_player.h"

#include "../entity.h"

void think_player(Entity* self) {
    // Also let the player *think* (react to hits).
    if (self) {
        if (self->m_health <= 0.0f) {
            self->set_kill_flag();
        }
    }
}
