// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap0_tests.h"
#include "kut/DEFS.h"
#include "heap0.h"
#include "symix.h"
#include "exp.h"

void heap0_tests () {
  puts(">>> heap0:");

  Heap0 *heap = heap0_new();
  TESTI(arr_size(heap0_get_array(heap)), 0);
  assert(heap0_add(heap, symix_add("val"), 33, exp_int(3)));
  assert(!heap0_add(heap, symix_add("val"), 33, exp_int(3)));
  // Heap0Entry
  Arr *a = heap0_get_array(heap);
  TESTI(arr_size(a), 1);
  Heap0Entry *e = opt_get(heap0_get(heap, symix_add("val")));
  TESTI(heap0_entry_nline(e), 33);
  TESTI(exp_get_int(heap0_entry_exp(e)), 3);
  assert(!opt_get(heap0_get(heap, symix_add("va"))));

  puts("  ... Finished");
}
