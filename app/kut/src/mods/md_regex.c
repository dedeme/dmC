// Copyright 07-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_regex.h"
#include "kut/regex.h"
#include "exp.h"
#include "function.h"
#include "DEFS.h"
#include "runner/fail.h"
#include "runner/runner.h"

// \s, s -> [s...]
static Exp *matches (Arr *exps) {
  CHECK_PARS ("regex.matches", 2, exps);
  char *s = exp_rget_string(arr_get(exps, 0));
  char *rg = exp_rget_string(arr_get(exps, 1));
  // <RegexOffset>
  Arr *r = opt_get(regex_matches(rg, s));
    //--
    Exp *to_str(RegexOffset* o) {
      return exp_string(str_sub(s, regexOffset_begin(o), regexOffset_end(o)));
    }
  if (r) return exp_array(arr_map(r, (FMAP)to_str));
  EXC_KUT(str_f("Fail of regular expression:\n%s", rg));
  return NULL; // Unreachable
}

// \s, s, s -> s
static Exp *replace (Arr *exps) {
  CHECK_PARS ("regex.replace", 3, exps);
  char *rg = exp_rget_string(arr_get(exps, 1));
  char *r = opt_get(regex_replace(
    rg,
    exp_rget_string(arr_get(exps, 0)),
    exp_rget_string(arr_get(exps, 2))
  ));
  if (r) return exp_string(r);
  EXC_KUT(str_f("Fail of regular expression:\n%s", rg));
  return NULL; // Unreachable
}

// \s, s, (\s->s) -> s
static Exp *replacef (Arr *exps) {
  CHECK_PARS ("regex.replace", 3, exps);
    //--
    Exp *fn (Bfunction bf) {
      char *rg = exp_rget_string(arr_get(exps, 1));
        //--
        char *c_fn (char *match) {
          return exp_rget_string(bf(arr_new_from(exp_string(match), NULL)));
        }
      char *r = opt_get(regex_replacef(
        rg,
        exp_rget_string(arr_get(exps, 0)),
        c_fn
      ));
      if (r) return exp_string(r);
      EXC_KUT(str_f("Fail of regular expression:\n%s", rg));
      return NULL; // Unreachable
    }
  return runner_fn(arr_get(exps, 2), fn);
}

Bfunction md_regex_get (char *fname) {
  if (!strcmp(fname, "matches")) return matches;
  if (!strcmp(fname, "replace")) return replace;
  if (!strcmp(fname, "replacef")) return replacef;

  EXC_KUT(fail_bfunction("regex", fname));
  return NULL; // Unreachable
}
