// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/WmaCalc2.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void wmacalc2_test() {
  printf("WmaCalc2 test\n");

  double inc;

  WmaCalc2 *m0 = wmacalc2_new(3, 5, 7);

  assert(!wmacalc2_add(&inc, m0, 1.5));
  assert(eq(wmacalc2_short_sum(m0), 1.5));
  assert(eq(wmacalc2_long_sum(m0), 1.5));

  assert(!wmacalc2_add(&inc, m0, 2));
  assert(eq(wmacalc2_short_sum(m0), 3.5));
  assert(eq(wmacalc2_long_sum(m0), 3.5));

  assert(!wmacalc2_add(&inc, m0, 3));
  assert(eq(wmacalc2_short_sum(m0), 6.5));
  assert(eq(wmacalc2_long_sum(m0), 6.5));

  assert(!wmacalc2_add(&inc, m0, 6));
  assert(eq(wmacalc2_short_sum(m0), 10.333333333333332));
  assert(eq(wmacalc2_long_sum(m0), 12.5));

  assert(wmacalc2_add(&inc, m0, 2));
  assert(eq(wmacalc2_short_sum(m0), 8.888888888888888));
  assert(eq(wmacalc2_long_sum(m0), 14.5));
  assert(eq(inc, 0.021711366538952566));

  assert(wmacalc2_add(&inc, m0, 4));
  assert(eq(wmacalc2_short_sum(m0), 9.925925925925924));
  assert(eq(wmacalc2_long_sum(m0), 15.6));
  assert(eq(inc, 0.060462171573282375));

  assert(wmacalc2_add(&inc, m0, 5));
  assert(eq(wmacalc2_short_sum(m0), 11.617283950617281));
  assert(eq(wmacalc2_long_sum(m0), 17.48));
  assert(eq(inc, 0.10767390833498078));

  m0 = wmacalc2_new(3, 5, 8);

  assert(!wmacalc2_add(&inc, m0, -1.5));
  assert(eq(wmacalc2_short_sum(m0), 0));
  assert(eq(wmacalc2_long_sum(m0), 0));

  assert(!wmacalc2_add(&inc, m0, 2));
  assert(eq(wmacalc2_short_sum(m0), 2));
  assert(eq(wmacalc2_long_sum(m0), 2));

  assert(!wmacalc2_add(&inc, m0, 3));
  assert(eq(wmacalc2_short_sum(m0), 5));
  assert(eq(wmacalc2_long_sum(m0), 5));

  assert(!wmacalc2_add(&inc, m0, 6));
  assert(eq(wmacalc2_short_sum(m0), 11));
  assert(eq(wmacalc2_long_sum(m0), 11));

  assert(!wmacalc2_add(&inc, m0, 2));
  assert(eq(wmacalc2_short_sum(m0), 9.333333333333332));
  assert(eq(wmacalc2_long_sum(m0), 13));

  assert(wmacalc2_add(&inc, m0, 4));
  assert(eq(wmacalc2_short_sum(m0), 10.222222222222221));
  assert(eq(wmacalc2_long_sum(m0), 17));
  assert(eq(inc, 0.002178649237472672));

  assert(wmacalc2_add(&inc, m0, -2));
  assert(eq(wmacalc2_short_sum(m0), 10.222222222222221));
  assert(eq(wmacalc2_long_sum(m0), 17));

  assert(wmacalc2_add(&inc, m0, 5));
  assert(eq(wmacalc2_short_sum(m0), 11.814814814814813));
  assert(eq(wmacalc2_long_sum(m0), 18.6));
  assert(eq(inc, 0.0586751626178147));

  printf( "    Finished\n");
}
