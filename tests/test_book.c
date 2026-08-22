#include "test_book.h"

bool book_order_present(OrderBook* book, OrderId id, u64 quantity)
{
    Order* order = map_lookup(&book->ordersMap, id);
    if (order == NULL) return false;
    if (order->orderId != id) return false;
    if (order->quantity != quantity) return false;

    return true;
}

bool book_check_best(OrderBook* book, Price price, Side side)
{
    PriceLevel* best;
    if (side == BUY)
    {
        best = book->bestBid;
    }
    else
    {
        best = book->bestAsk;
    }
    if (best == NULL) return false;
    return (best->price == price);
}

bool book_level_present(OrderBook* book, Price price, Side side)
{
    Map* map;
    if (side == BUY)
    {
        map = &book->bidMap;
    }
    else
    {
        map = &book->askMap;
    }
    PriceLevel* level = map_lookup(map, price);
    if (level == NULL) return false;
    return level->price == price;
}

bool book_first_level(Side side)
{
    OrderBook* book = book_init();

    OrderId id = 1;
    Price price = 5;
    u64 quantity = 100;
    book_add_order(book, id, price, quantity, side);
    if (!book_check_best(book, price, side)) return false;
    if (!book_level_present(book, price, side)) return false;
    if (!book_order_present(book, id, quantity)) return false;

    book_destroy(book);
    return true;
}

bool book_new_best_price(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);
    if (!book_level_present(book, price1, side) || !book_order_present(book, id1, quantity1)) return false;
    if (!book_check_best(book, price1, side)) return false;

    OrderId id2 = 2;
    Price price2 = (side == BUY) ? 6 : 4;
    u64 quantity2 = 100;
    book_add_order(book, id2, price2, quantity2, side);
    if (!book_level_present(book, price2, side)) return false;
    if (!book_order_present(book, id2, quantity2)) return false;
    if (!book_check_best(book, price2, side)) return false;
    
    book_destroy(book);
    return true;
}

bool book_new_level(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);
    if (!book_level_present(book, price1, side) || !book_order_present(book, id1, quantity1)) return false;
    if (!book_check_best(book, price1, side)) return false;

    OrderId id2 = 2;
    Price price2 = (side == BUY) ? 4 : 6;
    u64 quantity2 = 100;
    book_add_order(book, id2, price2, quantity2, side);
    if (!book_level_present(book, price2, side)) return false;
    if (!book_order_present(book, id2, quantity2)) return false;

    if (!book_check_best(book, price1, side)) return false;
    
    book_destroy(book);
    return true;
}

bool book_add_to_level(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);
    if (!book_level_present(book, price1, side) || !book_order_present(book, id1, quantity1)) return false;
    if (!book_check_best(book, price1, side)) return false;

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    if (!book_level_present(book, price1, side)) return false;
    if (!book_order_present(book, id2, quantity2)) return false;
    
    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 2) return false;

    if (!book_check_best(book, price1, side)) return false;
    
    book_destroy(book);
    return true;
}

bool book_delete(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    if (!book_delete_order(book, id2, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) != NULL) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 1) return false;

    book_destroy(book);
    return true;
}

bool book_delete_invalid(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    OrderId id3 = 3;
    if (book_delete_order(book, id3, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) == NULL) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 2) return false;

    book_destroy(book);
    return true;
}

bool book_delete_update_best(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    Price price2 = (side == BUY) ? 6 : 4;
    u64 quantity2 = 100;
    book_add_order(book, id2, price2, quantity2, side);
    PriceLevel* best = (side == BUY) ? book->bestBid : book->bestAsk;
    if (best->price != price2) return false;
    
    if (!book_delete_order(book, id2, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) != NULL) return false;

    PriceLevel* newBest = (side == BUY) ? book->bestBid : book->bestAsk;
    if (newBest->price != price1) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price2);
    if (level != NULL) return false;

    book_destroy(book);
    return true;   
}

bool book_delete_dont_update_best(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    Price price2 = (side == BUY) ? 6 : 4;
    u64 quantity2 = 100;
    book_add_order(book, id2, price2, quantity2, side);
    PriceLevel* best = (side == BUY) ? book->bestBid : book->bestAsk;
    if (best->price != price2) return false;
    
    if (!book_delete_order(book, id1, side)) return false;
    if (map_lookup(&book->ordersMap, id1) != NULL) return false;
    if (map_lookup(&book->ordersMap, id2) == NULL) return false;

    PriceLevel* newBest = (side == BUY) ? book->bestBid : book->bestAsk;
    if (newBest->price != price2) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level != NULL) return false;

    book_destroy(book);
    return true;   
}

bool book_cancel(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    u64 cancel_amount = 90;
    if (!book_cancel_order(book, id2, cancel_amount, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    Order* newOrder = map_lookup(&book->ordersMap, id2);
    if (newOrder->quantity != quantity2 - cancel_amount) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 2) return false;

    book_destroy(book);
    return true;
}

bool book_cancel_invalid(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    OrderId id3 = 3;
    u64 cancel_amount = 90;
    if (book_cancel_order(book, id3, cancel_amount, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) == NULL) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 2) return false;

    book_destroy(book);
    return true;
}

bool book_cancel_full_amount(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    u64 cancel_amount = 100;
    if (!book_cancel_order(book, id2, cancel_amount, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) != NULL) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 1) return false;

    book_destroy(book);
    return true;
}
bool book_cancel_too_much(Side side)
{
    OrderBook* book = book_init();

    OrderId id1 = 1;
    Price price1 = 5;
    u64 quantity1 = 100;
    book_add_order(book, id1, price1, quantity1, side);

    OrderId id2 = 2;
    u64 quantity2 = 100;
    book_add_order(book, id2, price1, quantity2, side);
    
    u64 cancel_amount = 110;
    if (book_cancel_order(book, id2, cancel_amount, side)) return false;
    if (map_lookup(&book->ordersMap, id1) == NULL) return false;
    if (map_lookup(&book->ordersMap, id2) == NULL) return false;

    Map* map = (side == BUY) ? &book->bidMap : &book->askMap;
    PriceLevel* level = map_lookup(map, price1);
    if (level == NULL) return false;
    if (level->length != 2) return false;

    book_destroy(book);
    return true;
}

int main()
{
    typedef struct TestCase {
        const char* name;
        bool (*fn)(Side);
    } TestCase;

    TestCase tests[] = {
        {"book_first_level", book_first_level},
        {"book_new_best_price", book_new_best_price},
        {"book_new_level", book_new_level},
        {"book_add_to_level", book_add_to_level},
        {"book_delete", book_delete},
        {"book_delete_invalid", book_delete_invalid},
        {"book_delete_update_best", book_delete_update_best},
        {"book_delete_dont_update_best", book_delete_dont_update_best},
        {"book_cancel", book_cancel},
        {"book_cancel_invalid", book_cancel_invalid},
        {"book_cancel_full_amount", book_cancel_full_amount},
        {"book_cancel_too_much", book_cancel_too_much},
    };

    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    for (int i = 0; i < total; ++i)
    {
        bool ok = tests[i].fn(BUY);
        ok &= tests[i].fn(SELL);
        if (ok) passed++;
    }

    return passed == total ? 0 : 1;
}