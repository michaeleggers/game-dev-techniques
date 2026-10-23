#pragma once

#include "../common.h"
#include "../entity.h"

Entity* spawn_turret(const Vector2& pos);
void think_turret(Entity* self, f32 dt);
