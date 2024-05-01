// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_math.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include <locale.h>
#include "kut/dec.h"
#include "kut/sys.h"
#include "exp.h"
#include "runner/fail.h"

// returns [f]|[]
static Exp *to_float_op_aux (char *s) {
  s = str_trim(s);
  if (!*s) return exp_array(arr_new());

  struct lconv *lc = localeconv();
  int ix = str_cindex(s, '.');
  if (ix != -1) s[ix] = *lc->decimal_point;

  char *tmp;
  double r = strtod(s, &tmp);
  if (*tmp) return exp_array(arr_new());

  return exp_array(arr_new_from(exp_float(r), NULL));
}

// returns [f]|[]
static Exp *to_int_op_aux (char *s, int base) {
  s = str_trim(s);
  if (!*s) return exp_array(arr_new());

  char *tmp;
  int64_t r = strtol(s, &tmp, base);
  if (*tmp) return exp_array(arr_new());

  return exp_array(arr_new_from(exp_int(r), NULL));
}

// \ -> f
static Exp *e (Arr *exps) {
  CHECK_PARS ("math.e", 0, exps);
  return exp_float(M_E);
}

// \ -> f
static Exp *pi (Arr *exps) {
  CHECK_PARS ("math.pi", 0, exps);
  return exp_float(M_PI);
}

// \f -> f
static Exp *f_abs (Arr *exps) {
  CHECK_PARS ("math.abs", 1, exps);
  double v = exp_rget_float(arr_get(exps, 0));
  return exp_float(v < 0 ? -v : v);
}

// \f -> f
static Exp *f_acos (Arr *exps) {
  CHECK_PARS ("math.acos", 1, exps);
  return exp_float(acos(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_acosh (Arr *exps) {
  CHECK_PARS ("math.acosh", 1, exps);
  return exp_float(acosh(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_asin (Arr *exps) {
  CHECK_PARS ("math.asin", 1, exps);
  return exp_float(asin(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_asinh (Arr *exps) {
  CHECK_PARS ("math.asinh", 1, exps);
  return exp_float(asinh(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_atan (Arr *exps) {
  CHECK_PARS ("math.atan", 1, exps);
  return exp_float(atan(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_atanh (Arr *exps) {
  CHECK_PARS ("math.atanh", 1, exps);
  return exp_float(atanh(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_ceil (Arr *exps) {
  CHECK_PARS ("math.ceil", 1, exps);
  return exp_float(ceil(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_cos (Arr *exps) {
  CHECK_PARS ("math.cos", 1, exps);
  return exp_float(cos(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_cosh (Arr *exps) {
  CHECK_PARS ("math.cosh", 1, exps);
  return exp_float(cosh(exp_rget_float(arr_get(exps, 0))));
}

// \f, f, f -> b
static Exp *eq (Arr *exps) {
  CHECK_PARS ("math.eq", 3, exps);
  return exp_bool(dec_eq_gap(
    exp_rget_float(arr_get(exps, 0)),
    exp_rget_float(arr_get(exps, 1)),
    exp_rget_float(arr_get(exps, 2))
  ));
}

// \f -> f
static Exp *f_exp (Arr *exps) {
  CHECK_PARS ("math.exp", 1, exps);
  return exp_float(exp(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_exp2 (Arr *exps) {
  CHECK_PARS ("math.exp2", 1, exps);
  return exp_float(exp2(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_floor (Arr *exps) {
  CHECK_PARS ("math.floor", 1, exps);
  return exp_float(floor(exp_rget_float(arr_get(exps, 0))));
}

// \s -> [f]|[]
static Exp *from_en (Arr *exps) {
  CHECK_PARS ("math.fromEn", 1, exps);
  char *s = exp_rget_string(arr_get(exps, 0));
  return to_float_op_aux(str_replace(s, ",", ""));
}

// \s -> [i]|[]
static Exp *from_hex (Arr *exps) {
  CHECK_PARS ("math.fromHex", 1, exps);
  return to_int_op_aux(exp_rget_string(arr_get(exps, 0)), 16);
}

// \s -> [f]|[]
static Exp *from_iso (Arr *exps) {
  CHECK_PARS ("math.fromIso", 1, exps);
  char *s = exp_rget_string(arr_get(exps, 0));
  return to_float_op_aux(str_replace(str_replace(s, ".", ""), ",", "."));
}

// \f -> i
static Exp *ftoi (Arr *exps) {
  CHECK_PARS ("math.ftoi", 1, exps);
  return exp_int(exp_rget_float(arr_get(exps, 0)));
}

// \f -> s
static Exp *ftos (Arr *exps) {
  CHECK_PARS ("math.ftos", 1, exps);
  return exp_string(dec_ftos(exp_rget_float(arr_get(exps, 0)), 9));
}

// \s -> b
static Exp *is_digits (Arr *exps) {
  CHECK_PARS ("math.isDigits", 1, exps);
  return exp_bool(dec_digits(exp_rget_string(arr_get(exps, 0))));
}

// \i -> f
static Exp *itof (Arr *exps) {
  CHECK_PARS ("math.itof", 1, exps);
  return exp_float(exp_rget_int(arr_get(exps, 0)));
}

// \i -> s
static Exp *itoh (Arr *exps) {
  CHECK_PARS ("math.itoh", 1, exps);
  return exp_string(str_f("%x", exp_rget_int(arr_get(exps, 0))));
}

// \i -> s
static Exp *itos (Arr *exps) {
  CHECK_PARS ("math.itos", 1, exps);
  return exp_string(dec_itos(exp_rget_int(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_log (Arr *exps) {
  CHECK_PARS ("math.log", 1, exps);
  return exp_float(log(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_log10 (Arr *exps) {
  CHECK_PARS ("math.log10", 1, exps);
  return exp_float(log10(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_log2 (Arr *exps) {
  CHECK_PARS ("math.log2", 1, exps);
  return exp_float(log2(exp_rget_float(arr_get(exps, 0))));
}

// \f, f -> f
static Exp *max (Arr *exps) {
  CHECK_PARS ("math.max", 2, exps);
  return arr_get(
    exps,
    exp_rget_float(arr_get(exps, 0)) >= exp_rget_float(arr_get(exps, 1)) ? 0 : 1
  );
}

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
static Exp *min (Arr *exps) {
  CHECK_PARS ("math.min", 2, exps);
  return arr_get(
    exps,
    exp_rget_float(arr_get(exps, 0)) <= exp_rget_float(arr_get(exps, 1)) ? 0 : 1
  );
}

// \ -> i
static Exp *min_int (Arr *exps) {
  CHECK_PARS ("math.minInt", 0, exps);
  return sizeof(int64_t) == sizeof(long long int)
    ? exp_int(LLONG_MIN)
    : exp_int(LONG_MIN)
  ;
}

// \f, f -> f
static Exp *f_pow (Arr *exps) {
  CHECK_PARS ("math.pow", 2, exps);
  return exp_float(pow(
    exp_rget_float(arr_get(exps, 0)),
    exp_rget_float(arr_get(exps, 1))
  ));
}

// \i -> f
static Exp *f_pow10 (Arr *exps) {
  CHECK_PARS ("math.pow10", 1, exps);
  int64_t n = exp_rget_int(arr_get(exps, 0));
  double r = 1;
  double mul = n > 0 ? 10 : 0.1;
  n = n >= 0 ? n : -n;
  REPEAT(n) {
    r *= mul;
  }_REPEAT
  return exp_float(r);
}

// \ -> f
static Exp *rnd (Arr *exps) {
  CHECK_PARS ("math.rnd", 0, exps);
  return exp_float(sys_rnd_d());
}

// \i -> i
static Exp *rndi (Arr *exps) {
  CHECK_PARS ("math.rndi", 1, exps);
  return exp_int(sys_rnd_i(exp_rget_int(arr_get(exps, 0))));
}

// \f, i -> f
static Exp *f_round (Arr *exps) {
  CHECK_PARS ("math.round", 2, exps);
  return exp_float(dec_round(
    exp_rget_float(arr_get(exps, 0)),
    exp_rget_int(arr_get(exps, 1))
  ));
}

// \f -> f
static Exp *f_sin (Arr *exps) {
  CHECK_PARS ("math.sin", 1, exps);
  return exp_float(sin(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_sinh (Arr *exps) {
  CHECK_PARS ("math.sinh", 1, exps);
  return exp_float(sinh(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_sqrt (Arr *exps) {
  CHECK_PARS ("math.sqrt", 1, exps);
  return exp_float(sqrt(exp_rget_float(arr_get(exps, 0))));
}

// \s -> [f]|[]
static Exp *stof (Arr *exps) {
  CHECK_PARS ("math.stof", 1, exps);
  return to_float_op_aux(exp_rget_string(arr_get(exps, 0)));
}

// \s -> [i]|[]
static Exp *stoi (Arr *exps) {
  CHECK_PARS ("math.stoi", 1, exps);
  return to_int_op_aux(exp_rget_string(arr_get(exps, 0)), 10);
}

// \f -> f
static Exp *f_tan (Arr *exps) {
  CHECK_PARS ("math.tan", 1, exps);
  return exp_float(tan(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_tanh (Arr *exps) {
  CHECK_PARS ("math.tanh", 1, exps);
  return exp_float(tanh(exp_rget_float(arr_get(exps, 0))));
}

// \f -> f
static Exp *f_trunc (Arr *exps) {
  CHECK_PARS ("math.trunc", 1, exps);
  return exp_float(trunc(exp_rget_float(arr_get(exps, 0))));
}

Bfunction md_math_get (char *fname) {
  // CONSTANTS

  if (!strcmp(fname, "e")) return e;
  if (!strcmp(fname, "pi")) return pi;

  // FUNCTIONS

  if (!strcmp(fname, "abs")) return f_abs;
  if (!strcmp(fname, "acos")) return f_acos;
  if (!strcmp(fname, "acosh")) return f_acosh;
  if (!strcmp(fname, "asin")) return f_asin;
  if (!strcmp(fname, "asinh")) return f_asinh;
  if (!strcmp(fname, "atan")) return f_atan;
  if (!strcmp(fname, "atanh")) return f_atanh;
  if (!strcmp(fname, "ceil")) return f_ceil;
  if (!strcmp(fname, "cos")) return f_cos;
  if (!strcmp(fname, "cosh")) return f_cosh;
  if (!strcmp(fname, "eq")) return eq;
  if (!strcmp(fname, "exp")) return f_exp;
  if (!strcmp(fname, "exp2")) return f_exp2;
  if (!strcmp(fname, "floor")) return f_floor;
  if (!strcmp(fname, "fromEn")) return from_en;
  if (!strcmp(fname, "fromHex")) return from_hex;
  if (!strcmp(fname, "fromIso")) return from_iso;
  if (!strcmp(fname, "ftoi")) return ftoi;
  if (!strcmp(fname, "ftos")) return ftos;
  if (!strcmp(fname, "isDigits")) return is_digits;
  if (!strcmp(fname, "itof")) return itof;
  if (!strcmp(fname, "itoh")) return itoh;
  if (!strcmp(fname, "itos")) return itos;
  if (!strcmp(fname, "log")) return f_log;
  if (!strcmp(fname, "log10")) return f_log10;
  if (!strcmp(fname, "log2")) return f_log2;
  if (!strcmp(fname, "max")) return max;
  if (!strcmp(fname, "maxFloat")) return max_float;
  if (!strcmp(fname, "maxInt")) return max_int;
  if (!strcmp(fname, "min")) return min;
  if (!strcmp(fname, "minInt")) return min_int;
  if (!strcmp(fname, "pow")) return f_pow;
  if (!strcmp(fname, "pow10")) return f_pow10;
  if (!strcmp(fname, "rnd")) return rnd;
  if (!strcmp(fname, "rndi")) return rndi;
  if (!strcmp(fname, "round")) return f_round;
  if (!strcmp(fname, "sin")) return f_sin;
  if (!strcmp(fname, "sinh")) return f_sinh;
  if (!strcmp(fname, "sqrt")) return f_sqrt;
  if (!strcmp(fname, "stof")) return stof;
  if (!strcmp(fname, "stoi")) return stoi;
  if (!strcmp(fname, "tan")) return f_tan;
  if (!strcmp(fname, "tanh")) return f_tanh;
  if (!strcmp(fname, "trunc")) return f_trunc;
  EXC_KUT(fail_bfunction("math", fname));
  return NULL; // Unreachable
}
