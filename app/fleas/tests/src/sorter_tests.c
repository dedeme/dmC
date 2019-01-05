// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "sorter_tests.h"
#include "Sorter.h"
#include <assert.h>

void sorter_tests() {
  puts("Sorter tests");

  Sorter *s = sorter_new(3, free);
  Arr *a = sorter_free_new(s);
  assert (arr_size(a) == 0);
  arr_free(a);

  s = sorter_new(2, free);
  sorter_add(s, str_new("b"), 3);
  sorter_add(s, str_new("c"), 1);
  sorter_add(s, str_new("a"), 2);
  a = sorter_free_new(s);
  assert(arr_size(a) == 2);
  assert(str_eq(arr_get(a, 0), "a"));
  assert(str_eq(arr_get(a, 1), "b"));
  arr_free(a);

  puts("    Finished");
}

