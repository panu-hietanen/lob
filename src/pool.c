#include "pool.h"

Pool* pool_init()
{
    Pool* pool = malloc(sizeof(Pool));
    if (!pool) exit(EXIT_FAILURE);
    pool->freeHead = &pool->data[0];
    for (int i = 0; i < POOL_CAPACITY - 1; ++i)
    {
        Order* node = &pool->data[i];
        node->next = &pool->data[i + 1];
    }
    pool->data[POOL_CAPACITY - 1].next = NULL;
    return pool;
}

Order* pool_alloc(Pool* pool)
{
    Order* next = pool->freeHead;
    if (next != NULL)
    {
        pool->freeHead = next->next;
        next->next = NULL;
        next->level = NULL;
        next->orderId = 0;
        next->quantity = 0;
    }
    return next;
}

void pool_free(Pool* pool, Order* order)
{
    Order* prev_head = pool->freeHead;
    pool->freeHead = order;
    order->next = prev_head;
}

void pool_destroy(Pool *pool)
{
    free(pool);
}
