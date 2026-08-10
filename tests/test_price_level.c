#include <assert.h>

#include "test_price_level.h"

bool list_invariant(const PriceLevel* level)
{
    return true;
}

bool insert_back_empty(PriceLevel* level)
{
    return list_invariant(level);
}
bool insert_back_single(PriceLevel* level)
{
    return list_invariant(level);
}
bool insert_back_several(PriceLevel* level)
{
    return list_invariant(level);
}

bool unlink_empty(PriceLevel* level)
{
    return list_invariant(level);
}
bool unlink_start(PriceLevel* level)
{
    return list_invariant(level);
}
bool unlink_middle(PriceLevel* level)
{
    return list_invariant(level);
}
bool unlink_back(PriceLevel* level)
{
    return list_invariant(level);
}

int main()
{
    PriceLevel level;
    PriceLevel* pLevel = &level;

    if (!insert_back_empty(pLevel)) return 1;
    if (!insert_back_single(pLevel)) return 1;
    if (!insert_back_several(pLevel)) return 1;
    if (!unlink_empty(pLevel)) return 1;
    if (!unlink_start(pLevel)) return 1;
    if (!unlink_middle(pLevel)) return 1;
    if (!unlink_back(pLevel)) return 1;
    return 0;
}