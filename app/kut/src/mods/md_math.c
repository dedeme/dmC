// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_math.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \ -> f
static Exp *max_float (Arr *exps) {
  CHECK_PARS ("math.maxFloat", 0, exps);
  return exp_float(DBL_MAX);
}

// \ -> i
static Exp *max_int (Arr *exps) {
  CHECK_PARS ("math.maxInt", 0, exps);
  return sizeof(int64_t) == sizeof(long long int)
    ? exp_int(LLONG_MAX)
    : exp_int(LONG_MAX)
  ;
}

// \f, f -> f
static Exp *fpow (Arr *exps) {
  CHECK_PARS ("math.pow", 2, exps);
  return exp_float(pow(
    exp_rget_float(arr_get(exps, 0)),
    exp_rget_float(arr_get(exps, 1))
  ));
}


Bfunction md_math_get (char *fname) {
  if (!strcmp(fname, "maxFloat")) return max_float;
  if (!strcmp(fname, "maxInt")) return max_int;
  if (!strcmp(fname, "pow")) return fpow;
  EXC_KUT(fail_bfunction("math", fname));
  return NULL; // Unreachable
}
