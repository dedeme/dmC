// Copyright 06-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_path.h"
#include "kut/path.h"
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \s -> s
static Exp *base (Arr *exps) {
  CHECK_PARS ("path.base", 1, exps);
  return exp_string(path_base(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *canonical (Arr *exps) {
  CHECK_PARS ("path.canonical", 1, exps);
  char *path = exp_rget_string(arr_get(exps, 0));
  char *r = opt_get(path_canonical(path));
  if (!r)
    EXC_KUT(str_f("'%s' can not be canonicalized", path));
  return exp_string(r);
}

// \[s...] -> s
static Exp *cat (Arr *exps) {
  CHECK_PARS ("path.cat", 1, exps);

  return exp_string(path_clean(arr_cjoin(
    arr_map(exp_rget_array(arr_get(exps, 0)), (FMAP)exp_rget_string)
  , '/')));
}

// \s -> s
static Exp *clean (Arr *exps) {
  CHECK_PARS ("path.clean", 1, exps);
  return exp_string(path_clean(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *extension (Arr *exps) {
  CHECK_PARS ("path.extension", 1, exps);
  return exp_string(path_extension(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *parent (Arr *exps) {
  CHECK_PARS ("path.parent", 1, exps);
  return exp_string(path_parent(exp_rget_string(arr_get(exps, 0))));
}

Bfunction md_path_get (char *fname) {
  if (!strcmp(fname, "base")) return base;
  if (!strcmp(fname, "canonical")) return canonical;
  if (!strcmp(fname, "cat")) return cat;
  if (!strcmp(fname, "clean")) return clean;
  if (!strcmp(fname, "extension")) return extension;
  if (!strcmp(fname, "parent")) return parent;

  EXC_KUT(fail_bfunction("path", fname));
  return NULL; // Unreachable
}
