#include "order_book.h"

#include "price_tree.h"
#include "price_level.h"

OrderBook *book_init()
{
    OrderBook* book = malloc(sizeof(OrderBook));
    if (book == NULL) return NULL;

    book->orderData = pool_init();
    book->bestAsk = NULL;
    book->bestBid = NULL;
    book->askRoot = NULL;
    book->bidRoot = NULL;

    map_init(&book->askMap);
    map_init(&book->bidMap);
    map_init(&book->ordersMap);

    return book;
}

void book_destroy(OrderBook *book)
{
    pool_destroy(book->orderData);
    tree_destroy(&book->bidRoot);
    tree_destroy(&book->askRoot);
    free(book);
}

bool book_add_order(OrderBook *book, OrderId id, Price price, u64 quantity, Side side)
{
    Order* order = pool_alloc(book->orderData);
    if (order == NULL) return false;

    order->orderId = id;
    order->quantity = quantity;
    
    Map* levelMap;
    PriceLevel** rootptr;
    if (side == BUY)
    {
        levelMap = &book->bidMap;
        rootptr = &book->bidRoot;
    }
    else
    {
        levelMap = &book->askMap;
        rootptr = &book->askRoot;
    }
    if (map_insert(&book->ordersMap, id, order) == FULL)
    {
        pool_free(book->orderData, order);
        return false;
    }

    PriceLevel* level = map_lookup(levelMap, price);
    if (level == NULL) // cache miss
    {
        level = malloc(sizeof(PriceLevel));
        if (level == NULL || map_insert(levelMap, price, level) == FULL)
        {
            map_delete(&book->ordersMap, id);
            pool_free(book->orderData, order);
            return false;
        }

        level->price = price;
        level->length = 0;
        
        level->back = NULL;
        level->front = NULL;

        tree_insert(rootptr, level);
        book_register_best(book, level, side);
    }
    insert_back(level, order);
    return true;
}

void book_register_best(OrderBook *book, PriceLevel *level, Side side)
{
    if (side == BUY)
    {
        if (book->bestBid == NULL || level->price > book->bestBid->price)
        {
            book->bestBid = level;
        }
    }
    else
    {
        if (book->bestAsk == NULL || level->price < book->bestAsk->price)
        {
            book->bestAsk = level;
        }
    }
}