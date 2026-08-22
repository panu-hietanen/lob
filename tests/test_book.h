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

bool book_delete(Side side);
bool book_delete_invalid(Side side);
bool book_delete_update_best(Side side);
bool book_delete_dont_update_best(Side side);

bool book_cancel(Side side);
bool book_cancel_invalid(Side side);
bool book_cancel_full_amount(Side side);
bool book_cancel_too_much(Side side);

bool book_submit_no_cross(Side side);
bool book_submit_full_match_exact_quantity(Side side);
bool book_submit_full_match_exact_price(Side side);
bool book_submit_full_match_multi_level(Side side);
bool book_submit_partial(Side side);


#endif // TEST_BOOK_H_