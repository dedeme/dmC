// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_arr.h"
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \a -> i
static Exp *size (Arr *exps) {
  CHECK_PARS ("arr.size", 1, exps);
  return exp_int(arr_size(exp_rget_array(arr_get(exps, 0))));
}

Bfunction md_arr_get (char *fname) {
  if (!strcmp(fname, "size")) return size;
  EXC_KUT(fail_bfunction("arr", fname));
  return NULL; // Unreachable
}
