// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap0_tests.h"
#include "kut/DEFS.h"
#include "heap0.h"
#include "exp.h"

void heap0_tests () {
  puts(">>> heap0:");

  Heap0 *heap = heap0_new();
  TESTI(map_size(heap0_get(heap)), 0);
  assert(heap0_add(heap, "val", 33, exp_int(3)));
  assert(!heap0_add(heap, "val", 33, exp_int(3)));
  // Map<Heap0Entry>
  Map *mp = heap0_get(heap);
  TESTI(map_size(mp), 1);
  Heap0Entry *e = opt_get(map_get(mp, "val"));
  TESTI(heap0_entry_nline(e), 33);
  TESTI(exp_get_int(heap0_entry_exp(e)), 3);
  assert(!opt_get(map_get(mp, "va")));

  puts("  ... Finished");
}
