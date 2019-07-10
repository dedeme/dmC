// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Node.h"

/* .
Node1
  # Opt[Node1]
  parent: Opt - Node1
  path: char *
  # Arr[Node1]
  children: Arr - Node1
===
Node2
  depth: int
  path: char *
  # Arr[Node2]
  children: Arr - Node2
*/

/*--*/

struct Node_Node1{
  Opt *parent;
  char *path;
  Arr *children;
};

Node1 *node1_new(Opt *parent, char *path, Arr *children) {
  Node1 *this = MALLOC(Node1);
  this->parent = parent;
  this->path = path;
  this->children = children;
  return this;
}

Opt *node1_parent(Node1 *this) {
  return this->parent;
}

char *node1_path(Node1 *this) {
  return this->path;
}

Arr *node1_children(Node1 *this) {
  return this->children;
}

struct Node_Node2{
  int depth;
  char *path;
  Arr *children;
};

Node2 *node2_new(int depth, char *path, Arr *children) {
  Node2 *this = MALLOC(Node2);
  this->depth = depth;
  this->path = path;
  this->children = children;
  return this;
}

int node2_depth(Node2 *this) {
  return this->depth;
}

char *node2_path(Node2 *this) {
  return this->path;
}

Arr *node2_children(Node2 *this) {
  return this->children;
}

/*--*/

int node1_eq (Node1 *n1, Node1 *n2) {
  return str_eq(n1->path, n2->path);
}

/// Returns '1' if 'n' is cyclical.
int node1_cyclic (Node1 *n) {
  char *path = n->path;
  int duplicate (Node1 *n) { return str_eq(path, n->path); }
  for(;;) {
    Node1 *p = opt_nget(n->parent);
    if (!p) return 0;
    if (duplicate(p)) return 1;
    n = p;
  }
}

char *node1_to_str (Node1 *n) {
  void to_str (Buf *bf, Node1 *n, char *tab) {
    buf_add(bf, str_f("%s%s\n", tab, node1_path(n)));
    tab = str_cat(tab, "  ", NULL);
    EACH(node1_children(n), Node1, ch)
      to_str(bf, ch, tab);
    _EACH
  }
  Buf *bf = buf_new();
  to_str(bf, n, "");
  return buf_to_str(bf);
}

Node2 *node1_to_node2 (Arr *ns, Node1 *n, int depth) {
  Node2 *n0 = node2_new(depth, n->path, arr_new());
  Node2 *n1 = opt_nget(node2_add(ns, n0));
  if (n1) {
    return n1;
  }

  ++depth;
  // Arr[Node2]
  Arr *chs = node2_children(n0);
  EACH(node1_children(n), Node1, ch)
    arr_push(chs, node1_to_node2(ns, ch, depth));
  _EACH
  return n0;
}

int node2_eq (Node2 *n1, Node2 *n2) {
  return str_eq(n1->path, n2->path);
}

char *node2_to_str (Node2 *n) {
  void to_str (Buf *bf, Node2 *n, char *tab) {
    buf_add(bf, str_f("%s%s\n", tab, node2_path(n)));
    tab = str_cat(tab, "  ", NULL);
    EACH(node2_children(n), Node2, ch)
      to_str(bf, ch, tab);
    _EACH
  }
  Buf *bf = buf_new();
  to_str(bf, n, "");
  return buf_to_str(bf);
}

/// Changes depth of 'n' and its children if 'node2_depth(n) < value'
void node2_set_depth (Node2 *n, int value) {
  if (n->depth < value) {
    n->depth = value;
    ++value;
    EACH(n->children, Node2, ch)
      node2_set_depth(ch, value);
    _EACH
  }
}

int node2_descendent (Node2 *n, Node2 *desc) {
  if (n == desc) return 1;
  EACH(node2_children(n), Node2, ch)
    if (node2_descendent(ch, desc)) return 1;
  _EACH
  return 0;
}

Opt *node2_add (Arr *ns, Node2 *n) {
  EACH(ns, Node2, node)
    if (node2_eq(node, n)) {
      node2_set_depth(node, n->depth);
      return opt_new(node);
    }
  _EACH
  arr_push(ns, n);
  return opt_empty();
}

void node2_sort (Arr *ns) {
  int fn (Node2 *n1, Node2 *n2) { return n1->depth > n2->depth; }
  arr_sort(ns, (FCMP)fn);
}

void node2_optimize (Arr *ns) {
  void optimize (Node2 *n) {
    // Arr[Node2]
    Arr *new = arr_new();
    // Arr[Node2]
    Arr *old = node2_children(n);
    int size = arr_size(old);
    EACH_IX(old, Node2, ch, ix)
      int duplicate = 0;
      EACH(new, Node2, chn)
        if (node2_descendent(chn, ch)) {
          duplicate = 1;
          break;
        }
      _EACH
      if (duplicate) continue;
      RANGE(i, ix + 1, size)
        if (node2_descendent(arr_get(old, i), ch)) {
          duplicate = 1;
          break;
        }
      _RANGE
      if (duplicate) continue;
      arr_push(new, ch);
    _EACH
    arr_clear(old);
    arr_cat(old, new);
  }

  EACHR(ns, Node2, n)
    optimize(n);
  _EACH
}
