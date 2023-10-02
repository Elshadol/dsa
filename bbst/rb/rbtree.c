#include "rbtree.h"

static inline void __rb_change_child(struct rb_node *x, struct rb_node *y,
                                     struct rb_node *x_parent,
                                     struct rb_root *root)
{
    if (x_parent) {
        if (x_parent->rb_left == x)
            x_parent->rb_left = y;
        else
            x_parent->rb_right = y;
    } else
        root->rb_node = y;
}

static inline void __rb_rotate_set_parents(struct rb_node *x,
        struct rb_node *y, struct rb_root *root)
{
    struct rb_node *x_parent = x->rb_parent;
    x->rb_parent = y;
    y->rb_parent = x_parent;
    __rb_change_child(x, y, x_parent, root);
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
    struct rb_node *parent = node->rb_parent, *gparent, *tmp;
    for (;;) {
        if (!parent) {
            node->rb_color = RB_BLACK;
            break;
        } else if (parent->rb_color == RB_BLACK)
            break;
        gparent = parent->rb_parent;
        if (parent == gparent->rb_left) {
            tmp = gparent->rb_right;
            if (tmp && tmp->rb_color == RB_RED) {
                tmp->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                node = gparent;
                parent = node->rb_parent;
                continue;
            }
            tmp = parent->rb_right;
            if (tmp == node) {
                parent->rb_right = tmp = node->rb_left;
                parent->rb_parent = node;
                node->rb_left = parent;
                if (tmp)
                    tmp->rb_parent = parent;
                parent = node;
                tmp = node->rb_right;
            }
            parent->rb_color = RB_BLACK;
            gparent->rb_color = RB_RED;
            gparent->rb_left = tmp;
            if (tmp)
                tmp->rb_parent = gparent;
            parent->rb_right = gparent;
            __rb_rotate_set_parents(gparent, parent, root);
            break;
        } else {
            tmp = gparent->rb_left;
            if (tmp && tmp->rb_color == RB_RED) {
                tmp->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                node = gparent;
                parent = node->rb_parent;
                continue;
            }
            tmp = parent->rb_left;
            if (tmp == node) {
                parent->rb_left = tmp = node->rb_right;
                parent->rb_parent = node;
                node->rb_right = parent;
                if (tmp)
                    tmp->rb_parent = parent;
                parent = node;
                tmp = node->rb_left;
            }
            parent->rb_color = RB_BLACK;
            gparent->rb_color = RB_RED;
            gparent->rb_right = tmp;
            if (tmp)
                tmp->rb_parent = gparent;
            parent->rb_left = gparent;
            __rb_rotate_set_parents(gparent, parent, root);
            break;
        }
    }
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
                             struct rb_root *root)
{
    struct rb_node *sibling, *tmp1, *tmp2;
    while ((!node || node->rb_color == RB_BLACK) && node != root->rb_node) {
        sibling = parent->rb_right;
        if (sibling != node) {   // node == parent->rb_left
            if (sibling->rb_color == RB_RED) {
                sibling->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                parent->rb_right = tmp1 = sibling->rb_left;
                tmp1->rb_parent = parent;
                sibling->rb_left = parent;
                __rb_rotate_set_parents(parent, sibling, root);
                sibling = tmp1;
            }
            tmp1 = sibling->rb_right;
            if (!tmp1 || tmp1->rb_color == RB_BLACK) {
                tmp2 = sibling->rb_left;
                if (!tmp2 || tmp2->rb_color == RB_BLACK) {
                    sibling->rb_color = RB_RED;
                    node = parent;
                    parent = node->rb_parent;
                    continue;
                }
                sibling->rb_color = RB_RED;
                tmp2->rb_color = RB_BLACK;
                sibling->rb_left = tmp1 = tmp2->rb_right;
				if (tmp1)
                    tmp1->rb_parent = sibling;
				tmp2->rb_right = sibling;
                sibling->rb_parent = tmp2;
                tmp2->rb_parent = parent;
				parent->rb_right = tmp2;
                tmp1 = sibling;
                sibling = tmp2;
            }
            sibling->rb_color = parent->rb_color;
            parent->rb_color = RB_BLACK;
            tmp1->rb_color = RB_BLACK;
            parent->rb_right = tmp2 = sibling->rb_left;
			sibling->rb_left = parent;
            if (tmp2)
                tmp2->rb_parent = parent;
            __rb_rotate_set_parents(parent, sibling, root);
            node = root->rb_node;
            break;
        } else {
            sibling = parent->rb_left;
            if (sibling->rb_color == RB_RED) {
                sibling->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                parent->rb_left = tmp1 = sibling->rb_right;
                tmp1->rb_parent = parent;
                sibling->rb_right = parent;
                __rb_rotate_set_parents(parent, sibling, root);
                sibling = tmp1;
            }
            tmp1 = sibling->rb_left;
            if (!tmp1 || tmp1->rb_color == RB_BLACK) {
                tmp2 = sibling->rb_right;
                if (!tmp2 || tmp2->rb_color == RB_BLACK) {
                    sibling->rb_color = RB_RED;
                    node = parent;
                    parent = node->rb_parent;
                    continue;
                }
                sibling->rb_color = RB_RED;
                tmp2->rb_color = RB_BLACK;
                sibling->rb_right = tmp1 = tmp2->rb_left;
				if (tmp1)
                    tmp1->rb_parent = sibling;
				tmp2->rb_left = sibling;
                sibling->rb_parent = tmp2;
                tmp2->rb_parent = parent;
				parent->rb_left = tmp2;
                tmp1 = sibling;
                sibling = tmp2;
            }
            sibling->rb_color = parent->rb_color;
            parent->rb_color = RB_BLACK;
            tmp1->rb_color = RB_BLACK;
            parent->rb_left = tmp2 = sibling->rb_right;
			sibling->rb_right = parent;
            if (tmp2)
                tmp2->rb_parent = parent;
            __rb_rotate_set_parents(parent, sibling, root);
            node = root->rb_node;
            break;
        }
    }
    if (node)
        node->rb_color = RB_BLACK;
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
    int color;
    struct rb_node *child, *parent;
    if (node->rb_left && node->rb_right) {
        struct rb_node *old = node, *tmp;
        node = node->rb_right;
        while ((tmp = node->rb_left))
            node = tmp;
        node->rb_left = old->rb_left;
        old->rb_left->rb_parent = node;
        child = node->rb_right;
        color = node->rb_color;
        if (node != old->rb_right) {
            node->rb_right = old->rb_right;
            old->rb_right->rb_parent = node;
            parent = node->rb_parent;
            parent->rb_left = child;
            if (child)
                child->rb_parent = parent;
        } else
            parent = node;
        __rb_change_child(old, node, old->rb_parent, root);
        node->rb_parent = old->rb_parent;
        node->rb_color = old->rb_color;
    } else {
        if (node->rb_left == NULL)
            child = node->rb_right;
        else
            child = node->rb_left;
        color = node->rb_color;
        parent = node->rb_parent;
        __rb_change_child(node, child, parent, root);
        if (child)
            child->rb_parent = parent;
    }
    if (color)
        __rb_erase_color(child, parent, root);
}

struct rb_node *rb_first(const struct rb_root *root)
{
    struct rb_node *node = root->rb_node;

    if (node == NULL)
        return NULL;

    while (node->rb_left != NULL)
        node = node->rb_left;

    return node;
}

struct rb_node *rb_last(const struct rb_root *root)
{
    struct rb_node *node = root->rb_node;

    if (node == NULL)
        return NULL;

    while (node->rb_right != NULL)
        node = node->rb_right;

    return node;
}

struct rb_node *rb_prev(const struct rb_node *node)
{
    if (node == NULL)
        return NULL;

    if (node->rb_left != NULL) {
        node = node->rb_left;
        while (node->rb_right)
            node = node->rb_right;
        return (struct rb_node *)node;
    } else {
        struct rb_node *parent;
        while (((parent = node->rb_parent) != NULL) &&
               (node == parent->rb_left))
            node = parent;
        return parent;
    }
}

struct rb_node *rb_next(const struct rb_node *node)
{
    if (node == NULL)
        return NULL;

    if (node->rb_right) {
        node = node->rb_right;
        while (node->rb_left)
            node = node->rb_left;

        return (struct rb_node *)node;

    } else {
        struct rb_node *parent;
        while (((parent =  node->rb_parent) != NULL) &&
               (node == parent->rb_right))
            node = parent;
        return parent;
    }
}
