#ifndef AVLTREE_INCLUDED
#define AVLTREE_INCLUDED
#include <stddef.h>

struct avl_node {
    struct avl_node *avl_parent;
    struct avl_node *avl_left;
    struct avl_node *avl_right;
    int avl_height;   // use avl_height for rebalance after insertion/deletion
};

struct avl_root {
    struct avl_node *avl_node;
};
#define AVL_ROOT (struct avl_root) { NULL, }

void avl_insert_rebalance(struct avl_node *node, struct avl_root *root);
void avl_erase(struct avl_node *, struct avl_root *);

struct avl_node *avl_first(const struct avl_root *);
struct avl_node *avl_last(const struct avl_root *);
struct avl_node *avl_prev(const struct avl_node *);
struct avl_node *avl_next(const struct avl_node *);

void avl_replace_node(struct avl_node *old,
                      struct avl_node *replacer,
                      struct avl_root *root);

static inline void avl_link_node(struct avl_node *node,
                                 struct avl_node *parent,
                                 struct avl_node **link)
{
    node->avl_parent = parent;
    node->avl_left = NULL;
    node->avl_right = NULL;
    node->avl_height = 0;

    link[0] = node;
}

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define avl_entry(ptr, type, member) container_of(ptr, type, member)

#endif
