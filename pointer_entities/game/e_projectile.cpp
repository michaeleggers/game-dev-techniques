#include "e_projectile.h"
#include "e_common.h"

#include "g_collision.h"

constexpr f32 PROJECTILE_SPEED           = 1.0f;
constexpr f32 PROJECTILE_TIME_TO_LIVE_MS = 10000.0f;

Entity* spawn_projectile(const Vector2& pos, Entity* target)
{
    EntityManager* em = EntityManager::instance();

    Entity* projectile            = em->create_entity(PROJECTILE, pos);
    projectile->m_speed           = PROJECTILE_SPEED;
    projectile->m_target          = target;
    projectile->m_scale           = 5.0f;
    projectile->m_color           = PINK;
    projectile->m_time_to_live_ms = PROJECTILE_TIME_TO_LIVE_MS;
    projectile->m_direction       = Vector2Subtract(target->m_position, projectile->m_position);
    projectile->m_direction       = Vector2Normalize(projectile->m_direction);

    return projectile;
}

void think_projectile(Entity* self, f32 dt, const Window& window)
{

    EntityManager* em = EntityManager::instance();

    self->m_time_to_live_ms -= dt;

    bool out_of_bounds = self->m_position.x >= window.width + MARGIN || self->m_position.x < -MARGIN
                         || self->m_position.y >= window.height + MARGIN || self->m_position.y < -MARGIN;
    bool ttl_is_zero = self->m_time_to_live_ms <= 0.0f;

    // Kill projectile if out of bounds or its ttl has expired.
    if ( ttl_is_zero || out_of_bounds )
    {
        em->kill_entity(self);
        return; // Not strictly needed at this point but if we add more
        // below it is a good guardrail to not proceed with anything
        // if ttl or out of bounds is true.
    }
    else
    {
        // Adjust direction
        self->m_direction = Vector2Normalize(Vector2Subtract(self->m_target->m_position, self->m_position));
        // Check if it hit other entity.
        for ( u32 other_slot = 0; other_slot < em->capacity(); other_slot++ )
        {
            if ( !em->is_alive(other_slot) )
            {
                continue;
            }

            Entity* other = em->at(other_slot);
            if ( other->m_type == ENEMY || other->m_type == PLAYER )
            {
                if ( collision(self, other) )
                {
                    // Hit!
                    // Lower health of target.
                    other->m_health -= 10.0f;
                    // Remove this projectile
                    em->kill_entity(self);
                    break; // Break here or bullet affects multiple overlapping entities.
                }
            }
        }
    }
}
