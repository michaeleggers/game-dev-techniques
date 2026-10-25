#pragma once

#include "../entity.h"
#include "../window.h"

Entity* spawn_projectile(const Vector2& pos, Entity* target);
void think_projectile(Entity* self, f32 dt, const Window& window);
