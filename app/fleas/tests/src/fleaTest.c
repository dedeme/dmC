// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "fleaTest.h"
#include <dm.h>
#include "Flea.h"

void flea_test(Db *db) {
  printf("Flea test\n");

  Nicks *nicks = db_nicks(db);

  Flea *f = flea_new(2, 34, nicks_size(nicks));
  Json *js = flea_serialize(f);
  Flea *f2 = flea_restore(js, nicks_size(nicks));
  Json *js2 = flea_serialize(f2);
  assert(!strcmp(js, js2));


  Fleas *fs = fleas_new(30);
  RANGE0(i, fleas_size(fs)) {
    assert(!fleas_get(fs, i));
  }_RANGE

  fleas_set(fs, 3, f);
  js2 = flea_serialize(fleas_get(fs, 3));
  assert(!strcmp(js2, js));

  fleas_set(fs, 4, f2);
  js2 = flea_serialize(fleas_get(fs, 4));
  assert(!strcmp(js2, js));

  fleas_kill(fs, 4);
  assert(!fleas_get(fs, 4));

  printf( "    Finished\n");
}
