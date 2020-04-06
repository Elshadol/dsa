#include "avltree.h"

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
        while ((parent = node->avl_parent) && (node == parent->avl_left))
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
        while ((parent =  node->avl_parent) && (node == parent->avl_right))
            node = parent;

        return parent;
    }
}

void avl_replace_node(struct avl_node *node, struct avl_node *replacer,
                      struct avl_root *root)
{
    if (node->avl_parent != NULL) {
        if (node->avl_parent->avl_left == node)
            node->avl_parent->avl_left = replacer;
        else
            node->avl_parent->avl_right = replacer;
    } else {
        root->avl_node = replacer;
    }

    if (node->avl_left != NULL)
        node->avl_left->avl_parent = replacer;

    if (node->avl_right != NULL)
        node->avl_right->avl_parent = replacer;

    *replacer = *node;
}

#if 0
static inline int __avl_get_height(struct avl_node *node)
{
    return (node == NULL) ? -1 : node->avl_height;
}
#else
#define __avl_get_height(n) \
    (((n) == NULL) ? -1 : (n)->avl_height)
#endif

static inline void __avl_update_height(struct avl_node *node)
{
    int h0 = __avl_get_height(node->avl_left);
    int h1 = __avl_get_height(node->avl_right);

    node->avl_height = ((h0 < h1) ? h1 : h0) + 1;
}

static struct avl_node *
__avl_rotate_left(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *right = node->avl_right;
    struct avl_node *parent = node->avl_parent;

    if ((node->avl_right = right->avl_left))
        right->avl_left->avl_parent = node;

    right->avl_parent = parent;

    if (parent == NULL)
        root->avl_node = right;
    else if (parent->avl_left == node)
        parent->avl_left = right;
    else
        parent->avl_right = right;

    node->avl_parent = right;
    right->avl_left = node;

    __avl_update_height(node);
    __avl_update_height(right);

    return right;
}

static struct avl_node *
__avl_rotate_right(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *left = node->avl_left;
    struct avl_node *parent = node->avl_parent;

    if ((node->avl_left = left->avl_right))
        left->avl_right->avl_parent = node;

    left->avl_parent = parent;

    if (parent == NULL)
        root->avl_node = left;
    else if (parent->avl_left == node)
        parent->avl_left = left;
    else
        parent->avl_right = left;

    node->avl_parent = left;
    left->avl_right = node;

    __avl_update_height(node);
    __avl_update_height(left);

    return left;
}

static struct avl_node *
__avl_rotate_left_right(struct avl_node *a, struct avl_root *root)
{
    struct avl_node *b = a->avl_left;
    struct avl_node *c = b->avl_right;

    a->avl_left = c->avl_right;
    b->avl_right = c->avl_left;

    c->avl_left = b;
    c->avl_right = a;

    c->avl_parent = a->avl_parent;
    a->avl_parent = b->avl_parent = c;

    if (a->avl_left != NULL)
        a->avl_left->avl_parent = a;

    if (b->avl_right != NULL)
        b->avl_right->avl_parent = b;

    if (a->avl_parent == NULL)
        root->avl_node = c;
    else if (a->avl_parent->avl_left == a)
        a->avl_parent->avl_left = c;
    else
        a->avl_parent->avl_right = c;

    __avl_update_height(a);
    __avl_update_height(b);
    __avl_update_height(c);

    return c;
}


static struct avl_node *
__avl_rotate_right_left(struct avl_node *a, struct avl_root *root)
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

        if (b->avl_left->avl_parent)
                b->avl_left->avl_parent = b;

        if (a->avl_parent == NULL)
                root->avl_node = c;
        else if (a->avl_parent->avl_left == a)
                a->avl_parent->avl_left = c;
        else
                a->avl_parent->avl_right = c;

        __avl_update_height(a);
        __avl_update_height(b);
        __avl_update_height(c);

        return c;
}

static void __avl_rebalance_to_root(struct avl_node *node,
                                    struct avl_root *root)
{
    while (node != NULL) {
        int h0 = __avl_get_height(node->avl_left);
        int h1 = __avl_get_height(node->avl_right);
        int balancefactor = h0 - h1;
        int height = ((h0 < h1) ? h1 : h0) + 1;

        //If height of the node didn't change, just return, because
        //the  height of the nodes above must keep the same
        if (node->avl_height == height)
            return;
        else
            node->avl_height = height;

        if (balancefactor < -1) {  // right heavy
            struct avl_node *right = node->avl_right;
            int rh0 = __avl_get_height(right->avl_left);
            int rh1 = __avl_get_height(right->avl_right);

            if (rh0 > rh1)  // left height
                    node = __avl_rotate_right_left(node, root);
            else
                    node = __avl_rotate_left(node, root);

        } else if (1 < balancefactor) {
            struct avl_node *left = node->avl_left;
            int lh0 = __avl_get_height(left->avl_left);
            int lh1 = __avl_get_height(left->avl_right);

            if (lh0 < lh1)
                    node = __avl_rotate_left_right(node, root);
            else
                    node = __avl_rotate_right(node, root);
        }
        node = node->avl_parent;
    }
}


void  avl_insert_rebalance(struct avl_node *node, struct avl_root *root)
{
#if 0
    for (node = node->avl_parent; node != NULL; node = node->avl_parent) {
        int h0 = __avl_get_height(node->avl_left);
        int h1 = __avl_get_height(node->avl_right);
        int balancefactor = h0 - h1;
        int height = ((h0 < h1) ? h1 : h0) + 1;

        //If height of the node didn't change, just return, because
        //the  height of the nodes above must keep the same
        if (node->avl_height == height)
            return;
        else
            node->avl_height = height;

        if (balancefactor < -1) {  // right heavy
            struct avl_node *right = node->avl_right;
            int rh0 = __avl_get_height(right->avl_left);
            int rh1 = __avl_get_height(right->avl_right);

            if (rh0 > rh1)
                    right = __avl_rotate_right(right, root);

                node = __avl_rotate_left(node, root);
        } else if (1 < balancefactor) {
            struct avl_node *left = node->avl_left;
            int lh0 = __avl_get_height(left->avl_left);
            int lh1 = __avl_get_height(left->avl_right);

            if (lh0 < lh1)
                    left = __avl_rotate_left(left, root);

                node = __avl_rotate_right(node, root);
        }

    }
#else
    __avl_rebalance_to_root(node->avl_parent, root);
#endif
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

    if (parent != NULL)
        __avl_rebalance_to_root(parent, root);
}
