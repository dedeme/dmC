// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "testGen.h"
#include "assert.h"
#include "Gen.h"

void testGen_run(void) {
  puts("Gen test:");

  double gens[4] = {1, 2, 3, 4};
  Gen *g = gen_new(4, gens);
  assert(gen_get(g, 0) == 1);
  assert(gen_get(g, 1) == 2);
  assert(gen_get(g, 2) == 3);
  assert(gen_get(g, 3) == 4);

  Gen *g2 = gen_from_json(gen_to_json(g));
  assert(gen_get(g2, 0) == 1);
  assert(gen_get(g2, 1) == 2);
  assert(gen_get(g2, 2) == 3);
  assert(gen_get(g2, 3) == 4);

/*
  Gen *g3 = gen_mutate(g);
  printf("%f, %f, %f, %f\n",
    gen_get(g3, 0),
    gen_get(g3, 1),
    gen_get(g3, 2),
    gen_get(g3, 3)
  );
*/

  puts("    Finished");
}

