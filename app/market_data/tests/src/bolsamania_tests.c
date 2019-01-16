// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bolsamania_tests.h"
#include "servers/bolsamania.h"
#include <assert.h>

void bolsamania_tests() {
  puts("Bolsamania tests:");
/*
   // Arr[char]
  Arr *codes;
  Darr *qcs;
  assert(!bolsamania_read_raw(&codes, &qcs));

  EACH_IX(codes, char, code, ix)
    printf("%s -> %f \n", code, darr_get(qcs, ix));
  _EACH

  arr_free(codes);
  darr_free(qcs);
*/
  // Varr[char]
  Varr *nicks;
  Darr *qs;
  assert(!bolsamania_read(&nicks, &qs));

  EACH_IX(nicks, char, nk, ix)
    printf("%s -> %f \n", nk, darr_get(qs, ix));
  _EACH

  varr_free(nicks);
  darr_free(qs);
  puts("    Finished");
}
