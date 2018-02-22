// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "sellTest.h"
#include "feesTest.h"
#include <dm.h>
#include "order/fees.h"
#include "order/Sell.h"
#include "Quote.h"

#define eq(a, b) dec_eq_gap(a, b, 0.00001)

void sell_test(Db *db) {
  printf("Sell test\n");

  Nicks *nicks = db_nicks(db);

  Sells *ss = sells_new();
  sells_add(ss, 1, 21556);
  Sell *s1 = sells_get(ss, 0);
  assert(sell_nick(s1) == 1);
  assert(sell_stocks(s1) == 21556);
  Json *js = sell_serialize(s1, nicks);
  assert(!strcmp(js, "[\"BKIA\",21556]"));

  Arr/*Quote*/ *qs = arr_new();

  Quote *q1 = quote_restore("[7.20,7.40,7.50,7.10,33]");
  arr_add(qs, q1);
  Quote *q2 = quote_restore("[1.20,1.40,1.50,1.10,33]");
  arr_add(qs, q2);
  Quote *q3 = quote_restore("[8.20,8.40,8.50,8.10,33]");
  arr_add(qs, q3);

  double r = sell_do(s1, qs);
  double vstocks = 21556 * 1.2;
  assert(eq(r, vstocks - fees_app(vstocks)));

  printf( "    Finished\n");
}
