// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "traceTest.h"
#include <dm.h>
#include "Trace.h"

void trace_test(Db *db) {
  printf("Trace test\n");

  Nicks *nicks = db_nicks(db);

  Quote *q = quote_restore("[1.20,1.40,1.50,1.10,33]");

  Portfolio *bf = portfolio_new();
  portfolio_add(bf, 1, 22);

  Portfolio *af = portfolio_new();
  portfolio_add(af, 1, 12);

  Trace *tr = trace_new("20100122", q, 3000, bf, "BKIA", 0, 10, 3500, af, "{}");

  Json *js = trace_serialize(tr, nicks);
//  puts(js);

  Trace *tr2 = trace_copy(tr);
  Json *js2 = trace_serialize(tr2, nicks);
  assert(!strcmp(js, js2));

  printf( "    Finished\n");
}
