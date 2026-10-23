#include "e_common.h"

#include "../entity.h"
#include "../utils.h"

Entity* spawn_enemy(int max_pos_x, int max_pos_y) {
    EntityManager* em = EntityManager::instance();
    Entity* enemy =
        em->create_entity(ENEMY, Vector2{(f32)GetRandomValue(0, max_pos_x), (f32)GetRandomValue(0, max_pos_y)});
    enemy->m_speed = rand_between(ENEMY_MIN_SPEED, ENEMY_MAX_SPEED);
    enemy->m_color = YELLOW;
    enemy->m_health = 50.0f;

    return enemy;
}

// Logic: If player gets close to enemy, the enemy starts to
//        follow the player.
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
