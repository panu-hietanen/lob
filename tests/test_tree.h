#ifndef TEST_TREE_H_
#define TEST_TREE_H_

#include "../src/config.h"
#include "../src/types.h"

bool tree_insert_empty();
bool tree_insert_level();

bool tree_remove_root();
bool tree_remove_no_children();
bool tree_remove_one_child_right();
bool tree_remove_one_child_left();
bool tree_remove_two_children_one_hop();
bool tree_remove_two_children_over_one_hop();

bool tree_find_min();
bool tree_find_max();

#endif // TEST_TREE_H_