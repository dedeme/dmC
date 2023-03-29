// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "module_tests.h"
#include "kut/DEFS.h"
#include "module.h"
#include "modules.h"

void module_tests () {
  puts(">>> module:");

  Module *md = module_new(map_new(), heap0_new(), arr_new());
  TESTI(map_size(module_get_imports(md)), 0);
  TESTI(map_size(heap_get(module_get_heap(md))), 0);
  TESTI(map_size(heap0_get(module_get_heap0(md))), 0);
  TESTI(arr_size(module_get_code(md)), 0);

  modules_add(1);
  assert(!opt_get(modules_get(0)));
  //modules_get_ok(0);
  assert(opt_get(modules_get(1)));
  assert(!opt_get(opt_get(modules_get(1))));
  assert(modules_get_ok(1));
  assert(!opt_get(modules_get_ok(1)));
  modules_set(0, md);
  assert(opt_get(modules_get(0)));
  assert(opt_get(opt_get(modules_get(0))));
  assert(modules_get_ok(0));
  assert(opt_get(modules_get_ok(0)));

  TESTI(arr_size(modules_get_array()), 2);

  puts("  ... Finished");
}
