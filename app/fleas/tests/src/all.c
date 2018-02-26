// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "io.h"
#include "Db.h"
#include "genTest.h"
#include "quoteTest.h"
#include "feesTest.h"
#include "buyTest.h"
#include "sellTest.h"
#include "traceTest.h"
#include "fleaTest.h"
#include "maCalcTest.h"
#include "wmaCalcTest.h"
#include "udCalcTest.h"
#include "maCalc2Test.h"
#include "udCalc2Test.h"
#include "wmaCalc2Test.h"

int main() {
  sys_init("fleas");
  io_init();
  Db *db = db_new();

  gen_test();
  quote_test();
  fees_test();
  buy_test(db);
  sell_test(db);
  trace_test(db);
  flea_test(db);
  macalc_test();
  wmacalc_test();
  udcalc_test();
  macalc2_test();
  udcalc2_test();
  wmacalc2_test();

  quotes_free();

  return 0;
}
