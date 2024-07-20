#ifndef AVLTREE_INCLUDED
#define AVLTREE_INCLUDED
#include <stdlib.h>

struct avl_node {
  struct avl_node *avl_parent;
  struct avl_node *avl_left;
  struct avl_node *avl_right;
  int avl_bf;
#define AVL_BALANCED 0
#define AVL_LEFT_HEAVY 1
#define AVL_RIGHT_HEAVY 2
};

struct avl_root {
  struct avl_node *avl_node;
};

void avl_insert_rebalance(struct avl_node *, struct avl_root *);
void avl_erase(struct avl_node *, struct avl_root *);

/* Find logical next and previous nodes in a tree */
struct avl_node *avl_next(const struct avl_node *);
struct avl_node *avl_prev(const struct avl_node *);
struct avl_node *avl_first(const struct avl_root *);
struct avl_node *avl_last(const struct avl_root *);

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const typeof(((type *)0)->member) *__mptr = (ptr);                         \
    (type *)((char *)__mptr - offsetof(type, member));                         \
  })
#endif

#define avl_entry(ptr, type, member) container_of(ptr, type, member)

#define AVL_ROOT                                                               \
  (struct avl_root) { NULL, }

#define AVL_EMPTY_NODE(node) \
	((node)->avl_parent == (node))
#define AVL_CLEAR_NODE(node) \
	((node)->avl_parent = (node))

void avl_replace_node(struct avl_node *victim, struct avl_node *replacer,
                      struct avl_root *root);

static inline void avl_link_node(struct avl_node *node, struct avl_node *parent,
                                 struct avl_node **avl_link) {
  node->avl_parent = parent;
  node->avl_left = node->avl_right = NULL;
  node->avl_bf = AVL_BALANCED;

  *avl_link = node;
}
#endif
