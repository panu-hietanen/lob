#ifndef TYPES_H_
#define TYPES_H_

#include "config.h"

typedef enum Side {
    BUY,
    SELL
} Side;

typedef struct Order Order;
typedef uint64_t Price;

typedef struct PriceLevel {
    Price price;
    uint64_t length;
    Order* front;
    Order* back;
    Side side;
} PriceLevel;

typedef uint64_t OrderId;

typedef struct Order {
    OrderId orderId;
    uint64_t quantity;
    Order* prev;
    Order* next;
    PriceLevel* level;
} Order;


#endif // TYPES_H_