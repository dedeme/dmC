// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "node1_tests.h"
#include "assert.h"

Node1 *n12111 = NULL;

Node1 *node1_tests_mk_node (void) {

  Node1 *n1 = node1_new(opt_empty(), "", arr_new());
  Node1 *n11 =  node1_new(opt_new(n1), "a", arr_new());
  Node1 *n12 =  node1_new(opt_new(n1), "b", arr_new());
  Node1 *n13 =  node1_new(opt_new(n1), "c", arr_new());
  // Arr[Node1]
  Arr *n1a = node1_children(n1);
  arr_push(n1a, n11);
  arr_push(n1a, n12);
  arr_push(n1a, n13);

  Node1 *n111 = node1_new(opt_new(n11), "d", arr_new());
  arr_push(node1_children(n11), n111);

  Node1 *n1111 = node1_new(opt_new(n111), "e", arr_new());
  Node1 *n1112 = node1_new(opt_new(n111), "f", arr_new());
  arr_push(node1_children(n111), n1111);
  arr_push(node1_children(n111), n1112);

  Node1 *n121 = node1_new(opt_new(n12), "g", arr_new());
  arr_push(node1_children(n12), n121);

  Node1 *n1211 = node1_new(opt_new(n121), "d", arr_new());
  Node1 *n1212 = node1_new(opt_new(n121), "h", arr_new());
  arr_push(node1_children(n121), n1211);
  arr_push(node1_children(n121), n1212);

  n12111 = node1_new(opt_new(n1211), "e", arr_new());
  Node1 *n12112 = node1_new(opt_new(n1211), "f", arr_new());
  arr_push(node1_children(n1211), n12111);
  arr_push(node1_children(n1211), n12112);

  Node1 *n131 = node1_new(opt_new(n13), "h", arr_new());
  Node1 *n132 = node1_new(opt_new(n13), "d", arr_new());
  Node1 *n133 = node1_new(opt_new(n13), "f", arr_new());
  arr_push(node1_children(n13), n131);
  arr_push(node1_children(n13), n132);
  arr_push(node1_children(n13), n133);

  Node1 *n1321 = node1_new(opt_new(n132), "e", arr_new());
  Node1 *n1322 = node1_new(opt_new(n132), "f", arr_new());
  arr_push(node1_children(n132), n1321);
  arr_push(node1_children(n132), n1322);

  return n1;
}

void node1_tests (void) {
  puts("Node1 tests:");

  node1_tests_mk_node();

  // puts(node1_to_str(n1));

  Node1 *cy = node1_new(opt_new(n12111), "b", arr_new());
  assert(node1_cyclic(cy));

  cy = node1_new(opt_new(n12111), "h", arr_new());
  assert(!node1_cyclic(cy));

  puts("    Finished");
}

