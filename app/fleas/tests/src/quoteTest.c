// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "quoteTest.h"
#include <dm.h>
#include "Quote.h"

void quote_test() {
  printf("Quote test\n");

  Quote *q = quote_restore("[1.20,1.40,1.50,1.10,33]");
  assert(quote_open(q) == 1.20);
  assert(quote_close(q) == 1.4);
  assert(quote_max(q) == 1.5);
  assert(quote_min(q) == 1.1);
  assert(quote_vol(q) == 33);
  Json *js = quote_serialize(q);
  assert(!strcmp(js,
    "[1.2000,1.4000,1.5000,1.1000,33]"));

  q = quote_restore(js);
  assert(quote_open(q) == 1.20);
  assert(quote_close(q) == 1.4);
  assert(quote_max(q) == 1.5);
  assert(quote_min(q) == 1.1);
  assert(quote_vol(q) == 33);

  printf( "    Finished\n");
}

