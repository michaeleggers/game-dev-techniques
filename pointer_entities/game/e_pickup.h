#pragma once

#include "../engine/common.h"
#include "../engine/entity.h"

Entity* spawn_pickup(const Vector2& pos);
void    think_pickup(Entity* self, Entity* player, f32 dt);
