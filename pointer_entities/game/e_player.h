#pragma once

#include "../entity.h"

Entity* spawn_player(const Vector2& pos);
void think_player(Entity* self);
