#include "test_tree.h"

#include "../src/price_tree.h"

bool tree_invariant(PriceLevel* root)
{
    if (root == NULL) return true;
    if (root->left != NULL && (root->left->price > root->price || root->left->parent != root))
    {
        return false;
    }
    else if (root->left != NULL)
    {
        if (!tree_invariant(root->left)) return false;
    }

    if (root->right != NULL && (root->right->price < root->price || root->right->parent != root))
    {
        return false;
    }
    else if (root->right != NULL)
    {
        if (!tree_invariant(root->right)) return false;
    }
    return true;
}

bool tree_insert_empty()
{
    PriceLevel* root = NULL;
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};

    tree_insert(&root, &tree);
    return tree_invariant(&tree);
}

bool tree_insert_level()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node = {.price = 1, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node);

    if (root->left != &node) return false;

    return tree_invariant(root);
}

bool tree_remove_root()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    tree_remove(&root, root);
    if (root != NULL) return false;

    return tree_invariant(&tree);
}

bool tree_remove_no_children()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 6, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);
    
    tree_remove(&root, &node1);
    if (root->right != NULL) return false;

    return tree_invariant(&tree);
}

bool tree_remove_one_child_right()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 6, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 7, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);
    
    tree_remove(&root, &node1);
    if (root->right != &node2) return false;

    return tree_invariant(&tree);
}

bool tree_remove_one_child_left()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 4, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 3, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);
    
    tree_remove(&root, &node1);
    if (root->left != &node2) return false;

    return tree_invariant(&tree);
}

bool tree_remove_two_children_one_hop()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 7, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 8, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);

    PriceLevel node3 = {.price = 6, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node3);
    
    tree_remove(&root, &node1);
    if (root->right != &node2) return false;

    return tree_invariant(&tree);
}

bool tree_remove_two_children_over_one_hop()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 7, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 6, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);

    PriceLevel node3 = {.price = 9, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node3);

    PriceLevel node4 = {.price = 8, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node4);
    
    tree_remove(&root, &node1);
    if (root->right != &node4) return false;

    return tree_invariant(&tree);
}

bool tree_find_min()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 3, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 2, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);

    PriceLevel node3 = {.price = 9, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node3);

    PriceLevel node4 = {.price = 8, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node4);

    PriceLevel* minNode = tree_min(root);
    
    if (minNode->price != 2) return false;
    return tree_invariant(&tree);
}

bool tree_find_max()
{
    PriceLevel tree = {.price = 5, .parent = NULL, .left = NULL, .right = NULL};
    PriceLevel* root = &tree;

    PriceLevel node1 = {.price = 3, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node1);

    PriceLevel node2 = {.price = 2, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node2);

    PriceLevel node3 = {.price = 9, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node3);

    PriceLevel node4 = {.price = 8, .parent = NULL, .left = NULL, .right = NULL};
    tree_insert(&root, &node4);
    
    if (tree_max(root)->price != 9) return false;
    return tree_invariant(&tree);
}

int main()
{
    typedef struct TestCase {
        const char* name;
        bool (*fn)();
    } TestCase;

    PriceLevel level;

    TestCase tests[] = {
        {"tree_insert_empty", tree_insert_empty},
        {"tree_insert_level", tree_insert_level},
        {"tree_remove_root", tree_remove_root},
        {"tree_remove_no_children", tree_remove_no_children},
        {"tree_remove_one_child_right", tree_remove_one_child_right},
        {"tree_remove_one_child_left", tree_remove_one_child_left},
        {"tree_remove_two_children_one_hop", tree_remove_two_children_one_hop},
        {"tree_remove_two_children_over_one_hop", tree_remove_two_children_over_one_hop},
        {"tree_find_min", tree_find_min},
        {"tree_find_max", tree_find_max},
    };

    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    for (int i = 0; i < total; ++i)
    {
        bool ok = tests[i].fn();
        if (ok) passed++;
    }

    return passed == total ? 0 : 1;
}