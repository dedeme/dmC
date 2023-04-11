// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap_tests.h"
#include "kut/DEFS.h"
#include "heap.h"
#include "heaps.h"
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

  Heaps *heaps1 = heaps_new(heap2);
  Heaps *heaps2 = heaps_add(heaps1, heap);

  TESTI(exp_get_int(opt_get(tp_e1(heaps_get(heaps1, "val")))), 5);
  TESTI(exp_get_int(opt_get(tp_e1(heaps_get(heaps2, "val")))), 3);
  assert(!opt_get(tp_e1(heaps_get(heaps2, "va"))));

  puts("  ... Finished");
}
