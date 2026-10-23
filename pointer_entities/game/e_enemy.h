#pragma once

#include "../entity.h"

Entity* spawn_enemy(int max_pos_x, int max_pos_y);
void think_enemy(Entity* self, Entity* player);
