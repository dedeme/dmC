// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "buyTest.h"
#include "feesTest.h"
#include <dm.h>
#include "order/fees.h"
#include "order/Buy.h"
#include "Quote.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void buy_test(Db *db) {
  printf("Buy test\n");

  Nicks *nicks = db_nicks(db);

  Buys *bs = buys_new();
  buys_add(bs, 1, 20000.0);
  Buy *b1 = buys_get(bs, 0);
  assert(buy_nick(b1) == 1);
  assert(buy_money(b1) == 20000);
  Json *js = buy_serialize(b1, nicks);

  assert(!strcmp(js, "[\"BKIA\",20000.00]"));

  Arr/*Quote*/ *qs = arr_new();

  Quote *q1 = quote_restore("[7.20,7.40,7.50,7.10,33]");
  arr_add(qs, q1);
  Quote *q2 = quote_restore("[1.20,1.40,1.50,1.10,33]");
  arr_add(qs, q2);
  Quote *q3 = quote_restore("[8.20,8.40,8.50,8.10,33]");
  arr_add(qs, q3);

  uint stocks;
  double cost;
  buy_do(&stocks, &cost, b1, qs);
  assert(cost < 20000);
  double vstocks1 = (stocks + 1) * 1.2;
  assert(vstocks1 + fees_app(vstocks1) > 20000);
  double vstocks = stocks * 1.2;
  assert(eq(vstocks + fees_app(vstocks), cost));

  printf( "    Finished\n");
}
