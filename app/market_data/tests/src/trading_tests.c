// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "trading_tests.h"
#include "trading.h"
#include "pf.h"
#include "io.h"
#include <assert.h>
#include "DEFS.h"

void trading_tests() {
  puts("Trading tests:");

  // Arr[char]
  Arr *nicks = io_nicks_new();

  // Map[Double]
  Darr *last_qs;
  Darr *signals;
  trading_read_new(&last_qs, &signals, nicks);

  EACH_IX(nicks, char, nick, c)
    printf("%s -> %f : %f\n", nick, darr_get(last_qs, c), darr_get(signals, c));
  _EACH

  // Map[PfValue]
  Map *pf = pf_read_new();

  // Arr[char]
  Varr *keys = map_keys_new(pf);
  varr_sort(keys, (FGREATER)str_greater);
  EACH(keys, char, k)
    if (str_eq(k, "PVA")) {
      continue;
    }
    int c = 0;
    EACH(nicks, char, nick)
      if (str_eq(nick, k)) break;
      ++c;
    _EACH
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
