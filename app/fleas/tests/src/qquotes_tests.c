// Copyright 26-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "qquotes_tests.h"

#include "gen_tests.h"
#include "assert.h"
#include "Qquotes.h"

void qquotes_tests(void) {
  puts("Qquotes tests:");

  Qquotes *qqs = qquotes_new(5);

  char *s = qquotes_to_str_new(qqs);
  assert(str_eq(s, "[-3.00, -3.00, -3.00, -3.00, -3.00]"));
  free(s);
  assert(qquotes_max(qqs) < 0);
  assert(qquotes_min(qqs) < 0);
  assert(qquotes_avg(qqs) < 0);

  qquotes_add(qqs, 1);
  qquotes_add(qqs, 3);
  qquotes_add(qqs, 5);

  s = qquotes_to_str_new(qqs);
  assert(str_eq(s, "[-3.00, -3.00, 1.00, 3.00, 5.00]"));
  free(s);
  assert(qquotes_max(qqs) < 0);
  assert(qquotes_min(qqs) < 0);
  assert(qquotes_avg(qqs) < 0);

  qquotes_add(qqs, 2);
  qquotes_add(qqs, 3);

  s = qquotes_to_str_new(qqs);
  assert(str_eq(s, "[1.00, 3.00, 5.00, 2.00, 3.00]"));
  free(s);
  assert(qquotes_max(qqs) == 5);
  assert(qquotes_min(qqs) == 1);
  assert(qquotes_avg(qqs) == 2.8);

  qquotes_free(qqs);
  puts("Finished");
}
