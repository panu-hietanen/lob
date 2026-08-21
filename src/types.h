#ifndef TYPES_H_
#define TYPES_H_

#include "config.h"

typedef enum Side {
    BUY,
    SELL
} Side;

typedef struct Order Order;
typedef struct PriceLevel PriceLevel;
typedef u64 Price;

typedef struct PriceLevel {
    Price price;
    u64 length;
    Order* front;
    Order* back;
    PriceLevel* parent;
    PriceLevel* left;
    PriceLevel* right;
} PriceLevel;

typedef u64 OrderId;

typedef struct Order {
    OrderId orderId;
    u64 quantity;
    Order* prev;
    Order* next;
    PriceLevel* level;
} Order;


#endif // TYPES_H_