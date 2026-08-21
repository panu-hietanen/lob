#ifndef PRICE_LEVEL_H_
#define PRICE_LEVEL_H_

#include "config.h"
#include "types.h"

void tree_insert(PriceLevel** rootptr, PriceLevel* level);
void tree_remove(PriceLevel** rootptr, PriceLevel* level);
PriceLevel* tree_min(PriceLevel* root);
PriceLevel* tree_max(PriceLevel* root);

#endif // PRICE_LEVEL_H_