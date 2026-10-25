#include "entity.h"

#include <assert.h>
#include <raylib.h>
#include <stdio.h>

Entity::Entity(EntityType type, const Vector2& position, const Vector2& direction)
{
    m_type      = type;
    m_position  = position;
    m_direction = direction;
    m_target    = nullptr;
}

void Entity::follow(Entity* target)
{
    m_target = target;
}

void Entity::unfollow()
{
    if ( m_target )
    {
        m_target    = nullptr;
        m_direction = Vector2{ 0.0f, 0.0f };
    }
}

void Entity::set_kill_flag()
{
    m_kill_flag = true;
}

bool Entity::is_kill_flag_set() const
{
    return m_kill_flag;
}

EntityManager* EntityManager::instance()
{
    static EntityManager instance{};
    return &instance;
}

Entity* EntityManager::create_entity(EntityType type, const Vector2& position, const Vector2& direction)
{
    u32 free = 0;
    for ( ; free < MAX_ENTITIES; free++ )
    {
        if ( !m_alive[ free ] )
        {
            m_alive[ free ] = true;
            break;
        }
    }

    if ( free >= MAX_ENTITIES )
    {
        assert(false && "Entity Manager: no more space left for more entities.");
        return nullptr;
    }

    Entity* entity = &m_entities[ free ];
    *entity        = Entity(type, position, direction);

    printf("[Entity Manager]: Created new entity at index: %d\n", free);

    return entity;
}

void EntityManager::kill_entity(Entity* entity)
{
    u32 slot_number = (u32)(entity - &m_entities[ 0 ]);

    assert(slot_number < MAX_ENTITIES);
    // Detect  double-free: If an entity was killed in that slot
    // before its alive flag should be false!
    assert(m_alive[ slot_number ]);

    m_kill_queue[ m_kill_count++ ] = slot_number;
}

void EntityManager::clear_kill_queue()
{
    for ( u32 i = 0; i < m_kill_count; i++ )
    {
        u32 slot = m_kill_queue[ i ];
        printf("[Entity Manager]: Killing entity at index: %d\n", slot);
        m_alive[ slot ] = false;
    }

    m_kill_count = 0;
}

u32 EntityManager::capacity() const
{
    return MAX_ENTITIES;
}

bool EntityManager::is_alive(u32 slot) const
{
    assert(slot < MAX_ENTITIES);
    return m_alive[ slot ];
}

Entity* EntityManager::at(u32 slot)
{
    assert(slot < MAX_ENTITIES);
    return &m_entities[ slot ];
}

u32 EntityManager::slot(const Entity* entity) const
{
    u32 index = (u32)(entity - &m_entities[ 0 ]);
    return index;
}
