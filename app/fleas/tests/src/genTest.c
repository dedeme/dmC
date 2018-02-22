// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "genTest.h"
#include <dm.h>
#include "Gen.h"

void gen_test() {
  printf("Gen test\n");

  Gen *g = gen_new(3);
  while (gen_actual(g) != 0) {
    assert(gen_actual(g) > 0 && gen_actual(g) < 3);
    g = gen_new(3);
  }

  Json *js = gen_serialize(g);
  assert(!strcmp(js, "[3,0]"));
  Gen *g2 = gen_restore(js);
  assert(gen_max(g2) == 3);
  assert(gen_actual(g2) == 0);

  g2 = gen_copy(g);
  assert(gen_max(g2) == 3);
  assert(gen_actual(g2) == 0);

  REPEAT(100) {
    g = gen_mutate(g2);
    assert(gen_actual(g) >= 0 && gen_actual(g) < 3);
    assert(gen_max(g) == 3);
    g2 = g;
  }_REPEAT

  printf( "    Finished\n");
}
