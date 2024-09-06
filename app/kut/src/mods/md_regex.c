// Copyright 07-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_regex.h"
#include "kut/regex.h"
#include "exp.h"
#include "obj.h"
#include "function.h"
#include "runner/fail.h"
#include "runner/runner.h"

// \s, s -> [s...]
static Exp *matches (Arr *exps) {
  CHECK_PARS ("regex.matches", 2, exps);
  char *s = exp_get_string(arr_get(exps, 0));
  char *rg = exp_get_string(arr_get(exps, 1));
  // <RegexOffset>
  Arr *ms = opt_get(regex_matches(rg, s));
  if (!ms) EXC_KUT(str_f("Fail compiling regular expression:\n%s", rg));
  // <Exp>
  Arr *r = arr_new();
  int ix = 0;
  EACH(ms, RegexOffset, of) {
    int begin = regexOffset_begin(of);
    int end = regexOffset_end(of);
    arr_push(r, exp_string(str_sub(s, ix, begin)));
    arr_push(r, exp_string(str_sub(s, begin, end)));
    ix = end;
  }_EACH
  arr_push(r, exp_string(str_right(s, ix)));
  return exp_array(r);
}

// \s, s, s -> s
static Exp *replace (Arr *exps) {
  CHECK_PARS ("regex.replace", 3, exps);
  char *rg = exp_get_string(arr_get(exps, 1));
  char *r = opt_get(regex_replace(
    rg,
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 2))
  ));
  if (r) return exp_string(r);
  EXC_KUT(str_f("Fail of regular expression:\n%s", rg));
  return NULL; // Unreachable
}

// \s, s, (\s->s) -> s
static Exp *replacef (Arr *exps) {
  CHECK_PARS ("regex.replace", 3, exps);
  char *rg = exp_get_string(arr_get(exps, 1));
  Exp *fn = arr_get(exps, 2);
    //--
    char *c_fn (char *match) {
      // Exp
      Arr *ps = arr_new_from(exp_string(match), NULL);
      FRUN(rs, fn, ps);
      return exp_get_string(rs);
    }
  char *r = opt_get(regex_replacef(
    rg,
    exp_get_string(arr_get(exps, 0)),
    c_fn
  ));
  if (r) return exp_string(r);
  EXC_KUT(str_f("Fail of regular expression:\n%s", rg));
  return NULL; // Unreachable
}

Bfunction md_regex_get (char *fname) {
  if (!strcmp(fname, "matches")) return matches;
  if (!strcmp(fname, "replace")) return replace;
  if (!strcmp(fname, "replacef")) return replacef;

  EXC_KUT(fail_bfunction("regex", fname));
  return NULL; // Unreachable
}
