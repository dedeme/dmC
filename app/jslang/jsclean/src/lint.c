// Copyright 07-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lint.h"

// files is Arr[char]
char *lint_run(Arr *files) {
  if (arr_size(files) == 0) {
    return "";
  }

  char *params = str_cjoin(files, ' ');
  // Opt[char]
  return opt_oget(
    sys_cmd(str_f("eslint %s", params)),
    "Fail running eslint"
  );
}
