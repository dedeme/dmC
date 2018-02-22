// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/UdCalc2.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void udcalc2_test() {
  printf("UdCalc2 test\n");

  double inc;

  UdCalc2 *m0 = udcalc2_new(3, 7);

  assert(!udcalc2_add(&inc, m0, 1.5));
  assert(eq(*udcalc2_values(m0), 1.5));

  assert(!udcalc2_add(&inc, m0, 2));
  assert(eq(*udcalc2_values(m0), 1.5));

  assert(udcalc2_add(&inc, m0, 3));
  assert(eq(inc, 1));

  assert(udcalc2_add(&inc, m0, 6));
  assert(eq(inc, 2));

  assert(udcalc2_add(&inc, m0, 2));
  assert(eq(inc, -0.3333333333333));

  assert(udcalc2_add(&inc, m0, 4));
  assert(eq(inc, -0.3333333333333));

  assert(udcalc2_add(&inc, m0, 5));
  assert(eq(inc, 1.5));

  m0 = udcalc2_new(3, 7);

  assert(!udcalc2_add(&inc, m0, -1.5));

  assert(!udcalc2_add(&inc, m0, 2));
  assert(eq(*udcalc2_values(m0), 2));

  assert(!udcalc2_add(&inc, m0, 3));
  assert(eq(*udcalc2_values(m0), 2));

  assert(udcalc2_add(&inc, m0, 6));
  assert(eq(*udcalc2_values(m0), 2));
  assert(eq(inc, 2));

  assert(!udcalc2_add(&inc, m0, -2));
  assert(eq(*udcalc2_values(m0), 2));

  assert(udcalc2_add(&inc, m0, 4));
  assert(eq(*udcalc2_values(m0), 3));
  assert(eq(inc, 0.333333333333));

  assert(udcalc2_add(&inc, m0, 5));
  assert(eq(inc, -0.166666666666667));

  printf( "    Finished\n");
}
