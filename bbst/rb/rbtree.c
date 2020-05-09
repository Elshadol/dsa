#include "rbtree.h"

#define RB_IS_BLACK(n) (((n) == NULL) || ((n)->rb_color == RB_BLACK))
#define RB_IS_RED(n) !RB_IS_BLACK(n)

static void __rb_rotate_left(struct rb_node *x, struct rb_root *root)
{
    struct rb_node *y = x->rb_right;

    x->rb_right = y->rb_left;
    if (y->rb_left != NULL)
        y->rb_left->rb_parent = x;

    y->rb_parent = x->rb_parent;
    if (x->rb_parent == NULL)
        root->rb_node = y;
    else if (x == x->rb_parent->rb_left)
        x->rb_parent->rb_left = y;
    else
        x->rb_parent->rb_right = y;

    y->rb_left = x;
    x->rb_parent = y;
}

static void __rb_rotate_right(struct rb_node *x, struct rb_root *root)
{
    struct rb_node *y = x->rb_left;

    x->rb_left = y->rb_right;
    if (y->rb_right != NULL)
        y->rb_right->rb_parent = x;

    y->rb_parent = x->rb_parent;
    if (x->rb_parent == NULL)
        root->rb_node = y;
    else if (x == x->rb_parent->rb_left)
        x->rb_parent->rb_left = y;
    else
        x->rb_parent->rb_right = y;

    y->rb_right = x;
    x->rb_parent = y;
}

static void __rb_rotate_left_right(struct rb_node *a, struct rb_root *root)
{
    struct rb_node *b = a->rb_left;
    struct rb_node *c = b->rb_right;

    a->rb_left = c->rb_right;
    b->rb_right = c->rb_left;
    c->rb_right = a;
    c->rb_left = b;

    c->rb_parent = a->rb_parent;
    a->rb_parent = b->rb_parent = c;

    if (a->rb_left != NULL)
        a->rb_left->rb_parent = a;


    if (b->rb_right != NULL)
        b->rb_right->rb_parent = b;

    if (c->rb_parent == NULL)
        root->rb_node = c;
    else if (c->rb_parent->rb_left == a)
        c->rb_parent->rb_left = c;
    else
        c->rb_parent->rb_right = c;
}

static void __rb_rotate_right_left(struct rb_node *a, struct rb_root *root)
{
    struct rb_node *b = a->rb_right;
    struct rb_node *c = b->rb_left;

    a->rb_right = c->rb_left;
    b->rb_left = c->rb_right;

    c->rb_left = a;
    c->rb_right = b;

    c->rb_parent = a->rb_parent;
    a->rb_parent = b->rb_parent = c;

    if (a->rb_right != NULL)
        a->rb_right->rb_parent = a;

    if (b->rb_left != NULL)
        b->rb_left->rb_parent = b;

    if (c->rb_parent == NULL)
        root->rb_node = c;
    else if (c->rb_parent->rb_left == a)
        c->rb_parent->rb_left = c;
    else
        c->rb_parent->rb_right = c;
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
    struct rb_node *parent;
    while ((parent = node->rb_parent) && (parent->rb_color == RB_RED)) {
        struct rb_node *gparent = parent->rb_parent;
        gparent->rb_color = RB_RED;
        if (parent == gparent->rb_left) {
            struct rb_node *uncle = gparent->rb_right;
            if (RB_IS_RED(uncle)) {
                uncle->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                node = gparent;
            } else {
                if (node == parent->rb_right) {
                    node->rb_color = RB_BLACK;
                    __rb_rotate_left_right(gparent, root);
                } else {
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_right(gparent, root);
                }
                break;
            }
        } else {
            struct rb_node *uncle = gparent->rb_left;
            if (RB_IS_RED(uncle)) {
                uncle->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                node = gparent;
            } else {
                if (node == parent->rb_left) {
                    node->rb_color = RB_BLACK;
                    __rb_rotate_right_left(gparent, root);
                } else {
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_left(gparent, root);
                }
                break;
            }
        }
    }
    root->rb_node->rb_color = RB_BLACK;
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
                             struct rb_root *root)
{
    while (RB_IS_BLACK(node) && (node != root->rb_node)) {
        if (node == parent->rb_left) {
            struct rb_node *sibling = parent->rb_right;
            if (sibling->rb_color == RB_RED) {
                sibling->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                __rb_rotate_left(parent, root);
                sibling = parent->rb_right;
            }
            if (RB_IS_BLACK(sibling->rb_left) &&
                RB_IS_BLACK(sibling->rb_right)) {
                sibling->rb_color = RB_RED;
                node = parent;
                parent = node->rb_parent;
            } else {
                if (RB_IS_RED(sibling->rb_left)) {
                    sibling->rb_left->rb_color = parent->rb_color;
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_right_left(parent, root);
                } else {
                    sibling->rb_color = parent->rb_color;
                    sibling->rb_right->rb_color = RB_BLACK;
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_left(parent, root);
                }
                return;
            }
        } else {
            struct rb_node *sibling = parent->rb_left;
            if (sibling->rb_color == RB_RED) {
                sibling->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                __rb_rotate_right(parent, root);
                sibling = parent->rb_left;
            }
            if (RB_IS_BLACK(sibling->rb_left) &&
                RB_IS_BLACK(sibling->rb_right)) {
                sibling->rb_color = RB_RED;
                node = parent;
                parent = node->rb_parent;
            } else {
                if (RB_IS_RED(sibling->rb_right)) {
                    sibling->rb_right->rb_color = parent->rb_color;
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_left_right(parent, root);
                } else {
                    sibling->rb_color = parent->rb_color;
                    sibling->rb_left->rb_color = RB_BLACK;
                    parent->rb_color = RB_BLACK;
                    __rb_rotate_right(parent, root);
                }
                return;
            }
        }
    }
	if (node != NULL)
		node->rb_color = RB_BLACK;
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
	int color;
    struct rb_node *parent, *child;

    if ((node->rb_left != NULL) && (node->rb_right != NULL)) {
        struct rb_node *replacer = node->rb_right;
        while (replacer->rb_left != NULL)
            replacer = replacer->rb_left;

        child = replacer->rb_right;
		color = replacer->rb_color;
        node->rb_left->rb_parent = replacer;
        replacer->rb_left = node->rb_left;

        if (replacer != node->rb_right) {
            parent = replacer->rb_parent;
            if (child != NULL)
                child->rb_parent = replacer->rb_parent;
            replacer->rb_parent->rb_left = child;
            replacer->rb_right = node->rb_right;
            node->rb_right->rb_parent = replacer;
        } else {
            parent = replacer;
        }

        if (node->rb_parent == NULL)
            root->rb_node = replacer;
        else if (node->rb_parent->rb_left ==  node)
            node->rb_parent->rb_left = replacer;
        else
            node->rb_parent->rb_right = replacer;

        replacer->rb_parent = node->rb_parent;
        replacer->rb_color = node->rb_color;
    } else {
        parent = node->rb_parent;
		color = node->rb_color;
        if (node->rb_left == NULL)
            child = node->rb_right;
        else
            child = node->rb_left;

        if (child != NULL)
            child->rb_parent = parent;

        if (parent == NULL)
            root->rb_node = child;
        else if (parent->rb_left == node)
            parent->rb_left = child;
        else
            parent->rb_right = child;
    }
	if (color == RB_BLACK)
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
