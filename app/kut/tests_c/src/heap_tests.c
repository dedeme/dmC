// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap_tests.h"
#include "kut/DEFS.h"
#include "symix.h"
#include "heap.h"
#include "heaps.h"
#include "exp.h"

void heap_tests () {
  puts(">>> heap:");

  Heap *heap = heap_new();
  TESTI(arr_size(heap_get_array(heap)), 0);
  assert(heap_add(heap, symix_add("val"), exp_int(3)));
  assert(!heap_add(heap, symix_add("val"), exp_int(3)));
  // <Exp>
  Arr *a = heap_get_array(heap);
  TESTI(arr_size(a), 1);
  HeapEntry *e = opt_get(heap_get(heap, symix_add("val")));
  TESTI(exp_get_int(heap_entry_exp(e)), 3);
  assert(!opt_get(heap_get(heap, symix_add("va"))));

  Heap *heap2 = heap_new();
  assert(heap_add(heap2, symix_add("val"), exp_int(5)));

  Heaps *heaps1 = heaps_new(heap2);
  Heaps *heaps2 = heaps_add(heaps1, heap);

  TESTI(exp_get_int(opt_get(tp_e1(heaps_get(heaps1, symix_add("val"))))), 5);
  TESTI(exp_get_int(opt_get(tp_e1(heaps_get(heaps2, symix_add("val"))))), 3);
  assert(!opt_get(tp_e1(heaps_get(heaps2, symix_add("va")))));

  puts("  ... Finished");
}
