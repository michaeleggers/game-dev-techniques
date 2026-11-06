#pragma once

#include "common.h"

template <typename T, u32 N> class EntityPool
{
  public:
    T*   create();
    void kill(T* entity);
    void clear_kill_queue();

    bool is_alive(u32 slot) const;
    bool flagged_to_kill(const T* entity) const;
    T*   at(u32 slot);
    u32  slot(const T* entity) const;
    u32  capacity() const
    {
        return N;
    }

  private:
    T    m_entities[ N ]      = {};
    bool m_alive[ N ]         = {};
    bool m_pending_kills[ N ] = {};
    u32  m_kill_queue[ N ]    = {};
    u32  m_kill_count         = 0;
};

template <typename T, u32 N> T* EntityPool<T, N>::create()
{
    T* result = new T();
    return result;
}
