// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "node2_tests.h"
#include "assert.h"

void node2_tests (void) {
  puts("Node2 tests:");

  Node1 *n1 = node1_tests_mk_node();

  // Arr[Node2]
  Arr *ns = arr_new();
  Node2 *n2 = node1_to_node2(ns, n1, 0);

  assert(str_eq(node2_to_str(n2), node1_to_str(n1)));

  Buf *bf = buf_new();
  EACH(ns, Node2, n)
    buf_add(bf, node2_path(n));
  _EACH
  assert(str_eq("adefbghc", buf_str(bf)));

  node2_sort(ns);
  bf = buf_new();
  EACH(ns, Node2, n)
    buf_add(bf, node2_path(n));
  _EACH
  assert(str_eq("abcgdhef", buf_str(bf)));

  assert(arr_get(node2_children(arr_get(node2_children(n2), 0)), 0) ==
         arr_get(node2_children(arr_get(node2_children(n2), 2)), 1));
  assert(node2_descendent(
    arr_get(node2_children(n2), 0),
    arr_get(node2_children(
      arr_get(node2_children(arr_get(node2_children(n2), 2)), 1)), 0
    )
  ));
  assert(node2_descendent(
    arr_get(node2_children(n2), 0),
    arr_get(node2_children(n2), 0)
  ));
  assert(!node2_descendent(
    arr_get(node2_children(n2), 0),
    arr_get(node2_children(arr_get(node2_children(n2), 2)), 0)
  ));

  puts("    Finished");
}

