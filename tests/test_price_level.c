#include <assert.h>

#include "test_price_level.h"
#include "../src/price_level.h"

bool list_invariant(const PriceLevel* level)
{
    // If level is empty then front and back are null
    bool t1 = true;
    if (is_empty(level))
    {
        t1 = (level->front == NULL && 
              level->back  == NULL);
    }
    
    // Front and back have null prev and next respectively if level is nonempty
    bool t2 = true;
    if (level->length > 0)
    {
        t2 = (level->front->prev == NULL &&
              level->back->next  == NULL);
    }

    // If level has length one then front == back
    bool t3 = true;
    if (level->length == 1)
    {
        t3 = (level->front == level->back);
    }

    // If a and b are neighbours then a = b.prev and b = a.next
    bool t4 = true;
    if (level->length > 1)
    {
        Order* prev = level->front;
        Order* next = level->front->next;
        while (next)
        {
            if (next->prev != prev)
            {
                t4 = false;
                break;
            }
            prev = next;
            next = next->next;
        }
    }

    // An order always points to the containing level
    bool t5 = true;
    {
        Order* o = level->front;
        while (o)
        {
            if (o->level != level)
            {
                t5 = false;
                break;
            }
            o = o->next;
        }
    }
    return t1 && t2 && t3 && t4 && t5;
}

bool insert_back_empty(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    insert_back(level, &o1);
    return list_invariant(level);
}
bool insert_back_single(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    Order o2 = { .orderId = 2 };

    insert_back(level, &o1);
    insert_back(level, &o2);
    return list_invariant(level);
}

bool insert_back_several(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    Order o2 = { .orderId = 2 };
    Order o3 = { .orderId = 3 };

    insert_back(level, &o1);
    insert_back(level, &o2);
    insert_back(level, &o3);
    return list_invariant(level);
}

bool unlink_single(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    insert_back(level, &o1);
    
    unlink_order(&o1);
    return list_invariant(level);
}

bool unlink_start(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    Order o2 = { .orderId = 2 };
    Order o3 = { .orderId = 3 };
    insert_back(level, &o1);
    insert_back(level, &o2);
    insert_back(level, &o3);
    
    unlink_order(&o1);
    return list_invariant(level);
}

bool unlink_middle(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    Order o2 = { .orderId = 2 };
    Order o3 = { .orderId = 3 };
    insert_back(level, &o1);
    insert_back(level, &o2);
    insert_back(level, &o3);
    
    unlink_order(&o2);
    return list_invariant(level);
}

bool unlink_back(PriceLevel* level)
{
    Order o1 = { .orderId = 1 };
    Order o2 = { .orderId = 2 };
    Order o3 = { .orderId = 3 };
    insert_back(level, &o1);
    insert_back(level, &o2);
    insert_back(level, &o3);
    
    unlink_order(&o3);
    return list_invariant(level);
}

int main()
{
    PriceLevel level = {.length = 0, .front = NULL, .back = NULL};

    if (!insert_back_empty(&level)) return 1;

    level = (PriceLevel) {.length = 0, .front = NULL, .back = NULL};
    if (!insert_back_single(&level)) return 1;
    
    level = (PriceLevel) {.length = 0, .front = NULL, .back = NULL};
    if (!insert_back_several(&level)) return 1;

    level = (PriceLevel) {.length = 0, .front = NULL, .back = NULL};
    if (!unlink_single(&level)) return 1;

    level = (PriceLevel) {.length = 0, .front = NULL, .back = NULL};
    if (!unlink_middle(&level)) return 1;

    level = (PriceLevel) {.length = 0, .front = NULL, .back = NULL};
    if (!unlink_back(&level)) return 1;

    return 0;
}