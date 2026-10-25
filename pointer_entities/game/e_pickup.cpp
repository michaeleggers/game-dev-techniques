#include "e_pickup.h"
#include "e_common.h"

#include "../collision.h"
#include "../entity.h"

constexpr f32 RUNNING_SHOES_SPEED_BOOST = 1.05;
constexpr f32 SHOES_TIME_TO_LIVE        = 3000.0f;

Entity* spawn_pickup(const Vector2& pos)
{
    EntityManager* em         = EntityManager::instance();
    Entity*        pickup     = em->create_entity(PICKUP_RUNNING_SHOES, pos);
    pickup->m_color           = GREEN;
    pickup->m_time_to_live_ms = SHOES_TIME_TO_LIVE;
    pickup->m_speed_mult      = RUNNING_SHOES_SPEED_BOOST;

    return pickup;
}

void think_pickup(Entity* self, Entity* player, f32 dt)
{
    EntityManager* em = EntityManager::instance();
    // If no-one picked it up after some time, despawn.
    self->m_time_to_live_ms -= dt;
    if ( self->m_time_to_live_ms <= 0.0f )
    {
        em->kill_entity(self);
    }
    else
    {
        if ( player )
        {
            if ( collision(self, player) )
            {
                player->m_speed *= self->m_speed_mult;
                em->kill_entity(self);
            }
        }
    }
}
