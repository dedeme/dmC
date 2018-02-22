// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/WmaCalc.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void wmacalc_test() {
  printf("WmaCalc test\n");

  WmaCalc *m0 = wmacalc_new(3, 1.1, 0.8);
  assert(wmacalc_add(m0, 1.5) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 1.5));
  assert(wmacalc_add(m0, 2) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 3.5));
  assert(wmacalc_add(m0, 3) == WMACALC_BUY);
  assert(eq(wmacalc_sum(m0), 6.5));
  assert(wmacalc_add(m0, 6) == WMACALC_BUY);
  assert(eq(wmacalc_sum(m0), 10.333333));
  assert(wmacalc_add(m0, 2) == WMACALC_SELL);
  assert(eq(wmacalc_sum(m0), 8.888889));
  assert(wmacalc_add(m0, 3) == WMACALC_NOP);
  assert(eq(wmacalc_sum(m0), 8.925926));
  assert(wmacalc_add(m0, 5) == WMACALC_BUY);
  assert(eq(wmacalc_sum(m0), 10.950617));

  m0 = wmacalc_new(3, 1.1, 0.8);
  assert(wmacalc_add(m0, -1.5) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 0));
  assert(wmacalc_add(m0, 2) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 2));
  assert(wmacalc_add(m0, 3) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 5));
  assert(wmacalc_add(m0, 6) == WMACALC_BUY);
  assert(eq(wmacalc_sum(m0), 11));
  assert(wmacalc_add(m0, 2) == WMACALC_SELL);
  assert(eq(wmacalc_sum(m0), 9.333333));
  assert(wmacalc_add(m0, -2) == WMACALC_NOT_VALID);
  assert(eq(wmacalc_sum(m0), 9.333333));
  assert(wmacalc_add(m0, 3) == WMACALC_NOP);
  assert(eq(wmacalc_sum(m0), 9.222222));

  printf( "    Finished\n");
}
