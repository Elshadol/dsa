#include "rbtree.h"
#include <stdlib.h>

#define rb_set_color(n, c) ((n)->rb_color = (c))
#define rb_set_red(node) ((node)->rb_color = RB_RED)
#define rb_set_black(node) ((node)->rb_color = RB_BLACK)
#define rb_parent(n) ((n)->rb_parent)
#define rb_color(n) ((n)->rb_color)

static inline void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;

        node->rb_right = right->rb_left;
	if (rb_has_lchild(right)) {
		right->rb_left->rb_parent = node;
	}

	if (rb_has_parent(node)) {
		if (rb_is_lchild(node))
			node->rb_parent->rb_left = right;
		else
			node->rb_parent->rb_right = right;
		right->rb_parent = node->rb_parent;
	} else {
		root->rb_node = right;
	}

	right->rb_left = node;
	node->rb_parent = right;
}

static inline void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;

	if (rb_has_rchild(left)) {
		node->rb_left = left->rb_right;
		left->rb_left->rb_parent = node;
	}

	if (rb_has_parent(node)) {
		if (rb_is_lchild(node))
			node->rb_parent->rb_left = left;
		else
			node->rb_parent->rb_right = left;
		left->rb_parent = node->rb_parent;
	} else {
		root->rb_node = left;
	}

	left->rb_right = node;
	node->rb_parent = left;
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent, *uncle;
	while ((parent = node->rb_parent) && rb_is_red(parent)) {
		gparent = parent->rb_parent;
		if (parent == gparent->rb_left) {
			uncle = gparent->rb_right;
			if (rb_is_red(uncle)) {
				uncle->rb_color = RB_BLACK;
				parent->rb_color = RB_BLACK;
				gparent->rb_color = RB_RED;
				node = gparent;
			} else {
                if (node == parent->rb_right) {
                    __rb_rotate_left(parent, root);
                    struct rb_node *temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                __rb_rotate_right(gparent, root);
            }
        } else {
            uncle = gparent->rb_left;
            if (rb_is_red(uncle)) {
                uncle->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                node = gparent;
            } else {
                if (node == parent->rb_left) {
                    __rb_rotate_right(parent, root);
                    struct rb_node *temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                __rb_rotate_left(gparent, root);
            }
        }
    }
	root->rb_node->rb_color = RB_BLACK;
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
                 struct rb_root *root)
{
    struct rb_node *other;

    while ((!node || rb_is_black(node)) && node != root->rb_node)
    {
        if (parent->rb_left == node)
        {
            other = parent->rb_right;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                __rb_rotate_left(parent, root);
                other = parent->rb_right;
            }
            if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                (!other->rb_right || rb_is_black(other->rb_right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->rb_right || rb_is_black(other->rb_right))
                {
                    rb_set_black(other->rb_left);
                    rb_set_red(other);
                    __rb_rotate_right(other, root);
                    other = parent->rb_right;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->rb_right);
                __rb_rotate_left(parent, root);
                node = root->rb_node;
                break;
            }
        }
        else
        {
            other = parent->rb_left;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                __rb_rotate_right(parent, root);
                other = parent->rb_left;
            }
            if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                (!other->rb_right || rb_is_black(other->rb_right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->rb_left || rb_is_black(other->rb_left))
                {
                    rb_set_black(other->rb_right);
                    rb_set_red(other);
                    __rb_rotate_left(other, root);
                    other = parent->rb_left;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->rb_left);
                __rb_rotate_right(parent, root);
                node = root->rb_node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

	if (rb_has_lchild(node) && rb_has_rchild(node)) {

        struct rb_node *replacer = node;

        replacer = replacer->rb_right;
        while (replacer->rb_left != NULL)
            replacer = replacer->rb_left;

        child = replacer->rb_right;
        color = replacer->rb_color;

        node->rb_left->rb_parent = replacer;
        replacer->rb_left = node->rb_left;

        if (replacer == node->rb_right) {
            parent = replacer;
        } else {
            parent = replacer->rb_parent;

            if (child != NULL)
                child->rb_parent = replacer->rb_parent;

            replacer->rb_parent->rb_left = child;
            replacer->rb_right = node->rb_right;
            node->rb_right->rb_parent = replacer;
        }

        if (rb_is_root(node)) {
            root->rb_node = replacer;
        } else {
            if (node->rb_parent->rb_left == node)
                node->rb_parent->rb_left = replacer;
            else
                node->rb_parent->rb_right = replacer;
        }
        replacer->rb_parent = node->rb_parent;
    } else {
        if (node->rb_left == NULL)
            child = node->rb_right;
        else
            child = node->rb_left;
        parent = node->rb_parent;

        if (child)
            child->rb_parent = parent;

        if (parent) {
            if (parent->rb_left == node)
                parent->rb_left = child;
            else
                parent->rb_right = child;
        } else {
            root->rb_node = child;
        }
    }

    if (color == RB_BLACK)
        __rb_erase_color(child, parent, root);
}

struct rb_node *rb_first(const struct rb_root *root)
{
    struct rb_node    *n;

    n = root->rb_node;
    if (!n)
        return NULL;
    while (n->rb_left)
        n = n->rb_left;
    return n;
}

struct rb_node *rb_last(const struct rb_root *root)
{
    struct rb_node    *n;

    n = root->rb_node;
    if (!n)
        return NULL;
    while (n->rb_right)
        n = n->rb_right;
    return n;
}

struct rb_node *rb_next(const struct rb_node *node)
{
    struct rb_node *parent;

    if (node == NULL)
        return NULL;

    /* If we have a right-hand child, go down and then left as far
       as we can. */
    if (node->rb_right) {
        node = node->rb_right;
        while (node->rb_left)
            node=node->rb_left;
        return (struct rb_node *)node;
    }

    /* No right-hand children.  Everything down and left is
       smaller than us, so any 'next' node must be in the general
       direction of our parent. Go up the tree; any time the
       ancestor is a right-hand child of its parent, keep going
       up. First time it's a left-hand child of its parent, said
       parent is our 'next' node. */
    while ((parent = node->rb_parent) && node == parent->rb_right)
        node = parent;

    return parent;
}

struct rb_node *rb_prev(const struct rb_node *node)
{
    struct rb_node *parent;

    if (node == NULL)
        return NULL;

    /* If we have a left-hand child, go down and then right as far
       as we can. */
    if (node->rb_left) {
        node = node->rb_left;
        while (node->rb_right)
            node=node->rb_right;
        return (struct rb_node *)node;
    }

    /* No left-hand children. Go up till we find an ancestor which
       is a right-hand child of its parent */
    while ((parent = node->rb_parent) && node == parent->rb_left)
        node = parent;

    return parent;
}
