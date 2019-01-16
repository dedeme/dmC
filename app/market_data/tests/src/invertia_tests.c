// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "invertia_tests.h"
#include "servers/invertia.h"
#include <assert.h>

void invertia_tests() {
  puts("Invertia tests:");
/*
  // Arr[char]
  Arr *codes;
  Darr *qcs;
  if (invertia_read_raw(&codes, &qcs)) {
    FAIL("Reading codes invertia");
  }

  EACH_IX(codes, char, code, ix)
    printf("%s -> %f \n", code, darr_get(qcs, ix));
  _EACH

  arr_free(codes);
  darr_free(qcs);
*/
  // Varr[char]
  Varr *nicks;
  Darr *qs;
  assert(!invertia_read(&nicks, &qs));

  EACH_IX(nicks, char, nk, ix)
    printf("%s -> %f \n", nk, darr_get(qs, ix));
  _EACH

  varr_free(nicks);
  darr_free(qs);
  puts("    Finished");
}
