#pragma once

#include "../entity.h"

Entity* spawn_projectile(const Vector2& pos, Entity* target);
void think_projectile(Entity* self, f32 dt, int window_width, int window_height);
