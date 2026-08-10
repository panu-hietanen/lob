#ifndef PRICE_LEVEL_H_
#define PRICE_LEVEL_H_

#include "config.h"
#include "types.h"

void insert_back(PriceLevel* level, Order* order);
void unlink_order(Order* order);
bool is_empty(const PriceLevel* level);

#endif // PRICE_LEVEL_H_