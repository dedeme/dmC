// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_sys.h"
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \s -> i
static Exp *len (Arr *exps) {
  CHECK_PARS ("str.len", 1, exps);
  return exp_int(strlen(exp_rget_string(arr_get(exps, 0))));
}

Bfunction md_str_get (char *fname) {
  if (!strcmp(fname, "len")) return len;
  EXC_KUT(fail_bfunction("str", fname));
  return NULL; // Unreachable
}

Exp *md_str_at (char *s, int64_t i) {
  EXC_RANGE(i, 0, strlen(s) - 1);
  return exp_string(str_new_c(s[i]));
}
