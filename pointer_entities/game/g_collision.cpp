#include "g_collision.h"

#include "../engine/collision.h"

bool collision(const Entity* a, const Entity* b)
{
    return circle_overlap(a->m_position, a->m_scale, b->m_position, b->m_scale);
}
