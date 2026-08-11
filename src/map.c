#include "map.h"

u64 fib_index(u64 key)
{
    return (key * 0x9E3779B97F4A7C15ULL) >> (64 - MAP_N);
}

InsertResult map_insert(Map *map, u64 key, void *value)
{
    u64 idx = fib_index(key);
    u64 counter = 0;

    bool use_deleted = false;
    u64 first_deleted = 0;
    while (counter < MAP_SIZE && map->data[idx].state != EMPTY)
    {
        if (map->data[idx].state == OCCUPIED)
        {
            if (map->data[idx].key == key)
            {
                use_deleted = false;
                break;
            }
        }
        else
        {
            if (!use_deleted)
            {
                use_deleted = true;
                first_deleted = idx;
            }
        }
        idx = (idx + 1) % MAP_SIZE;
        counter++;
    }
    if (use_deleted)
    {
        idx = first_deleted;
        counter = 0;
    }
    if (counter == MAP_SIZE)
    {
        return FULL;
    }
    map->data[idx].key = key;
    map->data[idx].value = value;
    map->data[idx].state = OCCUPIED;
    return SUCCESS;
}

void* map_lookup(Map* map, u64 key)
{
    u64 idx = fib_index(key);
    u64 counter = 0;
    while (counter < MAP_SIZE && map->data[idx].state != EMPTY)
    {
        if (map->data[idx].state == OCCUPIED && map->data[idx].key == key)
        {
            return map->data[idx].value;
        }
        idx = (idx + 1) % MAP_SIZE;
        counter++;
    }
    return NULL;
}

void map_delete(Map* map, u64 key)
{
    u64 idx = fib_index(key);
    u64 counter = 0;
    while (counter < MAP_SIZE && map->data[idx].state != EMPTY)
    {
        if (map->data[idx].state == OCCUPIED && map->data[idx].key == key)
        {
            map->data[idx].state = DELETED;
            map->data[idx].key = 0;
            map->data[idx].value = NULL;
            break;
        }
        idx = (idx + 1) % MAP_SIZE;
        counter++;
    }
}
