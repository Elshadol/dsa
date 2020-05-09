#include "avltree.h"

#define AVL_BALANCED(bf) ((-2 < bf) && (bf < 2))
#define AVL_HEIGHT(n) (((n) == NULL) ? -1 : (n)->avl_height)

struct avl_node *avl_first(const struct avl_root *root)
{
    struct avl_node *node = root->avl_node;

    if (node == NULL)
        return NULL;

    while (node->avl_left != NULL)
        node = node->avl_left;

    return node;
}

struct avl_node *avl_last(const struct avl_root *root)
{
    struct avl_node *node = root->avl_node;

    if (node == NULL)
        return NULL;

    while (node->avl_right != NULL)
        node = node->avl_right;

    return node;
}

struct avl_node *avl_prev(const struct avl_node *node)
{
    if (node == NULL)
        return NULL;

    if (node->avl_left != NULL) {
        node = node->avl_left;
        while (node->avl_right)
            node = node->avl_right;
        return (struct avl_node *)node;
    } else {
        struct avl_node *parent;
        while (((parent = node->avl_parent) != NULL) &&
               (node == parent->avl_left))
            node = parent;
        return parent;
    }
}

struct avl_node *avl_next(const struct avl_node *node)
{
    if (node == NULL)
        return NULL;

    if (node->avl_right) {
        node = node->avl_right;
        while (node->avl_left)
            node = node->avl_left;
        return (struct avl_node *)node;
    } else {
        struct avl_node *parent;
        while (((parent =  node->avl_parent) != NULL) &&
               (node == parent->avl_right))
            node = parent;
        return parent;
    }
}

void avl_replace_node(struct avl_node *node, struct avl_node *replacer,
                      struct avl_root *root)
{
    if (node->avl_parent == NULL)
        root->avl_node = replacer;
    else if (node->avl_parent->avl_left == node)
        node->avl_parent->avl_left = replacer;
    else
        node->avl_parent->avl_right = replacer;

    if (node->avl_left != NULL)
        node->avl_left->avl_parent = replacer;

    if (node->avl_right != NULL)
        node->avl_right->avl_parent = replacer;

    *replacer = *node;
}


static inline void __avl_update_height(struct avl_node *node)
{
    int h0 = AVL_HEIGHT(node->avl_left);
    int h1 = AVL_HEIGHT(node->avl_right);
    node->avl_height = ((h0 < h1) ? h1 : h0) + 1;
}

static void __avl_rotate_left(struct avl_node *x, struct avl_root *root)
{
    struct avl_node *y = x->avl_right;

    x->avl_right = y->avl_left;
    if (y->avl_left != NULL)
        y->avl_left->avl_parent = x;

    y->avl_parent = x->avl_parent;
    if (x->avl_parent == NULL)
        root->avl_node = y;
    else if (x == x->avl_parent->avl_left)
        x->avl_parent->avl_left = y;
    else
        x->avl_parent->avl_right = y;

    y->avl_left = x;
    x->avl_parent = y;
}

static void __avl_rotate_right(struct avl_node *x, struct avl_root *root)
{
    struct avl_node *y = x->avl_left;

    x->avl_left = y->avl_right;
    if (y->avl_right != NULL)
        y->avl_right->avl_parent = x;

    y->avl_parent = x->avl_parent;
    if (x->avl_parent == NULL)
        root->avl_node = y;
    else if (x == x->avl_parent->avl_left)
        x->avl_parent->avl_left = y;
    else
        x->avl_parent->avl_right = y;

    y->avl_right = x;
    x->avl_parent = y;
}

static void __avl_rotate_left_right(struct avl_node *a, struct avl_root *root)
{
    struct avl_node *b = a->avl_left;
    struct avl_node *c = b->avl_right;

    a->avl_left = c->avl_right;
    b->avl_right = c->avl_left;
    c->avl_right = a;
    c->avl_left = b;

    c->avl_parent = a->avl_parent;
    a->avl_parent = b->avl_parent = c;

    if (a->avl_left != NULL)
        a->avl_left->avl_parent = a;


    if (b->avl_right != NULL)
        b->avl_right->avl_parent = b;

    if (c->avl_parent == NULL)
        root->avl_node = c;
    else if (c->avl_parent->avl_left == a)
        c->avl_parent->avl_left = c;
    else
        c->avl_parent->avl_right = c;
}

static void __avl_rotate_right_left(struct avl_node *a, struct avl_root *root)
{
    struct avl_node *b = a->avl_right;
    struct avl_node *c = b->avl_left;

    a->avl_right = c->avl_left;
    b->avl_left = c->avl_right;

    c->avl_left = a;
    c->avl_right = b;

    c->avl_parent = a->avl_parent;
    a->avl_parent = b->avl_parent = c;

    if (a->avl_right != NULL)
        a->avl_right->avl_parent = a;

    if (b->avl_left != NULL)
        b->avl_left->avl_parent = b;

    if (c->avl_parent == NULL)
        root->avl_node = c;
    else if (c->avl_parent->avl_left == a)
        c->avl_parent->avl_left = c;
    else
        c->avl_parent->avl_right = c;
}

void avl_insert_rebalance(struct avl_node *node, struct avl_root *root)
{
    while ((node = node->avl_parent) != NULL) {
        int h0 = AVL_HEIGHT(node->avl_left);
        int h1 = AVL_HEIGHT(node->avl_right);
        int balancefactor = h0 - h1;
        int height = ((h0 < h1) ? h1 : h0) + 1;

        if (node->avl_height == height) {
            return;
        } else if (AVL_BALANCED(balancefactor)) {
            node->avl_height = height;
        } else {
            if (balancefactor < -1) {
                struct avl_node *right = node->avl_right;
                int rh0 = AVL_HEIGHT(right->avl_left);
                int rh1 = AVL_HEIGHT(right->avl_right);
                if (rh0 > rh1) {
                    __avl_rotate_right_left(node, root);
                    node = node->avl_parent;
                    __avl_update_height(node->avl_right);
                } else {
                    __avl_rotate_left(node, root);
                    node = node->avl_parent;
                }
                __avl_update_height(node->avl_left);
                __avl_update_height(node);
            } else if (1 < balancefactor) {
                struct avl_node *left = node->avl_left;
                int lh0 = AVL_HEIGHT(left->avl_left);
                int lh1 = AVL_HEIGHT(left->avl_right);
                if (lh0 < lh1) {
                    __avl_rotate_left_right(node, root);
                    node = node->avl_parent;
                    __avl_update_height(node->avl_left);
                } else {
                    __avl_rotate_right(node, root);
                    node = node->avl_parent;
                }
                __avl_update_height(node->avl_right);
                __avl_update_height(node);
            }
            return;
        }
    }
}

static void __avl_post_erase(struct avl_node *node, struct avl_node *parent,
                             struct avl_root *root)
{
    while (node != root->avl_node) {
        int h0 = AVL_HEIGHT(parent->avl_left);
        int h1 = AVL_HEIGHT(parent->avl_right);
        int bf = h0 - h1;
        int height = ((h0 < h1) ? h1 : h0) + 1;
        int old_height = parent->avl_height;

        if ((parent->avl_height == height) && AVL_BALANCED(bf)) {
            return;
        } else if (AVL_BALANCED(bf)) {
            parent->avl_height = height;
            node = parent;
            parent = parent->avl_parent;
        } else {
            if (bf < -1) {
                struct avl_node *right = parent->avl_right;
                int rh0 = AVL_HEIGHT(right->avl_left);
                int rh1 = AVL_HEIGHT(right->avl_right);
                if (rh0 > rh1) {
                    __avl_rotate_right_left(parent, root);
                    node = parent->avl_parent;
                    __avl_update_height(node->avl_right);
                } else {
                    __avl_rotate_left(parent, root);
                    node = parent->avl_parent;
                }
                __avl_update_height(node->avl_left);
            } else if (1 < bf) {
                struct avl_node *left = parent->avl_left;
                int lh0 = AVL_HEIGHT(left->avl_left);
                int lh1 = AVL_HEIGHT(left->avl_right);
                if (lh0 < lh1) {
                    __avl_rotate_left_right(parent, root);
                    node = parent->avl_parent;
                    __avl_update_height(node->avl_left);
                } else {
                    __avl_rotate_right(parent, root);
                    node = parent->avl_parent;
                }
                __avl_update_height(node->avl_right);
            }
			__avl_update_height(node);
            parent = node->avl_parent;

            if (node->avl_height == old_height)
                return;
        }
    }
}

void avl_erase(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *parent, *child;

    if ((node->avl_left != NULL) && (node->avl_right != NULL)) {
        struct avl_node *replacer = node->avl_right;
        while (replacer->avl_left != NULL)
            replacer = replacer->avl_left;

        child = replacer->avl_right;

        node->avl_left->avl_parent = replacer;
        replacer->avl_left = node->avl_left;

        if (replacer != node->avl_right) {
            parent = replacer->avl_parent;

            if (child != NULL)
                child->avl_parent = replacer->avl_parent;

            replacer->avl_parent->avl_left = child;
            replacer->avl_right = node->avl_right;
            node->avl_right->avl_parent = replacer;
        } else {
            parent = replacer;
        }

        if (node->avl_parent == NULL)
            root->avl_node = replacer;
        else if (node->avl_parent->avl_left == node)
            node->avl_parent->avl_left = replacer;
        else
            node->avl_parent->avl_right = replacer;

        replacer->avl_parent = node->avl_parent;
        replacer->avl_height = node->avl_height;
    } else {
        if (node->avl_left == NULL)
            child = node->avl_right;
        else
            child = node->avl_left;
        parent = node->avl_parent;

        if (child != NULL)
            child->avl_parent = parent;

        if  (parent == NULL)
            root->avl_node = child;
        else if (parent->avl_left == node)
            parent->avl_left = child;
        else
            parent->avl_right = child;
    }

    __avl_post_erase(child, parent, root);
}
