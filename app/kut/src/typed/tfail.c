// Copyright 23-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/tfail.h"
#include "fileix.h"

char *tfail_position (StatCode *st) {
  return str_f(
    "%s:%d:",
    fileix_to_fail(stat_code_file_ix(st)),
    stat_code_line(st)
  );
}

void tfail_throw (StatCode *st, char *msg_fmt, ...) {
  va_list args;
  va_start(args, msg_fmt);
  char *msg = str_vf(msg_fmt, args);
  va_end(args);

  EXC_KUT(str_f("%s\n  %s", msg, tfail_position(st)));
}

static char *check_type (Exp *exp, Ttype actual, int n, Ttype *expected) {
  int ok = FALSE;
  Ttype *p = expected;
  REPEAT(n) {
    if (*p++ == actual) {
      ok = TRUE;
      break;
    }
  }_REPEAT
  if (ok) return "";

  char *plt = exp
    ? str_f("Bad type:\n  %s\n  Expected: ", exp_to_js(exp))
    : "Bad type:\n  Expected: "
  ;
  if (n == 1)
    return str_f(
      "%s%s\n    Actual: %s",
      plt,
      ttype_to_str(*expected),
      ttype_to_str(actual)
    );

  if (n == 2)
    return str_f(
      "%s%s or %s\n    Actual: %s",
      plt,
      ttype_to_str(expected[0]),
      ttype_to_str(expected[1]),
      ttype_to_str(actual)
    );

  RANGE0(i, n - 2) {
    plt = str_f("%s%s, ", plt, ttype_to_str(expected[i]));
  }_RANGE
  return str_f(
    "%s%s or %s\n    Actual: %s",
    plt,
    ttype_to_str(expected[n - 2]),
    ttype_to_str(expected[n - 1]),
    ttype_to_str(actual)
  );
}

void tfail_check_type (StatCode *st, Ttype actual, int n, Ttype *expected) {
  char *msg = check_type(NULL, actual, n, expected);
  if (*msg) tfail_throw(st, msg);
}

void tfail_check_type1 (StatCode *st, Ttype actual, Ttype expected) {
  tfail_check_type(st, actual, 1, (Ttype[]){expected});
}

void tfail_check_etype (
  StatCode *st, Exp *exp, Ttype actual, int n, Ttype *expected
) {
  char *msg = check_type(exp, actual, n, expected);
  if (*msg) tfail_throw(st, msg);
}

void tfail_check_etype1 (StatCode *st, Exp *exp, Ttype actual, Ttype expected){
  tfail_check_etype(st, exp, actual, 1, (Ttype[]){expected});
}
