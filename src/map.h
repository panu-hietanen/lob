#ifndef MAP_H_
#define MAP_H_

#include "config.h"
#include "types.h"

typedef enum SlotState {
    EMPTY,
    OCCUPIED,
    DELETED
} SlotState;

typedef enum InsertResult {
    SUCCESS,
    FULL
} InsertResult;

typedef struct MapData {
    SlotState state;
    u64 key;
    void* value;
} MapData;

typedef struct Map {
    MapData data[MAP_SIZE];
} Map;

u64 fib_index(u64 key);

void map_init(Map* map);

InsertResult map_insert(Map* map, u64 key, void* value);
void* map_lookup(Map* map, u64 key);
void map_delete(Map* map, u64 key);

#endif // MAP_H_