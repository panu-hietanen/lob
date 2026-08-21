#include "price_tree.h"

void tree_insert(PriceLevel** rootptr, PriceLevel* level)
{
    PriceLevel** curr = rootptr;
    PriceLevel* parent = NULL;
    while (*curr != NULL)
    {
        parent = *curr;
        if (level->price > (*curr)->price)
        {
            curr = &(*curr)->right;
        }
        else
        {
            curr = &(*curr)->left;
        }
    }
    *curr = level;
    level->parent = parent;
    level->right = NULL;
    level->left = NULL;
}

void tree_remove(PriceLevel** rootptr, PriceLevel* level)
{
    PriceLevel** level_replacement;
    if (level == *rootptr)
    {
        level_replacement = rootptr;
    }
    else
    {
        if (level->parent->right == level)
        {
            level_replacement = &level->parent->right;
        }
        else
        {
            level_replacement = &level->parent->left;
        }
    }
    if (level->left == NULL && level->right == NULL)
    {
        *level_replacement = NULL;
    }
    else if (level->left == NULL || level->right == NULL)
    {
        PriceLevel* child = (level->right == NULL) ? level->left : level->right;
        *level_replacement = child;
        child->parent = level->parent;
    }
    else
    {
        PriceLevel* successor = level->right;
        while (successor->left != NULL)
        {
            successor = successor->left;
        }
        PriceLevel* successor_parent = successor->parent;
        PriceLevel* successor_right = successor->right;

        if (successor != level->right)
        {
            successor_parent->left = successor_right;
            if (successor_right != NULL)
            {
                successor_right->parent = successor_parent;
            }
            successor->right = level->right;
            level->right->parent = successor;
        }
        successor->left = level->left;
        level->left->parent = successor;

        successor->parent = level->parent;
        *level_replacement = successor;

        level->left = NULL;
        level->right = NULL;
    }
}

PriceLevel* tree_min(PriceLevel* root)
{
    if (root == NULL) return NULL;
    PriceLevel* curr = root;
    while (curr->left != NULL)
    {
        curr = curr->left;
    }
    return curr;
}

PriceLevel* tree_max(PriceLevel* root)
{
    if (root == NULL) return NULL;
    PriceLevel* curr = root;
    while (curr->right != NULL)
    {
        curr = curr->right;
    }
    return curr;
}
