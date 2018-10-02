// Copyright 26-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "testIo.h"
#include "assert.h"
#include "io.h"
#include "market/Quote.h"

void testIo_run(void) {
  puts("Io test:");

  puts("    read_nicks");

  int model;
  Achar *nicks;
  io_read_nicks(&model, &nicks, "data", (Group *)"all");
  assert(str_eq(achar_get(nicks, model), "TEF"));
  assert(achar_size(nicks) == 5);

  io_read_nicks(&model, &nicks, "data", (Group *)"ibex");
  assert(str_eq(achar_get(nicks, model), "TEF"));
  assert(achar_size(nicks) == 2);
  assert(str_eq(achar_get(nicks, 0), "TEF"));
  assert(str_eq(achar_get(nicks, 1), "VOC"));

  io_read_nicks(&model, &nicks, "data", (Group *)"sel");
  assert(str_eq(achar_get(nicks, model), "TEF"));
  assert(achar_size(nicks) == 3);
  assert(!ichar_contains(achar_to_it(nicks), "ADZ"));
  assert(!ichar_contains(achar_to_it(nicks), "VOC"));

  puts("    read_quotes");

  Achar *dates;
  Quote ***qs;

  io_read_quotes(&nicks, &dates, &qs, "data", (Group *)"all");
  int nicks_size = achar_size(nicks);
  int dates_size = achar_size(dates);
  assert(nicks_size == 3);
  assert(dates_size == 550);
  assert(quote_open(qs[0][0]) == 3.28);
  assert(quote_open(qs[540][1]) == 6.667);
  assert(quote_open(qs[549][2]) == 1.282);

  io_read_quotes(&nicks, &dates, &qs, "data", (Group *)"ibex");
  nicks_size = achar_size(nicks);
  dates_size = achar_size(dates);
  assert(nicks_size == 2);
  assert(dates_size == 550);
  assert(quote_open(qs[0][0]) == 8.923);
  assert(quote_open(qs[540][0]) == 6.667);
  assert(quote_open(qs[549][1]) == 1.282);

  io_read_quotes(&nicks, &dates, &qs, "data", (Group *)"sel");
  nicks_size = achar_size(nicks);
  dates_size = achar_size(dates);
  assert(nicks_size == 1);
  assert(dates_size == 550);
  assert(quote_open(qs[0][0]) == 8.923);
  assert(quote_open(qs[540][0]) == 6.667);
  assert(quote_open(qs[549][0]) == 6.812);

  puts("    Finished");
}
