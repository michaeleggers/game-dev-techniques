#include <raylib.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "../collision.h"
#include "../common.h"
#include "../entity.h"
#include "../utils.h"
#include "../window.h"

#include "game.h"
#include "raymath.h"

#include "e_common.h"
#include "e_enemy.h"
#include "e_pickup.h"
#include "e_player.h"
#include "e_projectile.h"
#include "e_turret.h"

void Game::init(Window window) {

    m_window = window;
    m_font = LoadFont("./assts/fonts/Inter-Bold.ttf");

    // Create player
    m_player = spawn_player(Vector2{window.width / 2.0f - 20.0f, window.height / 2.0f - 20.0f});

    // Spawn enemies within the hole game field.
    for (u32 i = 0; i < MAX_ENEMIES; i++) {
        spawn_enemy(window.width, window.height);
    }

    // Create turrets at each corner and one in the middle.
    spawn_turret(Vector2{TURRET_SIZE + MARGIN, TURRET_SIZE + MARGIN});
    spawn_turret(Vector2{(f32)window.width - TURRET_SIZE - MARGIN, TURRET_SIZE + MARGIN});
    spawn_turret(Vector2{TURRET_SIZE + MARGIN, (f32)window.height - TURRET_SIZE - MARGIN});
    spawn_turret(Vector2{(f32)window.width - TURRET_SIZE - MARGIN, (f32)window.height - TURRET_SIZE - MARGIN});
    spawn_turret(Vector2{(f32)window.width / 2.0f, (f32)window.height / 2.0f});
}

void Game::update_and_render(Window window, f32 dt) {

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
        think(entity, dt);
    }

    // Check if it is time to spawn new pickup items
    m_running_shoes_accum += dt;
    if (m_running_shoes_accum >= RUNNING_SHOES_PICKUP_SPAWN_MS) {
        spawn_pickup(Vector2{(f32)GetRandomValue(0, window.width), (f32)GetRandomValue(0, window.height)});
        m_running_shoes_accum = 0.0f;
    }

    // 2nd pass: update positions.
    // Check if player should be killed explicitly because
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
            // For each killed enemy, spawn a new one.
            if (entity->m_type == ENEMY) {
                spawn_enemy(m_window.width, m_window.height);
            }
            em->kill_entity(entity);
            continue;
        }

        simulate(entity, dt);
    }

    // Rendition
    render();
}

void Game::think(Entity* entity, f32 dt) {

    switch (entity->m_type) {
        case PLAYER: {
            think_player(entity);
        } break;

        case ENEMY: {
            think_enemy(entity, m_player);
        } break;

        case TURRET: {
            think_turret(entity, dt);
        } break;

        case PROJECTILE: {
            think_projectile(entity, dt, m_window);
        } break;

        case PICKUP_RUNNING_SHOES: {
            think_pickup(entity, m_player, dt);
        } break;

        default: {
            assert(false && "Unknown entity type.");
        }
    }
}

void Game::simulate(Entity* entity, f32 dt) {
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

void Game::render() {
    EntityManager* em = EntityManager::instance();

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
                    Color line_color = ORANGE;
                    f32 line_width = 3.0f;
                    // Give enemies a dedicated target line.
                    if (entity->m_type == ENEMY) {
                        line_color = RED;
                        line_width = 3.0f;
                    }
                    DrawLineEx(entity->m_position, entity->m_target->m_position, line_width, line_color);
                    DrawCircleLinesV(entity->m_target->m_position, entity->m_target->m_scale + 3.0f, WHITE);
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
