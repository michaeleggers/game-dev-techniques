
#include "common.h"
#include "entity.h"

#include <raylib.h>

bool collision(const Entity* a, const Entity* b) {
    f32 radius2 = (a->m_scale + b->m_scale);
    radius2 *= radius2;
    return Vector2DistanceSqr(a->m_position, b->m_position) <= radius2;
}
