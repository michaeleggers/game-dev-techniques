#pragma once

#include "../engine/common.h"
#include "../engine/entity.h"

constexpr f32 TURRET_SIZE = 10.0f;

Entity* spawn_turret(const Vector2& pos);
void    think_turret(Entity* self, f32 dt);
