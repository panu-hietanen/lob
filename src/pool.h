#ifndef POOL_H_
#define POOL_H_

#include "config.h"
#include "types.h"

typedef struct Pool {
    Order data[POOL_CAPACITY];
    Order* freeHead;
} Pool;

Pool* pool_init();
Order* pool_alloc(Pool* pool);
void pool_free(Pool* pool, Order* order);
void pool_destroy(Pool* pool);

#endif // POOL_H_