// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "skeleton.h"
#include <stdio.h>
#include "kut/arr.h"
#include "sub/mul.h"

Exp *skeleton_echo (Arr *exps) {
  char *s = exp_get_string(arr_get(exps, 0));
  puts(s);
  return exp_empty();
}

Exp *skeleton_sum (Arr *exps) {
  int64_t n1 = exp_get_int(arr_get(exps, 0));
  int64_t n2 = exp_get_int(arr_get(exps, 1));
  return exp_int(n1 + n2);
}
