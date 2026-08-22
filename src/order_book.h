#ifndef ORDER_BOOK_H_
#define ORDER_BOOK_H_

#include "config.h"
#include "types.h"
#include "map.h"
#include "pool.h"

typedef struct Receipt {
    OrderId id;
    Price price;
    u64 quantity;
} Receipt;

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

OrderBook* book_init();
void book_destroy(OrderBook* book);

bool book_add_order(OrderBook* book, OrderId id, Price price, u64 quantity, Side side);
void book_register_best(OrderBook* book, PriceLevel* level, Side side);
void book_update_best(OrderBook* book, Side side);

bool book_cancel_order_id(OrderBook* book, OrderId id, u64 quantity, Side side);
bool book_delete_order_id(OrderBook* book, OrderId id, Side side);
bool book_cancel_order(OrderBook* book, Order* order, u64 quantity, Side side);
bool book_delete_order(OrderBook* book, Order* order, Side side);

bool book_submit_order(OrderBook* book, OrderId id, Price price, u64 quantity, Side side, Receipt* receipt, u64* filled);

#endif // ORDER_BOOK_H_