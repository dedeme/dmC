// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_dic.h"
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \d, s -> [*]|[]
static Exp *getOp (Arr *exps) {
  CHECK_PARS ("dic.get", 2, exps);
  Exp *r = opt_get(map_get(
    exp_rget_map(arr_get(exps, 0)),
    exp_rget_string(arr_get(exps, 1))
  ));
  return r
    ? exp_array(arr_new_from(r, NULL))
    : exp_array(arr_new())
  ;
}

Bfunction md_dic_get (char *fname) {
  if (!strcmp(fname, "get")) return getOp;
  EXC_KUT(fail_bfunction("dic", fname));
  return NULL; // Unreachable
}

Exp *md_dic_fget (Map *dic, char *key) {
  Exp *r = opt_get(map_get(dic, key));
  if (!r)
    EXC_KUT(str_f("Key '%s' not found in dictionary", key));
  return r;
}
