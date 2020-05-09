#ifndef RBTREE_INCLUDED
#define RBTREE_INCLUDED
#include <stddef.h>

struct rb_node {
    struct rb_node *rb_parent;
    struct rb_node *rb_left;
    struct rb_node *rb_right;
    int rb_color;
#define RB_RED 1
#define RB_BLACK 0
};

struct rb_root {
    struct rb_node *rb_node;
};

#define rb_is_black(node) (((node) == NULL) || ((node)->rb_color == RB_BLACK))
#define rb_is_red(node) (!rb_is_black(node))

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define RB_ROOT (struct rb_root) { NULL, }
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

void rb_insert_color(struct rb_node *, struct rb_root *);
void rb_erase(struct rb_node *, struct rb_root *);

struct rb_node *rb_next(struct rb_node *);
struct rb_node *rb_prev(struct rb_node *);
struct rb_node *rb_first(struct rb_node *);
struct rb_node *rb_last(struct rb_node *);

void rb_replace_node(struct rb_node *oldnode, struct rb_node *newnode,
                     struct rb_root *root);

static inline void rb_link_node(struct rb_node *node,
                                struct rb_node *parent, struct rb_node **link)
{
    node->rb_parent = parent;
    node->rb_left = NULL;
    node->rb_right = NULL;
    node->rb_color = RB_RED;

    link[0] = node;
}
#endif  //rbtree.h
