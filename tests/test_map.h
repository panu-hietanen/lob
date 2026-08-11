#ifndef TEST_MAP_H_
#define TEST_MAP_H_

#include "../src/config.h"
#include "../src/map.h"

void reset_map(Map* map);

bool keys_unique(Map* map);
u64 key_find_colliding(u64 key);

bool key_find_existing(Map* map);
bool key_insert_colliding(Map* map);
bool key_overwrite_existing(Map* map);
bool key_delete_exists(Map* map);
bool key_delete_nonexistent(Map* map);

bool table_fill(Map* map);
bool table_insert_tombstone_only(Map* map);


#endif // TEST_MAP_H_