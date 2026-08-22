#ifndef PRICE_TREE_H_
#define PRICE_TREE_H_

#include "config.h"
#include "types.h"

void tree_destroy(PriceLevel** rootptr);

void tree_insert(PriceLevel** rootptr, PriceLevel* level);
void tree_remove(PriceLevel** rootptr, PriceLevel* level);

PriceLevel* tree_min(PriceLevel* root);
PriceLevel* tree_max(PriceLevel* root);
PriceLevel* tree_next_min(PriceLevel* node);
PriceLevel* tree_next_max(PriceLevel* node);

#endif // PRICE_TREE_H_