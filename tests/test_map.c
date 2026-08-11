#include "test_map.h"

void reset_map(Map *map)
{
    for (u64 i = 0; i < MAP_SIZE; ++i)
    {
        map->data[i].state = EMPTY;
        map->data[i].key = 0;
        map->data[i].value = NULL;
    }
}

bool keys_unique(Map *map)
{
    u64 seen[MAP_SIZE];
    u64 seen_size = 0;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (map->data[i].state != OCCUPIED) continue;
        u64 key = map->data[i].key;
        for (int j = 0; j < seen_size; ++j)
        {
            if (seen[j] == key)
            {
                return false;
            }
        }
        seen[seen_size++] = key;
    }
    return true;
}

u64 key_find_colliding(u64 key)
{
    u64 idx = fib_index(key);
    for (u64 i = 0; i < UINT64_MAX; ++i)
    {
        if (i != key && fib_index(i) == idx) return i;
    }
    return 0;
}

bool key_find_existing(Map* map)
{
    reset_map(map);
    int value1 = 42, value2 = 99;
    u64 key1 = 5, key2 = 12;

    if (map_insert(map, key1, &value1) != SUCCESS) return false;
    if (map_insert(map, key2, &value2) != SUCCESS) return false;

    if (map_lookup(map, key1) != &value1) return false;
    if (map_lookup(map, key2) != &value2) return false;

    return keys_unique(map);
}

bool key_insert_colliding(Map* map)
{
    reset_map(map);
    int value1 = 42, value2 = 50;
    u64 key1 = 1, key2 = key_find_colliding(key1);

    if (map_insert(map, key1, &value1) != SUCCESS) return false;
    if (map_insert(map, key2, &value2) != SUCCESS) return false;

    if (map_lookup(map, key1) != &value1) return false;
    if (map_lookup(map, key2) != &value2) return false;

    return keys_unique(map);
}

bool key_overwrite_existing(Map* map)
{
    reset_map(map);
    int value_old = 1, value_new = 2;
    u64 key = 7;

    if (map_insert(map, key, &value_old) != SUCCESS) return false;
    if (map_insert(map, key, &value_new) != SUCCESS) return false;

    if (map_lookup(map, key) != &value_new) return false;

    // make sure overwrite didn't create a second slot for the same key
    int count = 0;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (map->data[i].state == OCCUPIED && map->data[i].key == key) count++;
    }
    if (count != 1) return false;

    return keys_unique(map);
}

bool key_delete_exists(Map* map)
{
    reset_map(map);
    int value = 55;
    u64 key = 3;

    if (map_insert(map, key, &value) != SUCCESS) return false;

    map_delete(map, key);

    if (map_lookup(map, key) != NULL) return false;
    if (!keys_unique(map)) return false;

    // the slot vacated by delete should be reusable, not permanently lost
    if (map_insert(map, key, &value) != SUCCESS) return false;
    if (map_lookup(map, key) != &value) return false;

    return true;
}

bool key_delete_nonexistent(Map* map)
{
    reset_map(map);
    int value = 10;
    u64 present_key = 4;
    u64 missing_key = 999;

    if (map_insert(map, present_key, &value) != SUCCESS) return false;

    // deleting a key that was never inserted must be a safe no-op
    map_delete(map, missing_key);

    if (map_lookup(map, present_key) != &value) return false;
    if (map_lookup(map, missing_key) != NULL) return false;

    return keys_unique(map);
}

bool table_fill(Map* map)
{
    reset_map(map);
    static int values[MAP_SIZE];

    for (u64 i = 0; i < MAP_SIZE; ++i)
    {
        values[i] = (int)i;
        if (map_insert(map, i, &values[i]) != SUCCESS) return false;
    }

    // table is completely full now, a brand new key must be rejected
    int extra = -1;
    if (map_insert(map, MAP_SIZE, &extra) != FULL) return false;

    for (u64 i = 0; i < MAP_SIZE; ++i)
    {
        if (map_lookup(map, i) != &values[i]) return false;
    }

    return keys_unique(map);
}

bool table_insert_tombstone_only(Map* map)
{
    reset_map(map);
    static int values[MAP_SIZE];

    // fill every slot...
    for (u64 i = 0; i < MAP_SIZE; ++i)
    {
        values[i] = (int)i;
        if (map_insert(map, i, &values[i]) != SUCCESS) return false;
    }

    // ...then delete everything, leaving every slot a tombstone (DELETED)
    for (u64 i = 0; i < MAP_SIZE; ++i)
    {
        map_delete(map, i);
    }

    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (map->data[i].state == OCCUPIED) return false;
    }

    // a table full of tombstones must still accept an insert (not report FULL)
    int value = 123;
    u64 new_key = 0;
    if (map_insert(map, new_key, &value) != SUCCESS) return false;
    if (map_lookup(map, new_key) != &value) return false;

    return true;
}

int main()
{
    typedef struct TestCase {
        const char* name;
        bool (*fn)(Map*);
    } TestCase;

    Map map;

    TestCase tests[] = {
        {"key_find_existing",          key_find_existing},
        {"key_insert_colliding",       key_insert_colliding},
        {"key_overwrite_existing",     key_overwrite_existing},
        {"key_delete_exists",          key_delete_exists},
        {"key_delete_nonexistent",     key_delete_nonexistent},
        {"table_fill",                 table_fill},
        {"table_insert_tombstone_only", table_insert_tombstone_only},
    };

    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    for (int i = 0; i < total; ++i)
    {
        bool ok = tests[i].fn(&map);
        if (ok) passed++;
    }

    return passed == total ? 0 : 1;
}