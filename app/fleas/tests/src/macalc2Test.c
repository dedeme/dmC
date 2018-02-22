// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/MaCalc2.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void macalc2_test() {
  printf("MaCalc2 test\n");

  double inc;

  MaCalc2 *m0 = macalc2_new(3, 5, 7);

  assert(!macalc2_add(&inc, m0, 1.5));
  assert(eq(macalc2_short_sum(m0), 1.5));
  assert(eq(macalc2_long_sum(m0), 1.5));
  assert(eq(*macalc2_short_values(m0), 1.5));
  assert(eq(*macalc2_long_values(m0), 1.5));

  assert(!macalc2_add(&inc, m0, 2));
  assert(eq(macalc2_short_sum(m0), 3.5));
  assert(eq(macalc2_long_sum(m0), 3.5));
  assert(eq(*macalc2_short_values(m0), 1.5));
  assert(eq(*macalc2_long_values(m0), 1.5));

  assert(!macalc2_add(&inc, m0, 3));
  assert(eq(macalc2_short_sum(m0), 6.5));
  assert(eq(macalc2_long_sum(m0), 6.5));

  assert(!macalc2_add(&inc, m0, 6));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 12.5));

  assert(macalc2_add(&inc, m0, 2));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 14.5));
  assert(eq(inc, 0.26436781609195403));

  assert(macalc2_add(&inc, m0, 4));
  assert(eq(macalc2_short_sum(m0), 12));
  assert(eq(macalc2_long_sum(m0), 17));
  assert(eq(inc, 0.17647058823529413));

  assert(macalc2_add(&inc, m0, 5));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 20));
  assert(eq(inc, -0.08333333333333333));

  m0 = macalc2_new(3, 5, 8);

  assert(!macalc2_add(&inc, m0, -1.5));
  assert(eq(macalc2_short_sum(m0), 0));
  assert(eq(macalc2_long_sum(m0), 0));

  assert(!macalc2_add(&inc, m0, 2));
  assert(eq(macalc2_short_sum(m0), 2));
  assert(eq(macalc2_long_sum(m0), 2));
  assert(eq(*macalc2_short_values(m0), 2));
  assert(eq(*macalc2_long_values(m0), 2));

  assert(!macalc2_add(&inc, m0, 3));
  assert(eq(macalc2_short_sum(m0), 5));
  assert(eq(macalc2_long_sum(m0), 5));
  assert(eq(*macalc2_short_values(m0), 2));
  assert(eq(*macalc2_long_values(m0), 2));

  assert(!macalc2_add(&inc, m0, 6));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 11));

  assert(!macalc2_add(&inc, m0, 2));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 13));

  assert(macalc2_add(&inc, m0, 4));
  assert(eq(macalc2_short_sum(m0), 12));
  assert(eq(macalc2_long_sum(m0), 17));
  assert(eq(inc, 0.17647058823529413));

  assert(macalc2_add(&inc, m0, -2));
  assert(eq(macalc2_short_sum(m0), 12));
  assert(eq(macalc2_long_sum(m0), 17));

  assert(macalc2_add(&inc, m0, 5));
  assert(eq(macalc2_short_sum(m0), 11));
  assert(eq(macalc2_long_sum(m0), 20));
  assert(eq(inc, -0.08333333333333333));

  printf( "    Finished\n");
}
