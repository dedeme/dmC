// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/UdCalc.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void udcalc_test() {
  printf("UdCalc test\n");

  UdCalc *m0 = udcalc_new(3, 1.1, 0.8, 7);
  assert(udcalc_add(m0, 1.5) == UDCALC_NOT_VALID);
  assert(eq(*udcalc_values(m0), 1.5));
  assert(udcalc_add(m0, 2) == UDCALC_NOT_VALID);
  assert(eq(*udcalc_values(m0), 1.5));
  assert(udcalc_add(m0, 3) == UDCALC_BUY);
  assert(eq(*udcalc_values(m0), 2));
  assert(udcalc_add(m0, 6) == UDCALC_BUY);
  assert(eq(*udcalc_values(m0), 3));
  assert(udcalc_add(m0, 2) == UDCALC_SELL);
  assert(eq(*udcalc_values(m0), 6));
  assert(udcalc_add(m0, 5) == UDCALC_NOP);
  assert(eq(*udcalc_values(m0), 2));
  assert(udcalc_add(m0, 5) == UDCALC_BUY);
  assert(eq(*udcalc_values(m0), 5));

  m0 = udcalc_new(3, 1.1, 0.8, 7);
  assert(udcalc_add(m0, -1.5) == UDCALC_NOT_VALID);
  assert(udcalc_add(m0, 2) == UDCALC_NOT_VALID);
  assert(eq(*udcalc_values(m0), 2));
  assert(udcalc_add(m0, 3) == UDCALC_NOT_VALID);
  assert(eq(*udcalc_values(m0), 2));
  assert(udcalc_add(m0, 6) == UDCALC_BUY);
  assert(eq(*udcalc_values(m0), 3));
  assert(udcalc_add(m0, -2) == UDCALC_NOT_VALID);
  assert(eq(*udcalc_values(m0), 3));
  assert(udcalc_add(m0, 5) == UDCALC_BUY);
  assert(eq(*udcalc_values(m0), 6));
  assert(udcalc_add(m0, 5) == UDCALC_NOP);
  assert(eq(*udcalc_values(m0), 5));

  printf( "    Finished\n");
}
