#include "common.h"

#include <raymath.h>

bool circle_overlap(const Vector2& a, const f32& radius_a, const Vector2& b, const f32& radius_b)
{
    f32 radius2 = (radius_a + radius_b);
    radius2 *= radius2;
    return Vector2DistanceSqr(a, b) <= radius2;
}
