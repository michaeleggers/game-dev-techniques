#include "e_player.h"

#include "../entity.h"

constexpr f32 PLAYER_SPEED = 2.3f;

Entity* spawn_player(const Vector2& pos)
{
    EntityManager* em     = EntityManager::instance();
    Entity*        player = em->create_entity(PLAYER);
    player->m_position    = pos;
    player->m_speed       = PLAYER_SPEED;
    player->m_color       = RED;

    return player;
}

void think_player(Entity* self)
{
    // Also let the player *think* (react to hits).
    if ( self )
    {
        if ( self->m_health <= 0.0f )
        {
            EntityManager::instance()->kill_entity(self);
        }
    }
}
