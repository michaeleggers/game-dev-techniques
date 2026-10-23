#pragma once

#include "../common.h"
#include "../entity.h"

Entity* spawn_pickup(const Vector2& pos);
void think_pickup(Entity* self, Entity* player, f32 dt);
