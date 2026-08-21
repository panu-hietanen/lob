#ifndef ORDER_BOOK_H_
#define ORDER_BOOK_H_

#include "config.h"
#include "types.h"
#include "map.h"
#include "pool.h"

typedef struct OrderBook {
    PriceLevel* bestBid;
    PriceLevel* bestAsk;
    PriceLevel* bidRoot;
    PriceLevel* askRoot;
    Map bidMap;
    Map askMap;
    Map ordersMap;
    Pool* orderData;
} OrderBook;

#endif // ORDER_BOOK_H_