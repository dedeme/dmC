// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "trading_tests.h"
#include "trading.h"
#include "pf.h"
#include <assert.h>
#include "DEFS.h"

void trading_tests() {
  puts("Trading tests:");

  // Map[Double]
  Darr *last_qs;
  Darr *signals;
  trading_read_new(&last_qs, &signals);

  char *nicks[] = NICKS;
  char **p = nicks;
  int c = 0;
  while (*p) {
    printf("%s -> %f : %f\n", *p++, darr_get(last_qs, c), darr_get(signals, c));
    ++c;
  }

  // Map[PfValue]
  Map *pf = pf_read_new();

  // Arr[char]
  Varr *keys = map_keys_new(pf);
  varr_sort(keys, (FGREATER)str_greater);
  EACH(keys, char, k)
    char **p = nicks;
    int c = 0;
    while (*p) {
      if (str_eq(*p++, k)) break;
      ++c;
    }
    double last_q = darr_get(last_qs, c);
    double signal = -darr_get(signals, c);

    PfValue *v = map_get_null(pf, k);
    int stocks =  pfValue_stocks(v);
    double price = pfValue_price(v);
//    printf("%s -> [%d, %f][%f, %f] -> {Vl = %f, Pr = %f, Rk = %f}\n",
//      k, stocks, price, last_q, signal,
//      stocks * last_q, stocks * (last_q - price), stocks * (signal - price)
//    );
    printf("%s -> {Vl = %f, Pr = %f, Rk = %f}\n",
      k, stocks * last_q, stocks * (last_q - price), stocks * (signal - price)
    );
  _EACH

  varr_free(keys);
  map_free(pf);

  darr_free(last_qs);
  darr_free(signals);

  puts("    Finished");
}
