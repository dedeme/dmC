// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "flea_tests.h"
#include "Flea.h"
#include "dmc/Dec.h"
#include <assert.h>

void flea_tests() {
  puts("Flea tests");

  puts("    Constructores"); // --------------------------------------

  Flea *f1 = flea_new(str_new("1"));
  assert(str_eq(flea_id(f1), "1"));
  Js *js = flea_to_js_new(f1);
  Flea *f2 = flea_from_js_new(js);
  assert(str_eq(flea_id(f1), flea_id(f2)));
  assert(gen_get(flea_gen(f1), gen_DAYS) == gen_get(flea_gen(f1), gen_DAYS));
  assert(gen_get(flea_gen(f1), gen_BUY) == gen_get(flea_gen(f1), gen_BUY));
  assert(gen_get(flea_gen(f1), gen_SELL) == gen_get(flea_gen(f1), gen_SELL));
  free(js);
  flea_free(f2);
  f2 = flea_mutate_new(f1, str_new("2"));
  assert(!str_eq(flea_id(f1), flea_id(f2)));
  assert(
    gen_get(flea_gen(f1), gen_DAYS) != gen_get(flea_gen(f2), gen_DAYS) ||
    gen_get(flea_gen(f1), gen_BUY) != gen_get(flea_gen(f2), gen_BUY) ||
    gen_get(flea_gen(f1), gen_SELL) != gen_get(flea_gen(f2), gen_SELL)
  );

  flea_free(f1);
  flea_free(f2);

  puts("    Init"); // -----------------------------------------------

  f1 = flea_from_js_new((Js *)"[\"1\",[0,0.1,0.2]]");
  assert(flea_ndays(f1) == 20);
  assert(dec_eq_gap(flea_buy_strip(f1),0.02, 0.00001));
  assert(dec_eq_gap(flea_sell_strip(f1),0.04, 0.00001));

  flea_free(f1);

  puts("    Finished");
}
