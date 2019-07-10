// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader_dependency_tests.h"
#include "assert.h"
#include "reader.h"

void reader_dependency_tests (void) {
  puts("Reader (reader_dependency) tests:");

  Node1 *n0 = node1_new(opt_empty(), "", arr_new());

  Node1 *n1 = node1_new(opt_new(n0), "a", arr_new());
  arr_push(node1_children(n0), n1);

  assert(!reader_dependency(n1, "f.h", "abc", 2));
  assert(arr_size(node1_children(n1)) == 0);
  assert(!reader_dependency(n1, "f.h", "#include abc", 2));
  assert(arr_size(node1_children(n1)) == 0);
  assert(!reader_dependency(n1, "f.h", "#include <abc", 2));
  assert(arr_size(node1_children(n1)) == 0);

  assert(!reader_dependency(n1, "f.h", "#include <a>", 2));
  assert(arr_size(node1_children(n1)) == 0);
  assert(!reader_dependency(n1, "f.h", "#include \"a\"", 2));
  assert(arr_size(node1_children(n1)) == 0);
  assert(!reader_dependency(n1, "f.h", "#include <b/f.h>", 2));
  assert(arr_size(node1_children(n1)) == 0);
  assert(!reader_dependency(n1, "f.h", "#include \"b/f.h\"", 2));
  assert(arr_size(node1_children(n1)) == 0);

  assert(!reader_dependency(n1, "f.h", "#include <a/b/f.h>", 2));
  assert(arr_size(node1_children(n1)) == 1);
  assert(str_eq(node1_path(arr_get(node1_children(n1), 0)), "a/b"));
  assert(!reader_dependency(n1, "f.h", "#include \"a/b/f.h\"", 2));
  assert(arr_size(node1_children(n1)) == 1);
  assert(str_eq(node1_path(arr_get(node1_children(n1), 0)), "a/b"));

  arr_remove(node1_children(n1), 0);
  assert(!reader_dependency(n1, "f.h", "#include \"a/b/f.h\"", 2));
  assert(arr_size(node1_children(n1)) == 1);
  assert(str_eq(node1_path(arr_get(node1_children(n1), 0)), "a/b"));
  assert(!reader_dependency(n1, "f.h", "#include <a/b/f.h>", 2));
  assert(arr_size(node1_children(n1)) == 1);
  assert(str_eq(node1_path(arr_get(node1_children(n1), 0)), "a/b"));

  /*
  assert(reader_dependency(n1, "f.h", "#include \"a/f.h\"", 2));
  assert(arr_size(node1_children(n1)) == 1);
  assert(reader_dependency(n1, "f.h", "#include <a/f.h>", 2));
  assert(arr_size(node1_children(n1)) == 1);
  */

  puts("    Finished");
}


