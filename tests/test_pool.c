#include "test_pool.h"

bool pool_alloc_full()
{
    Pool* pool = pool_init();
    for (int i = 0; i < POOL_CAPACITY; ++i)
    {
        if (pool_alloc(pool) == NULL) return false;
    }
    if (pool_alloc(pool) != NULL) return false;
    
    pool_destroy(pool);
    return true;
}

bool pool_alloc_after_free()
{
    Pool* pool = pool_init();

    Order* original = pool_alloc(pool);
    if (original == NULL) return false;
    pool_free(pool, original);

    if (original != pool_alloc(pool)) return false;
    
    pool_destroy(pool);
    return true;
}

bool pool_free_zeroes_fields()
{
    Pool* pool = pool_init();

    Order* original = pool_alloc(pool);
    if (original == NULL) return false;
    original->orderId = 10;
    original->quantity = 25;
    pool_free(pool, original);

    if (original != pool_alloc(pool)) return false;
    if (original->orderId != 0) return false;
    if (original->quantity != 0) return false;
    
    pool_destroy(pool);
    return true;
}


int main()
{
    typedef struct TestCase {
        const char* name;
        bool (*fn)();
    } TestCase;

    TestCase tests[] = {
        {"pool_alloc_full", pool_alloc_full},
        {"pool_alloc_after_free", pool_alloc_after_free},
        {"pool_free_zeroes_fields", pool_free_zeroes_fields},
    };

    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    for (int i = 0; i < total; ++i)
    {
        bool ok = tests[i].fn();
        if (ok) passed++;
    }

    return passed == total ? 0 : 1;
}