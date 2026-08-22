#ifndef TEST_BOOK_H_
#define TEST_BOOK_H_

#include "../src/config.h"
#include "../src/types.h"
#include "../src/order_book.h"

bool book_order_present(OrderBook* book, OrderId id, u64 quantity);
bool book_check_best(OrderBook* book, Price price, Side side);
bool book_level_present(OrderBook* book, Price price, Side side);
bool book_first_level(Side side);
bool book_new_best_price(Side side);
bool book_new_level(Side side);
bool book_add_to_level(Side side);

#endif // TEST_BOOK_H_