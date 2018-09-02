// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "params_reading.h"
#include "test_duplicates.h"
#include "filter.h"
#include "lint.h"
#include "clean.h"
#include "dot.h"
#include "tp/P.h"

int main (int argc, char **argv) {
  sys_init("jsclean");
  dot_init();

  Oparams *ops = params_reading_run(argc, argv);
  if (oparams_is_null(ops)) {
    return 1;
  }
  Params *ps = oparams_value(ops);
  Achar *roots = params_roots(ps);
  char *target = params_target(ps);

  bool dups = test_duplicates_run(roots);
  if (dups) {
    return 1;
  }

  Ap *to_lint = filter_run(roots, target);

  lint_run(to_lint);

  EACH(to_lint, P, p) {
    clean_run(p, target);
 }_EACH

  return 0;
}
