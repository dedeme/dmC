// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/MaCalc.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void macalc_test() {
  printf("MaCalc test\n");

  MaCalc *m0 = macalc_new(3, 1.1, 0.8, 7);
  assert(macalc_add(m0, 1.5) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 1.5));
  assert(macalc_add(m0, 2) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 3.5));
  assert(macalc_add(m0, 3) == MACALC_BUY);
  assert(eq(macalc_sum(m0), 6.5));
  assert(macalc_add(m0, 6) == MACALC_BUY);
  assert(eq(macalc_sum(m0), 11));
  assert(macalc_add(m0, 2) == MACALC_SELL);
  assert(eq(macalc_sum(m0), 11));
  assert(macalc_add(m0, 4) == MACALC_NOP);
  assert(eq(macalc_sum(m0), 12));
  assert(macalc_add(m0, 5) == MACALC_BUY);

  m0 = macalc_new(3, 1.1, 0.8, 7);
  assert(macalc_add(m0, -1.5) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 0));
  assert(macalc_add(m0, 2) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 2));
  assert(macalc_add(m0, 3) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 5));
  assert(macalc_add(m0, 6) == MACALC_BUY);
  assert(eq(macalc_sum(m0), 11));
  assert(macalc_add(m0, -2) == MACALC_NOT_VALID);
  assert(eq(macalc_sum(m0), 11));
  assert(macalc_add(m0, 4) == MACALC_NOP);
  assert(eq(macalc_sum(m0), 13));
  assert(macalc_add(m0, 3) == MACALC_SELL);
  assert(eq(macalc_sum(m0), 13));

  printf( "    Finished\n");
}
