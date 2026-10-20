#include <raylib.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "common.h"
#include "entity.h"
#include "game.h"
#include "raymath.h"
#include "window.h"

#define MAX_ENEMIES 3
#define PLAYER_SPEED 1.5f
#define ENEMY_MIN_SPEED 0.5f
#define ENEMY_MAX_SPEED 2.0f
#define TURRET_FIRE_SPEED_MS 1000.0f
#define TURRET_SIZE 10.0f
#define PROJECTILE_SPEED 1.0f
#define RUNNING_SHOES_PICKUP_SPAWN_MS 2000.0f
#define SHOES_TIME_TO_LIVE 3000.0f
#define MARGIN 10.0f

static f32 rand_between(f32 min, f32 max) {
    f32 r = (f32)rand() / (f32)RAND_MAX;
    f32 range = max - min;
    return r * range + min;
}

static bool collision(const Entity* a, const Entity* b) {
    f32 radius2 = (a->m_scale + b->m_scale);
    radius2 *= radius2;
    return Vector2DistanceSqr(a->m_position, b->m_position) <= radius2;
}

void Game::init(Window window) {

    EntityManager* em = EntityManager::instance();

    m_font = LoadFont("./assts/fonts/Inter-Bold.ttf");

    m_player = em->create_entity(PLAYER);
    m_player->m_position = Vector2{(f32)window.width / 2.0f, (f32)window.height / 2.0f};
    m_player->m_speed = PLAYER_SPEED;
    m_player->m_color = RED;

    for (u32 i = 0; i < MAX_ENEMIES; i++) {
        Entity* enemy = em->create_entity(
            ENEMY, Vector2{(f32)GetRandomValue(0, window.width), (f32)GetRandomValue(0, window.height)});
        enemy->m_speed = rand_between(ENEMY_MIN_SPEED, ENEMY_MAX_SPEED);
        enemy->m_color = YELLOW;
    }

    m_turrets[0] = em->create_entity(TURRET, Vector2{TURRET_SIZE + MARGIN, TURRET_SIZE + MARGIN});
    m_turrets[1] = em->create_entity(TURRET, Vector2{(f32)window.width - TURRET_SIZE - MARGIN, TURRET_SIZE + MARGIN});
    m_turrets[2] = em->create_entity(TURRET, Vector2{TURRET_SIZE + MARGIN, (f32)window.height - TURRET_SIZE - MARGIN});
    m_turrets[3] = em->create_entity(
        TURRET, Vector2{(f32)window.width - TURRET_SIZE - MARGIN, (f32)window.height - TURRET_SIZE - MARGIN});
    m_turrets[0]->m_color = BLACK;
    m_turrets[1]->m_color = BLACK;
    m_turrets[2]->m_color = BLACK;
    m_turrets[3]->m_color = BLACK;
    m_turrets[0]->m_scale = TURRET_SIZE;
    m_turrets[1]->m_scale = TURRET_SIZE;
    m_turrets[2]->m_scale = TURRET_SIZE;
    m_turrets[3]->m_scale = TURRET_SIZE;
    m_turrets[0]->m_cooldown = TURRET_FIRE_SPEED_MS;
    m_turrets[1]->m_cooldown = TURRET_FIRE_SPEED_MS;
    m_turrets[2]->m_cooldown = TURRET_FIRE_SPEED_MS;
    m_turrets[3]->m_cooldown = TURRET_FIRE_SPEED_MS;
}

void Game::update_and_render(Window window, f32 dt) {

    // Logic: If player gets close to enemy, the enemy starts to
    //        follow the player.
    EntityManager* em = EntityManager::instance();

    // Press TAB to draw debug info.
    if (IsKeyPressed(KEY_TAB)) {
        m_draw_debug = !m_draw_debug;
    }

    // 1st pass: *think*.
    for (u32 i = 0; i < em->capacity(); i++) {
        // Not interested in dead entities.
        if (!em->is_alive(i)) {
            continue;
        }

        Entity* entity = em->at(i);

        // We want the enemies.
        if (entity->m_type == ENEMY) {
            if (m_player) { // if a player exists, start to hunt it if close enough.
                if (Vector2Distance(m_player->m_position, entity->m_position) <= 60.0f) {
                    entity->follow(m_player);
                }
            }
        }

        else if (entity->m_type == TURRET) {
            if (entity->m_accumulator >= entity->m_cooldown) {
                if (m_player) {
                    // printf("[TURRET] with id:%d is firing.\n", em->slot(entity));
                    Entity* projectile = em->create_entity(PROJECTILE, entity->m_position);
                    projectile->m_target = m_player;
                    projectile->m_speed = PROJECTILE_SPEED;
                    projectile->m_scale = 5.0f;
                    projectile->m_color = PINK;
                    projectile->m_direction = Vector2Subtract(projectile->m_target->m_position, entity->m_position);
                    projectile->m_direction = Vector2Normalize(projectile->m_direction);
                }
                entity->m_accumulator = 0.0f;
            }
        }

        else if (entity->m_type == PROJECTILE) {

            // Kill projectile if out of bounds.
            if (entity->m_position.x >= 1000 || entity->m_position.x < -10.0f || entity->m_position.y >= 1000 ||
                entity->m_position.y < -10) {
                entity->set_kill_flag();
            } else {
                // Check if it hit other thing (the Player).
                if (entity->m_target) {
                    if (collision(entity, entity->m_target)) {
                        // Hit!

                        // Lower health of target.
                        entity->m_target->m_health -= 10.0f;

                        // Remove this projectile
                        entity->set_kill_flag();
                    }
                }
            }
        }

        else if (entity->m_type == PICKUP_RUNNING_SHOES) {
            // TODO: collide with player and speed them up.

            // If no-one picked it up after some time, despawn.
            entity->m_time_to_live_ms -= dt;
            if (entity->m_time_to_live_ms <= 0.0f) {
                entity->set_kill_flag();
            } else {
                if (m_player) {
                    if (collision(entity, m_player)) {
                        m_player->m_speed *= entity->m_speed_mult;
                        entity->set_kill_flag();
                    }
                }
            }
        }
    }

    // Check if it is time to spawn new pickup items
    m_running_shoes_accum += dt;
    if (m_running_shoes_accum >= RUNNING_SHOES_PICKUP_SPAWN_MS) {
        Entity* pickup = em->create_entity(
            PICKUP_RUNNING_SHOES, Vector2{(f32)GetRandomValue(0, window.width), (f32)GetRandomValue(0, window.height)});
        pickup->m_color = GREEN;
        pickup->m_time_to_live_ms = SHOES_TIME_TO_LIVE;
        m_running_shoes_accum = 0.0f;
    }

    // Also let the player *think* (react to hits).
    if (m_player) {
        if (m_player->m_health <= 0.0f) {
            m_player->set_kill_flag();
        }
    }

    // 2nd pass: update positions.
    // Check if player should be killed here because
    // we need to assing nullptr.
    if (m_player) {
        if (m_player->is_kill_flag_set()) {
            em->kill_entity(m_player);
            m_player = nullptr;
        }
    }
    for (u32 i = 0; i < em->capacity(); i++) {
        if (!em->is_alive(i)) {
            continue;
        }

        Entity* entity = em->at(i);

        if (entity->is_kill_flag_set()) {
            em->kill_entity(entity);
            continue;
        }

        switch (entity->m_type) {

            case PLAYER: {
                // Reset direction to 0 so that if no key
                // is being pressed the player stands still.
                entity->m_direction = Vector2{0.0f, 0.0f};

                if (IsKeyDown(KEY_A)) {
                    entity->m_direction.x = -1;
                }
                if (IsKeyDown(KEY_D)) {
                    entity->m_direction.x = 1;
                }
                if (IsKeyDown(KEY_W)) {
                    entity->m_direction.y = -1;
                }
                if (IsKeyDown(KEY_S)) {
                    entity->m_direction.y = 1;
                }
                entity->m_direction = Vector2Normalize(entity->m_direction);
                entity->m_direction = Vector2Scale(entity->m_direction, entity->m_speed);

                // Logic
                Vector2 velocity = Vector2Scale(entity->m_direction, entity->m_speed);
                entity->m_position = Vector2Add(entity->m_position, velocity);
            } break;

            case ENEMY: {
                if (entity->m_target) {
                    // printf("[ENEMY]: own slot: %d, target slot: %d\n", em->slot(entity), em->slot(entity->m_target));
                    Vector2 dir = Vector2Subtract(entity->m_target->m_position, entity->m_position);
                    dir = Vector2Normalize(dir);
                    entity->m_direction = dir;
                }
                Vector2 velocity = Vector2Scale(entity->m_direction, entity->m_speed);
                entity->m_position = Vector2Add(entity->m_position, velocity);
            } break;

            case TURRET: {
                entity->m_accumulator += dt;
            } break;

            case PROJECTILE: {
                // Move projectile.
                Vector2 velocity = Vector2Scale(entity->m_direction, entity->m_speed);
                entity->m_position = Vector2Add(entity->m_position, velocity);

            } break;

            case PICKUP_RUNNING_SHOES: {
            } break;

            default: {
                assert(false && "Unknown entity type!");
            }
        }
    }

    // Rendition
    BeginDrawing();
    ClearBackground((Color){90, 100, 255, 0xFF});
    {
        for (u32 i = 0; i < em->capacity(); i++) {
            if (!em->is_alive(i)) {
                continue;
            }

            Entity* entity = em->at(i);

            // Draw the entity
            DrawCircleV(entity->m_position, entity->m_scale, entity->m_color);

            if (m_draw_debug) {

                // Draw entity's slot-number next to it
                u32 slot_number = em->slot(entity);
                char text_buffer[32] = {};
                snprintf(text_buffer, 32, "%d", slot_number);
                DrawTextEx(m_font, text_buffer, entity->m_position, 24.0f, 1.0f, WHITE);

                // Draw line from entity to its target if it has one.
                if (entity->m_target) {
                    // Default target line properties.
                    Color line_color = RAYWHITE;
                    f32 line_width = 1.0f;
                    // Give enemies a dedicated target line.
                    if (entity->m_type == ENEMY) {
                        line_color = RED;
                        line_width = 3.0f;
                    }
                    DrawLineEx(entity->m_position, entity->m_target->m_position, line_width, line_color);
                }
            }
        }

        // Draw player overlays

        // Player health
        if (m_player) {
            f32 health_bar_width = m_player->m_scale * 5.0f;
            health_bar_width *= m_player->m_health / 100.0f;
            DrawRectangleV(Vector2{m_player->m_position.x - health_bar_width / 2.0f, m_player->m_position.y - 25.0f},
                           Vector2{health_bar_width, 10.0f}, GREEN);
        }
    }

    EndDrawing();
}

void Game::shutdown() {
    // Nothing to clean up at this point.
}
