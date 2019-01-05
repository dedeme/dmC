// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "gen_tests.h"
#include "Gen.h"
#include <assert.h>

void gen_tests() {
  puts("Gen tests");

  Gen *g1 = gen_new();
  Js *js = gen_to_js_new(g1);
  assert(str_eq((char *)js, "[0.500000,0.500000,0.500000]"));
  Gen *g2 = gen_from_js_new(js);
  assert(gen_get(g1, gen_DAYS) == gen_get(g2, gen_DAYS));
  assert(gen_get(g1, gen_BUY) == gen_get(g2, gen_BUY));
  assert(gen_get(g1, gen_SELL) == gen_get(g2, gen_SELL));
  assert(gen_proximity(g1, g2) == 1);
  free(js);
  free(g2);
  free(g1);

  REPEAT(100)
    g1 = gen_new();
    g2 = gen_mutate_new(g1);
    free(g1);
    assert(gen_get(g2, gen_DAYS) >= 0 && gen_get(g2, gen_DAYS) < 1);
    assert(gen_get(g2, gen_BUY) >= 0 && gen_get(g2, gen_BUY) < 1);
    assert(gen_get(g2, gen_SELL) >= 0 && gen_get(g2, gen_SELL) < 1);
    Js *js2 = gen_to_js_new(g2);
    g1 = gen_from_js_new(js2);
    Js *js1 = gen_to_js_new(g1);
    assert(str_eq((char *)js1, (char *)js2));

    free(js1);
    free(js2);
    free(g1);
    free(g2);
  _REPEAT

  puts("    Finished");
}
