// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <assert.h>
#include <stdio.h>
#include "dmc/Opt.h"
#include "dmc/str.h"
#include "dmc/Achar.h"
#include "dmc/Ochar.h"

void opt_test() {
  printf("opt test\n");

  Opt *o = opt_null();
  assert(opt_is_null(o));

  char *d = "data";
  o = opt_new(d);
  assert(!opt_is_null(o));
  assert(str_eq(opt_value(o), "data"));

  Ochar *oc = ochar_null();
  assert(ochar_is_null(oc));

  d = "data";
  oc = ochar_new(d);
  assert(!ochar_is_null(oc));
  assert(str_eq(ochar_value(oc), "data"));

  printf("    Finished\n");
}
