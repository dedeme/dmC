// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modmath.h"
#include <math.h>
#include "fails.h"
#include "Machine.h"

static void fn1 (Machine *m, double (*f)(double)) {
  machine_push(m, token_new_float(
    f(token_float(machine_pop_exc(m, token_FLOAT)))
  ));
}

static void fn2 (Machine *m, double (*f)(double, double)) {
  double n2 = token_float(machine_pop_exc(m, token_FLOAT));
  double n1 = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_float(f(n1, n2)));
}

static void pi (Machine *m) {
  machine_push(m, token_new_float(M_PI));
}

static void ssin (Machine *m) {
  fn1(m, sin);
}

static void scos (Machine *m) {
  fn1(m, cos);
}

static void stan (Machine *m) {
  fn1(m, tan);
}

static void sasin (Machine *m) {
  fn1(m, asin);
}

static void sacos (Machine *m) {
  fn1(m, acos);
}

static void satan (Machine *m) {
  fn1(m, atan);
}

static void satan2 (Machine *m) {
  fn2(m, atan2);
}

static void e (Machine *m) {
  machine_push(m, token_new_float(M_E));
}

static void sexp (Machine *m) {
  fn1(m, exp);
}

static void sexp2 (Machine *m) {
  fn1(m, exp2);
}

static void sexp10 (Machine *m) {
  double exp10(double e) { return pow(10, e); }
  fn1(m, exp10);
}

static void slog (Machine *m) {
  fn1(m, log);
}

static void slog2 (Machine *m) {
  fn1(m, log2);
}

static void slog10 (Machine *m) {
  fn1(m, log10);
}

static void spow (Machine *m) {
  fn2(m, pow);
}

static void ssqrt (Machine *m) {
  fn1(m, sqrt);
}

static void scbrt (Machine *m) {
  fn1(m, cbrt);
}

// Resturns Map<primitives_Fn>
Map *modmath_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "math.PI", pi);
  map_put(r, "math.sin", ssin);
  map_put(r, "math.cos", scos);
  map_put(r, "math.tan", stan);
  map_put(r, "math.asin", sasin);
  map_put(r, "math.acos", sacos);
  map_put(r, "math.atan", satan);
  map_put(r, "math.atan2", satan2);

  map_put(r, "math.E", e);
  map_put(r, "math.exp", sexp);
  map_put(r, "math.exp2", sexp2);
  map_put(r, "math.exp10", sexp10);
  map_put(r, "math.log", slog);
  map_put(r, "math.log2", slog2);
  map_put(r, "math.log10", slog10);

  map_put(r, "math.pow", spow);
  map_put(r, "math.sqrt", ssqrt);
  map_put(r, "math.cbrt", scbrt);

  return r;
}


