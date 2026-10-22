#include "../entity.h"

void think_enemy(Entity* self, Entity* player) {
    if (self->m_health <= 0.0f) {
        self->set_kill_flag();
    }

    /*
     * This looks like a fix but it isn't.
     * An entity Type is not its identity! (Just like we are all
     * humans but we are all individuals).
     * It just says what *kind* of self, not who exactly.
     * In this case
     * where there is only one player, it would work.
     * But as soon as another player is being
     * spawned this could already go bad.
     *
    if (self->m_target) {
        if (self->m_target->m_type != PLAYER) {
            self->m_target = nullptr;
            self->m_speed = 0.0f;
        }
    }
    */

    if (player) { // if a player exists, start to hunt it if close enough.
        if (Vector2Distance(player->m_position, self->m_position) <= 60.0f) {
            if (player->m_type == PLAYER) {
                self->follow(player);
            }
        }
    }
}
