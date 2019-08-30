// Copyright 07-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lint.h"
#include "data/Path.h"

// files is Arr[Path]
char *lint_run(Arr *paths) {
  if (arr_size(paths) == 0) {
    return "";
  }

  char *fn (Path *p) { return path_absolute(p); }
  char *params = str_cjoin(
    arr_map(paths, (FCOPY)fn),
    ' '
  );
  // Opt[char]
  return opt_oget(
    sys_cmd(str_f("eslint %s", params)),
    "Fail running eslint"
  );
}
