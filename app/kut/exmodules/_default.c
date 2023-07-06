// Copyright 04-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "exmodule.h"

// \s -> s
static Exp *test(Exp *tx) {
  char *t = exp_rget_string(tx);
  return exp_string(str_f("%s ok", t, NULL));
}

Exp *exmodule_run (char *fn, Exp *data) {
  if (!strcmp(fn, "test")) return test(data);

  EXC_KUT(str_f("Extern 'c' function '%s' not found", fn));
  return NULL; // Unreachable
}
