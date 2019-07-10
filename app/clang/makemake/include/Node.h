// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef NODE_H
  #define NODE_H

#include "dmc/std.h"

/*--*/

///
typedef struct Node_Node1 Node1;

///
Node1 *node1_new (Opt *parent, char *path, Arr *children);

/// Opt[Node1]
Opt *node1_parent (Node1 *this);

///
char *node1_path (Node1 *this);

/// Arr[Node1]
Arr *node1_children (Node1 *this);

///
typedef struct Node_Node2 Node2;

///
Node2 *node2_new(int depth, char *path, Arr *children);

///
int node2_depth(Node2 *this);

///
char *node2_path(Node2 *this);

/// Arr[Node2]
Arr *node2_children(Node2 *this);

/*--*/

/// Equals to str_eq(node1_parent(n1), node1_parent(n2))
int node1_eq (Node1 *n1, Node1 *n2);

/// Returns '1' if 'n' is cyclical.
int node1_cyclic (Node1 *n);

///
char *node1_to_str (Node1 *n);

/// Generates a Node2 from a Node1
///   ns: Arr[Node2] List with predefined Node2's.
///   n: Node to convert
///   depth: Depth of Node2
///   return: A Node2 with children equals to 'n' children, also converted by
///           this function.
///           If Node2 is equals with "node2_eq" to some element of 'ns', this
///           element will be returned.
Node2 *node1_to_node2 (Arr *ns, Node1 *n, int depth);

/// Equals to str_eq(node1_parent(n1), node1_parent(n2))
int node2_eq (Node2 *n1, Node2 *n2);

/// Changes depth of 'n' and its children if 'node2_depth(n) < value'
void node2_set_depth (Node2 *n, int value);

/// Returns 1 if 'desc' is descendent of 'n'. ('n' is descendent of itself)
int node2_descendent (Node2 *n, Node2 *desc);

///
char *node2_to_str (Node2 *n);

/// Adds 'n' to 'ns'
///   ns: Arr[Node2].
///   n : Node to add. It is added if there is no element equals to 'n' in 'ns'
///       using 'node2_eq'.
///       When 'n' is not added, the previous node depth is set to the depth of
///       'n'
///   returns: Opt[Node2]
///     - If 'ns' contains one element equals to 'n' using 'node2_e1', such
///       element.
///     - Otherwise Opt_empty()
Opt *node2_add (Arr *ns, Node2 *n);

/// Sorts 'ns' by depth. 'ns' is Arr[Node2]
void node2_sort (Arr *ns);

/// Optimizes dependencies of each node of 'ns'
///   ns: Arr[Node1]. It will be modified. It is better if previouly it has
///       been called 'node2_sort'
void node2_optimize (Arr *ns);

#endif
