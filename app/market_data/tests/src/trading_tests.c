// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "trading_tests.h"
#include "trading.h"
#include <assert.h>
#include "DEFS.h"

void trading_tests() {
  puts("Trading tests:");

  // Map[Double]
  Darr *sup_res = trading_read_new();

  char *nicks[] = NICKS;
  char **p = nicks;
  int c = 0;
  while (*p) {
    printf("%s -> %f\n", *p++, darr_get(sup_res, c++));
  }

  darr_free(sup_res);

  puts("    Finished");
}
