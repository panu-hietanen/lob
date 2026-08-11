#include "price_level.h"

void insert_back(PriceLevel* level, Order* order)
{
    if (level->back)
    {
        level->back->next = order;
    }
    if (!level->front)
    {
        level->front = order;
    }
    order->prev = level->back;
    order->next = NULL;
    order->level = level;
    
    level->back = order;
    level->length++;

}

void unlink_order(Order* order)
{
    Order* prev = order->prev;
    Order* next = order->next;
    if (prev)
    {
        prev->next = next;
    }
    else
    {
        order->level->front = next;
    }

    if (next)
    {
        next->prev = prev;
    }
    else
    {
        order->level->back = prev;
    }
    
    order->level->length--;

    order->level = NULL;
    order->next = NULL;
    order->prev = NULL;
}

bool is_empty(const PriceLevel* level)
{
    return level->length == 0;
}