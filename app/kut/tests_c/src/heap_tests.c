// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap_tests.h"
#include "kut/DEFS.h"
#include "heap.h"
#include "exp.h"

void heap_tests () {
  puts(">>> heap:");

  Heap *heap = heap_new();
  TESTI(map_size(heap_get(heap)), 0);
  assert(heap_add(heap, "val", exp_int(3)));
  assert(!heap_add(heap, "val", exp_int(3)));
  // <Exp>
  Map *mp = heap_get(heap);
  TESTI(map_size(mp), 1);
  Exp *e = opt_get(map_get(mp, "val"));
  TESTI(exp_get_int(e), 3);
  assert(!opt_get(map_get(mp, "va")));

  Heap *heap2 = heap_new();
  assert(heap_add(heap2, "val", exp_int(5)));

  // <Heap>
  Arr *heaps = arr_new();
  // <Heap>
  Arr *heaps1 = heap_add_to_arr(heap2, heaps);
  // <Heap>
  Arr *heaps2 = heap_add_to_arr(heap, heaps1);
  TESTI(arr_size(heaps), 0);
  TESTI(arr_size(heaps1), 1);
  TESTI(arr_size(heaps2), 2);
  TESTI(exp_get_int(opt_get(heap_get_exp(heaps1, "val"))), 5);
  TESTI(exp_get_int(opt_get(heap_get_exp(heaps2, "val"))), 3);
  assert(!opt_get(heap_get_exp(heaps2, "va")));

  puts("  ... Finished");
}
