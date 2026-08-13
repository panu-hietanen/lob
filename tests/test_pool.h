#ifndef TEST_POOL_H_
#define TEST_POOL_H_

#include "../src/config.h"
#include "../src/pool.h"

bool pool_alloc_full();
bool pool_alloc_after_free();
bool pool_free_zeroes_fields();


#endif // TEST_POOL_H_