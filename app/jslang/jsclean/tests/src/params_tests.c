// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "params_tests.h"
#include <assert.h>
#include "data/Params.h"

void params_tests (void) {
  puts("'params' tests:");

  Kv *_ps =params_read(
    3, (char *[]){"", "dm", "www"}
  );
  assert(!*kv_key(_ps));
  Params *ps = kv_value(_ps);
  assert(arr_size(params_roots(ps)) == 1);
  assert(str_eq(arr_get(params_roots(ps), 0), "dm"));
  assert(str_eq(params_target(ps), "www"));

  _ps =params_read(
    3, (char *[]){"", "ddm:wg", "www2"}
  );
  assert(!*kv_key(_ps));
  ps = kv_value(_ps);
  assert(arr_size(params_roots(ps)) == 2);
  assert(str_eq(arr_get(params_roots(ps), 0), "ddm"));
  assert(str_eq(arr_get(params_roots(ps), 1), "wg"));
  assert(str_eq(params_target(ps), "www2"));

  _ps =params_read(
    2, (char *[]){"", "dm"}
  );
  assert(*kv_key(_ps));
  assert(str_starts(kv_key(_ps), "Wrong number of parameters (2)"));

  _ps =params_read(
    3, (char *[]){"", "dm:dm", "www"}
  );
  assert(*kv_key(_ps));
  assert(str_starts(kv_key(_ps), "Source 'dm' is duplicate"));

  puts("    Finished");
}

