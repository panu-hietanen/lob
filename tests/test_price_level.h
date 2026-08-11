#ifndef TEST_PRICE_LEVEL_H_
#define TEST_PRICE_LEVEL_H_

#include "../src/config.h"
#include "../src/types.h"

bool list_invariant(const PriceLevel* level);

bool insert_back_empty(PriceLevel* level);
bool insert_back_single(PriceLevel* level);
bool insert_back_several(PriceLevel* level);

bool unlink_single(PriceLevel* level);
bool unlink_start(PriceLevel* level);
bool unlink_middle(PriceLevel* level);
bool unlink_back(PriceLevel* level);


#endif // TEST_PRICE_LEVEL_H_