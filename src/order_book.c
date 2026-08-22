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

bool book_cancel_order_id(OrderBook *book, OrderId id, u64 quantity, Side side)
{
    Order* order = map_lookup(&book->ordersMap, id);
    return book_cancel_order(book, order, quantity, side);
}

bool book_cancel_order(OrderBook *book, Order* order, u64 quantity, Side side)
{
    if (order == NULL) return false;
    if (quantity > order->quantity) return false;
    if (quantity == order->quantity) return book_delete_order(book, order, side);

    order->quantity -= quantity;
    return true;
}

bool book_delete_order_id(OrderBook *book, OrderId id, Side side)
{
    Order* order = map_lookup(&book->ordersMap, id);
    return book_delete_order(book, order, side);
}

bool book_delete_order(OrderBook *book, Order* order, Side side)
{
    if (order == NULL) return false;
    PriceLevel* level = order->level;

    Map* map;
    PriceLevel* best;
    PriceLevel** rootptr;
    if (side == BUY)
    {
        map = &book->bidMap;
        best = book->bestBid;
        rootptr = &book->bidRoot;
    }
    else
    {
        map = &book->askMap;
        best = book->bestAsk;
        rootptr = &book->askRoot;
    }

    map_delete(&book->ordersMap, order->orderId);
    unlink_order(order);
    if (is_empty(level))
    {
        tree_remove(rootptr, level);
        map_delete(map, level->price);
        if (best == level)
        {
            book_update_best(book, side);
        }
        free(level);
    }
    pool_free(book->orderData, order);

    return true;
}

void book_update_best(OrderBook *book, Side side)
{
    if (side == BUY)
    {
        book->bestBid = tree_max(book->bidRoot);
    }
    else
    {
        book->bestAsk = tree_min(book->askRoot);
    }
}

bool book_submit_order(OrderBook* book, OrderId id, Price price, u64 quantity, Side side, Receipt* receipt, u64* filled)
{
    Side restingSide = (side == BUY) ? SELL : BUY;
    if (side == BUY)
    {
        if (book->bestAsk != NULL && price >= book->bestAsk->price)
        {
            PriceLevel** rootptr = &book->askRoot;
            PriceLevel* currLevel = tree_min(*rootptr);
            u64 bought = 0;
            while (bought < quantity)
            {
                if (currLevel != NULL && price >= currLevel->price)
                {
                    Order* currOrder = currLevel->front;
                    receipt[*filled].id = currOrder->orderId;
                    receipt[*filled].price = currLevel->price;

                    bool updateLevel = false;
                    if (currOrder->quantity < quantity - bought)
                    {
                        receipt[*filled].quantity = currOrder->quantity;
                        bought += currOrder->quantity;
                        if (currLevel->length == 1) updateLevel = true;
                        book_delete_order(book, currOrder, restingSide);
                        if (updateLevel)
                        {
                            updateLevel = false;
                            currLevel = tree_min(*rootptr);
                        }
                    }
                    else
                    {
                        receipt[*filled].quantity = quantity - bought;
                        book_cancel_order(book, currOrder, quantity - bought, restingSide);
                        bought = quantity;
                    }
                    (*filled)++;
                }
                else
                {
                    return book_add_order(book, id, price, quantity - bought, side);
                }
            }
        }
        else
        {
            return book_add_order(book, id, price, quantity, side);
        }
    }
    else
    {
        if (book->bestBid != NULL && price <= book->bestBid->price)
        {
            PriceLevel** rootptr = &book->bidRoot;
            PriceLevel* currLevel = tree_max(*rootptr);
            u64 bought = 0;
            while (bought < quantity)
            {
                if (currLevel != NULL && price <= currLevel->price)
                {
                    Order* currOrder = currLevel->front;
                    receipt[*filled].id = currOrder->orderId;
                    receipt[*filled].price = currLevel->price;

                    bool updateLevel = false;
                    if (currOrder->quantity < quantity - bought)
                    {
                        receipt[*filled].quantity = currOrder->quantity;
                        bought += currOrder->quantity;
                        if (currLevel->length == 1) updateLevel = true;
                        book_delete_order(book, currOrder, restingSide);
                        if (updateLevel)
                        {
                            updateLevel = false;
                            currLevel = tree_max(*rootptr);
                        }
                    }
                    else
                    {
                        receipt[*filled].quantity = quantity - bought;
                        book_cancel_order(book, currOrder, quantity - bought, restingSide);
                        bought = quantity;
                    }
                    (*filled)++;
                }
                else
                {
                    return book_add_order(book, id, price, quantity - bought, side);
                }
            }
        }
        else
        {
            return book_add_order(book, id, price, quantity, side);
        }
    }
    return true;
}
