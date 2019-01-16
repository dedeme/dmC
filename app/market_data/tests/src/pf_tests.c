// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pf_tests.h"
#include "pf.h"
#include <assert.h>

void pf_tests() {
  puts("Pf tests:");

  // Map[PfValue]
  Map *pf = pf_read_new();

  // Arr[char]
  Varr *keys = map_keys_new(pf);
  EACH(keys, char, k)
    PfValue *v = map_get_null(pf, k);
    printf("%s -> [%d, %f]\n", k, pfValue_stocks(v), pfValue_price(v));
  _EACH

  varr_free(keys);
  map_free(pf);
  puts("    Finished");
}
