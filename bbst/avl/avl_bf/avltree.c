#include "avltree.h"
#include <assert.h>
#include <stdlib.h>

#define REPLACER_CHOOSE_SIDE 0

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
        while (node->avl_right != NULL)
            node = node->avl_right;

        return (struct avl_node *)node;
    } else {
        struct avl_node *parent = node->avl_parent;
        while ((parent != NULL) && (node == parent->avl_left)) {
            node = parent;
            parent = parent->avl_parent;
        }
        return parent;
    }
}

struct avl_node *avl_next(const struct avl_node *node)
{
    if (node == NULL)
        return NULL;

    if (node->avl_right != NULL) {
        node = node->avl_right;

        while (node->avl_left != NULL)
            node = node->avl_left;

        return (struct avl_node *)node;
    } else {
        struct avl_node *parent = node->avl_parent;

        while ((parent != NULL) && (node == parent->avl_right)) {
            node = parent;
            parent = parent->avl_parent;
        }
        return parent;
    }
}

static void  __avl_rotate_left(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *right = node->avl_right;

    node->avl_right = right->avl_left;
    right->avl_left = node;

    right->avl_parent = node->avl_parent;
    node->avl_parent = right;

    if (node->avl_right != NULL)
        node->avl_right->avl_parent = node;

    if (right->avl_parent == NULL) // right now is root
        root->avl_node = right;
    else if (right->avl_parent->avl_left == node)
        right->avl_parent->avl_left = right;
    else
        right->avl_parent->avl_right = right;

    //reset balance factor
    if (right->avl_bf == AVL_RIGHT_HEAVY) {
        right->avl_bf = AVL_BALANCED;
        node->avl_bf = AVL_BALANCED;
    } else {    // this doesn't happpen during insertion rebalance
        right->avl_bf = AVL_LEFT_HEAVY;
        node->avl_bf = AVL_RIGHT_HEAVY;
    }
}

static inline void __avl_rotate_right(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *left = node->avl_left;

    node->avl_left = left->avl_right;
    left->avl_right = node;

    left->avl_parent = node->avl_parent;
    node->avl_parent = left;

    if (node->avl_left != NULL)
        node->avl_left->avl_parent = node;

    if (left->avl_parent == NULL)
        root->avl_node = left;
    else if (left->avl_parent->avl_left == node)
        left->avl_parent->avl_left = left;
    else
        left->avl_parent->avl_right = left;

    if (left->avl_bf == AVL_LEFT_HEAVY) {
        left->avl_bf = AVL_BALANCED;
        node->avl_bf = AVL_BALANCED;
    } else {
        left->avl_bf = AVL_RIGHT_HEAVY;
        node->avl_bf = AVL_LEFT_HEAVY;
    }
}

static void __avl_rotate_left_right(struct avl_node *a,
                                    struct avl_root *root)
{
    /*        |                               |
     *        a                               c
     *      /   \                           /   \
     *    b(h+2) T3(h)   ==>              b       a
     *  /   \                           /   \   /   \
     *T0(h)   c(h+1)                   T0(h) T1 T2   T3(h)
     *      /  \
     *    T1    T2
     *
     */
    struct avl_node *b = a->avl_left;
    struct avl_node *c = b->avl_right;

    // switch
    a->avl_left = c->avl_right;
    b->avl_right = c->avl_left;
    c->avl_right = a;
    c->avl_left = b;

    // set parents
    c->avl_parent = a->avl_parent;
    a->avl_parent = c;
    b->avl_parent = c;

    if (a->avl_left != NULL)
        a->avl_left->avl_parent = a;

    if (b->avl_right != NULL)
        b->avl_right->avl_parent = b;

    if (c->avl_parent != NULL) {
        if (c->avl_parent->avl_left == a)
            c->avl_parent->avl_left = c;
        else
            c->avl_parent->avl_right = c;
    } else {
        root->avl_node = c;
    }

    //reset balance factors
    switch (c->avl_bf) {
    case AVL_RIGHT_HEAVY:
        a->avl_bf = AVL_BALANCED;
        b->avl_bf = AVL_LEFT_HEAVY;
        break;

    case AVL_BALANCED:
        a->avl_bf = AVL_BALANCED;
        b->avl_bf = AVL_BALANCED;
        break;

    case AVL_LEFT_HEAVY:
        a->avl_bf = AVL_RIGHT_HEAVY;
        b->avl_bf = AVL_BALANCED;
        break;

    default:
        assert(0);   // should nerver reach
    }
    c->avl_bf = AVL_BALANCED;
}

static void __avl_rotate_right_left(struct avl_node *a,
                                    struct avl_root *root)
{
    struct avl_node *b = a->avl_right;
    struct avl_node *c = b->avl_left;

    a->avl_right = c->avl_left;
    b->avl_left = c->avl_right;

    c->avl_left = a;
    c->avl_right = b;

    c->avl_parent = a->avl_parent;
    a->avl_parent = c;
    b->avl_parent = c;

    if (a->avl_right != NULL)
        a->avl_right->avl_parent = a;

    if (b->avl_left != NULL)
        b->avl_left->avl_parent = b;

    if (c->avl_parent != NULL) {
        if (c->avl_parent->avl_left == a)
            c->avl_parent->avl_left = c;
        else
            c->avl_parent->avl_right = c;
    } else
        root->avl_node = c;

    switch (c->avl_bf) {
    case AVL_LEFT_HEAVY:
        a->avl_bf = AVL_BALANCED;
        b->avl_bf = AVL_RIGHT_HEAVY;
        break;

    case AVL_BALANCED:
        a->avl_bf = AVL_BALANCED;
        b->avl_bf = AVL_BALANCED;
        break;

    case AVL_RIGHT_HEAVY:
        a->avl_bf = AVL_LEFT_HEAVY;
        b->avl_bf = AVL_BALANCED;
        break;

    default:
        assert(0);
    }
    c->avl_bf = AVL_BALANCED;
}

void avl_insert_rebalance(struct avl_node *node,
                          struct avl_root *root)
{
    while (node != root->avl_node) {
        struct avl_node *parent = node->avl_parent;

        switch (parent->avl_bf) {
        case AVL_RIGHT_HEAVY:
            if (node == parent->avl_left) {
                parent->avl_bf = AVL_BALANCED;
            } else {
                if (node->avl_bf == AVL_LEFT_HEAVY)
                    __avl_rotate_right_left(parent, root);
                else
                    __avl_rotate_left(parent, root);
            }
            return;

        case AVL_BALANCED:
            parent->avl_bf =
                (node == parent->avl_left) ? AVL_LEFT_HEAVY : AVL_RIGHT_HEAVY;
            node = parent;
            break;

        case AVL_LEFT_HEAVY:
            if (node == parent->avl_left) {
                if (node->avl_bf == AVL_LEFT_HEAVY)
                    __avl_rotate_right(parent, root);
                else
                    __avl_rotate_left_right(parent, root);
            } else {
                parent->avl_bf = AVL_BALANCED;
            }
            return;

        default:
            assert(0);
        }
    }
}

static void __avl_erase_rebalance(struct avl_node *node,
                                  struct avl_node *parent,
                                  struct avl_root *root)
{
    while (node != root->avl_node) {
        switch (parent->avl_bf) {
        case AVL_RIGHT_HEAVY:
            if (node == parent->avl_right) {
                parent->avl_bf = AVL_BALANCED;
                node = parent;
                parent = parent->avl_parent;
            } else {
                struct avl_node *right = parent->avl_right;
                assert(right);

                if (right->avl_bf == AVL_LEFT_HEAVY) {
                    assert(right->avl_left);
                    __avl_rotate_right_left(parent, root);
                } else {
                    __avl_rotate_left(parent, root);
                }

                node = parent->avl_parent;
                parent = parent->avl_parent->avl_parent;

                // if the bf of node Changes from -1 to 1,
                if (node->avl_bf == AVL_LEFT_HEAVY)
                    return;
            }
            break;

        case AVL_BALANCED:
            // the height of node didnot Change, just return
            parent->avl_bf = (node == parent->avl_left) ?
                             AVL_RIGHT_HEAVY : AVL_LEFT_HEAVY;
            return;

        case AVL_LEFT_HEAVY:
            if (node == parent->avl_left) {
                parent->avl_bf = 0;
                node = parent;
                parent = parent->avl_parent;
            } else {
                struct avl_node *left = parent->avl_left;
                assert(left);
                if (left->avl_bf == AVL_RIGHT_HEAVY) {
                    assert(left->avl_right);
                    __avl_rotate_left_right(parent, root);
                } else {
                    __avl_rotate_right(parent, root);
                }

                node = parent->avl_parent;
                parent = parent->avl_parent->avl_parent;

                if (node->avl_bf == AVL_RIGHT_HEAVY)
                    return;
            }
            break;

        default:
            assert(0);
        }
    }
}

void avl_erase(struct avl_node *node, struct avl_root *root)
{
    struct avl_node *parent;
    struct avl_node *child;

    // If node has both non-null children, the go down the taller subtree
    if ((node->avl_left != NULL) && (node->avl_right != NULL)) {
#if REPLACER_CHOOSE_SIDE
        if (node->avl_bf == AVL_LEFT_HEAVY) {
            replacer = node->avl_left;
            while (replacer->avl_right != NULL)
                replacer = replacer->avl_right;

            child = replacer->avl_left;

            node->avl_right->avl_parent = replacer;
            replacer->avl_right = node->avl_right;

            if (replacer != node->avl_left) {
                parent = replacer->avl_parent;
                if (child != NULL)
                    child->avl_parent = replacer->avl_parent;

                replacer->avl_parent->avl_right = child;
                replacer->avl_left = node->avl_left;
                node->avl_left->avl_parent = replacer;
            } else {
                parent = replacer;
            }
        }
       else {
#endif
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
#if REPLACER_CHOOSE_SIDE
        }
#endif
                // replace replacer with its child, may be null
        if (node->avl_parent == NULL)
            root->avl_node = replacer;
        else if (node->avl_parent->avl_left ==  node)
            node->avl_parent->avl_left = replacer;
        else
            node->avl_parent->avl_right = replacer;

        replacer->avl_parent = node->avl_parent;
        replacer->avl_bf = node->avl_bf;

    } else {
        if (node->avl_left == NULL) // node has at most one non-null child
            child = node->avl_right;
        else
            child = node->avl_left;

        parent = node->avl_parent;

        if (child != NULL)
            child->avl_parent = parent;

        if (parent != NULL) {
            if (parent->avl_left == node)
                parent->avl_left = child;
            else
                parent->avl_right = child;
        } else
            root->avl_node = child;
    }

    __avl_erase_rebalance(child, parent, root);
}

void avl_replace_node(struct avl_node *victim, struct avl_node *replacer,
                      struct avl_root *root)
{
    struct avl_node *parent = victim->avl_parent;

    if (parent == NULL)
        root->avl_node = replacer;
    else if (parent->avl_left == victim)
        parent->avl_left = replacer;
    else
        parent->avl_right = replacer;

    if (victim->avl_left != NULL)
        victim->avl_left->avl_parent = replacer;

    if (victim->avl_right != NULL)
        victim->avl_right->avl_parent = replacer;

    *replacer = *victim;
}
