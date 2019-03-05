// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "gen_tests.h"
#include "assert.h"
#include "Gen.h"
void gen_tests(void) {
  puts("Gen tests:");

  // Arr*[Darr]
  Arr *gens = arr_new((FPROC)gen_free);

  REPEAT(100)
    Gen *g = gen_new(4);
    arr_push(gens, gen_mutate_new(g));
    gen_free(g);
  _REPEAT

  RANGE0(i, 100)
    Gen *g = arr_get(gens, i);
    Js *js = gen_to_js_new(g);
    Gen *g2 = gen_from_js_new(js);
    Js *js2 = gen_to_js_new(g2);
    assert(gen_eq(g, g2));
    assert(str_eq((char *)js, (char *)js2));
    free(js);
    gen_free(g2);
    free(js2);
    RANGE0(j, 100)
      assert((i == j) == (gen_eq(g, arr_get(gens, j))));
    _RANGE
  _RANGE

  puts("Finished");
}
