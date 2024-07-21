#include "avltree.h"

static inline struct avl_node *avl_parent(const struct avl_node *n) {
  return n->avl_parent;
}

static inline int avl_bf(const struct avl_node *n) { return n->avl_bf; }

static inline void avl_set_parent(struct avl_node *n, struct avl_node *p) {
  n->avl_parent = p;
}

static inline void __avl_switch_child(struct avl_node *old,
                                      struct avl_node *new,
                                      struct avl_node *parent,
                                      struct avl_root *root) {
  if (parent) {
    if (old == parent->avl_left)
      parent->avl_left = new;
    else
      parent->avl_right = new;
  } else
    root->avl_node = new;
}

static inline void __avl_rotate_set_parent(struct avl_node *old,
                                           struct avl_node *new,
                                           struct avl_root *root) {
  struct avl_node *parent = old->avl_parent;
  old->avl_parent = new;
  new->avl_parent = parent;
  __avl_switch_child(old, new, parent, root);
}

void avl_insert_rebalance(struct avl_node *node, struct avl_root *root) {
  int bf;
  struct avl_node *parent, *tmp1, *tmp2;
  while ((parent = node->avl_parent)) {
    bf = avl_bf(parent);
    if (node == parent->avl_left) {
      if (bf == AVL_RIGHT_HEAVY) {
        parent->avl_bf = AVL_BALANCED;
        break;
      } else if (bf == AVL_BALANCED) {
        parent->avl_bf = AVL_LEFT_HEAVY;
        node = parent;
        continue;
      } else {
        parent->avl_bf = AVL_BALANCED;
        bf = avl_bf(node);
        node->avl_bf = AVL_BALANCED;
        if (bf == AVL_RIGHT_HEAVY) {
          tmp1 = node->avl_right;
          bf = avl_bf(tmp1);
          tmp1->avl_bf = AVL_BALANCED;
          if (bf == AVL_LEFT_HEAVY)
            parent->avl_bf = AVL_RIGHT_HEAVY;
          else if (bf == AVL_RIGHT_HEAVY)
            node->avl_bf = AVL_LEFT_HEAVY;
          node->avl_right = tmp2 = tmp1->avl_left;
          tmp1->avl_left = node;
          if (tmp2)
            avl_set_parent(tmp2, node);
          avl_set_parent(node, tmp1);
          node = tmp1;
        }
        parent->avl_left = tmp1 = node->avl_right;
        node->avl_right = parent;
        if (tmp1)
          avl_set_parent(tmp1, parent);
        __avl_rotate_set_parent(parent, node, root);
        break;
      }
    } else {
      if (bf == AVL_LEFT_HEAVY) {
        parent->avl_bf = AVL_BALANCED;
        break;
      } else if (bf == AVL_BALANCED) {
        parent->avl_bf = AVL_RIGHT_HEAVY;
        node = parent;
        continue;
      } else {
        parent->avl_bf = AVL_BALANCED;
        bf = avl_bf(node);
        node->avl_bf = AVL_BALANCED;
        if (bf == AVL_LEFT_HEAVY) {
          tmp1 = node->avl_left;
          bf = avl_bf(tmp1);
          tmp1->avl_bf = AVL_BALANCED;
          if (bf == AVL_LEFT_HEAVY)
            node->avl_bf = AVL_RIGHT_HEAVY;
          else if (bf == AVL_RIGHT_HEAVY)
            parent->avl_bf = AVL_LEFT_HEAVY;
          node->avl_left = tmp2 = tmp1->avl_right;
          tmp1->avl_right = node;
          if (tmp2)
            avl_set_parent(tmp2, node);
          avl_set_parent(node, tmp1);
          node = tmp1;
        }
        parent->avl_right = tmp1 = node->avl_left;
        node->avl_left = parent;
        if (tmp1)
          avl_set_parent(tmp1, parent);
        __avl_rotate_set_parent(parent, node, root);
        break;
      }
    }
  }
}

static void __avl_erase_rebalance(struct avl_node *node, struct avl_node *parent,
                              struct avl_root *root) {
  int bf1, bf2;
  struct avl_node *sibling, *tmp1, *tmp2;

  if (parent->avl_left == parent->avl_right) {
    parent->avl_bf = AVL_BALANCED;
    node = parent;
    parent = avl_parent(node);
    if (!parent)
      return;
  }

  do {
    bf1 = avl_bf(parent);
    sibling = parent->avl_right;
    if (node != sibling) {
      if (bf1 == AVL_BALANCED) {
        parent->avl_bf = AVL_RIGHT_HEAVY;
        break;
      } else if (bf1 == AVL_LEFT_HEAVY) {
        parent->avl_bf = AVL_BALANCED;
        node = parent;
        continue;
      } else {
        parent->avl_bf = AVL_BALANCED;
        bf1 = avl_bf(sibling);
        sibling->avl_bf = AVL_BALANCED;
        if (bf1 == AVL_LEFT_HEAVY) {
          tmp1 = sibling->avl_left;
          bf2 = avl_bf(tmp1);
          tmp1->avl_bf = AVL_BALANCED;
          if (bf2 == AVL_RIGHT_HEAVY)
            parent->avl_bf = AVL_LEFT_HEAVY;
          else if (bf2 == AVL_LEFT_HEAVY)
            sibling->avl_bf = AVL_RIGHT_HEAVY;
          sibling->avl_left = tmp2 = tmp1->avl_right;
          tmp1->avl_right = sibling;
          if (tmp2)
            avl_set_parent(tmp2, sibling);
          avl_set_parent(sibling, tmp1);
          sibling = tmp1;
        } else if (bf1 == AVL_BALANCED) {
          parent->avl_bf = AVL_RIGHT_HEAVY;
          sibling->avl_bf = AVL_LEFT_HEAVY;
        }
        parent->avl_right = tmp1 = sibling->avl_left;
        sibling->avl_left = parent;
        if (tmp1)
          avl_set_parent(tmp1, parent);
        __avl_rotate_set_parent(parent, sibling, root);
        if (bf1 == AVL_BALANCED)
          break;
        node = sibling;
        continue;
      }
    } else {
      sibling = parent->avl_left;
      if (bf1 == AVL_BALANCED) {
        parent->avl_bf = AVL_LEFT_HEAVY;
        break;
      } else if (bf1 == AVL_RIGHT_HEAVY) {
        parent->avl_bf = AVL_BALANCED;
        node = parent;
        continue;
      } else {
        parent->avl_bf = AVL_BALANCED;
        bf1 = avl_bf(sibling);
        sibling->avl_bf = AVL_BALANCED;
        if (bf1 == AVL_RIGHT_HEAVY) {
          tmp1 = sibling->avl_right;
          bf2 = avl_bf(tmp1);
          tmp1->avl_bf = AVL_BALANCED;
          if (bf2 == AVL_LEFT_HEAVY)
            parent->avl_bf = AVL_RIGHT_HEAVY;
          else if (bf2 == AVL_RIGHT_HEAVY)
            sibling->avl_bf = AVL_LEFT_HEAVY;
          sibling->avl_right = tmp2 = tmp1->avl_left;
          tmp1->avl_left = sibling;
          if (tmp2)
            avl_set_parent(tmp2, sibling);
          avl_set_parent(sibling, tmp1);
          sibling = tmp1;
        } else if (bf1 == AVL_BALANCED) {
          parent->avl_bf = AVL_LEFT_HEAVY;
          sibling->avl_bf = AVL_RIGHT_HEAVY;
        }
        parent->avl_left = tmp1 = sibling->avl_right;
        sibling->avl_right = parent;
        if (tmp1)
          avl_set_parent(tmp1, parent);
        __avl_rotate_set_parent(parent, sibling, root);
        if (bf1 == AVL_BALANCED)
          break;
        node = sibling;
        continue;
      }
    }
  } while ((parent = node->avl_parent));
}

void avl_erase(struct avl_node *node, struct avl_root *root) {
  struct avl_node *parent, *parent1 = avl_parent(node);
  struct avl_node *child = node->avl_left, *tmp = node->avl_right;
  if (!child || !tmp) {
    tmp = child = (!tmp ? child : tmp);
    parent = parent1;
  } else {
    parent = tmp;
    if (node->avl_bf != AVL_LEFT_HEAVY) {
      while (tmp->avl_left)
        tmp = tmp->avl_left;
      tmp->avl_left = child;
      child->avl_parent = tmp;
      child = tmp->avl_right;
      if (parent != tmp) {
        parent->avl_parent = tmp;
        tmp->avl_right = parent;
        parent = tmp->avl_parent;
        parent->avl_left = child;
      }
    } else {
      tmp = child;
      child = parent;
      parent = tmp;
      while (tmp->avl_right)
        tmp = tmp->avl_right;
      tmp->avl_right = child;
      child->avl_parent = tmp;
      child = tmp->avl_left;
      if (parent != tmp) {
        parent->avl_parent = tmp;
        tmp->avl_left = parent;
        parent = tmp->avl_parent;
        parent->avl_right = child;
      }
    }
    tmp->avl_parent = node->avl_parent;
    tmp->avl_bf = node->avl_bf;
  }
  __avl_switch_child(node, tmp, parent1, root);
  if (child)
    child->avl_parent = parent;
  if (parent)
    __avl_erase_rebalance(child, parent, root);
}
void avl_replace_node(struct avl_node *victim, struct avl_node *new,
                      struct avl_root *root) {
  struct avl_node *parent = avl_parent(victim);

  __avl_switch_child(victim, new, parent, root);
  if (victim->avl_left)
    avl_set_parent(victim->avl_left, new);
  if (victim->avl_right)
    avl_set_parent(victim->avl_right, new);

  *new = *victim;
}

struct avl_node *avl_first(const struct avl_root *root) {
  struct avl_node *node = root->avl_node;
  if (node) {
    while (node->avl_left)
      node = node->avl_left;
  }
  return node;
}

struct avl_node *avl_last(const struct avl_root *root) {
  struct avl_node *node = root->avl_node;
  if (node) {
    while (node->avl_right)
      node = node->avl_right;
  }
  return node;
}

struct avl_node *avl_next(const struct avl_node *node) {
  struct avl_node *parent;

  if (AVL_EMPTY_NODE(node))
    return NULL;

  if (node->avl_right) {
    node = node->avl_right;
    while (node->avl_left)
      node = node->avl_left;
    return (struct avl_node *)node;
  }

  while ((parent = avl_parent(node)) && node == parent->avl_right)
    node = parent;

  return parent;
}

struct avl_node *avl_prev(const struct avl_node *node) {
  struct avl_node *parent;

  if (AVL_EMPTY_NODE(node))
    return NULL;

  if (node->avl_left) {
    node = node->avl_left;
    while (node->avl_right)
      node = node->avl_right;
    return (struct avl_node *)node;
  }

  while ((parent = avl_parent(node)) && node == parent->avl_left)
    node = parent;

  return parent;
