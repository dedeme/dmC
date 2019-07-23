// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "log_tests.h"
#include "assert.h"
#include "log.h"

void log_tests() {
  puts("Log tests:");

  assert(str_eq(log_error_s("An error"), "{\"error\":\"An error\"}"));
  assert(str_eq(log_ok_s(js_wi(12)), "{\"error\":\"\",\"value\":12}"));

  puts("    Finished");
}
