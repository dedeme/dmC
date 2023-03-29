// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_sys.h"
#include "exp.h"
#include "DEFS.h"
#include "reader/cdr/cdr.h"
#include "reader/ex_reader.h"
#include "runner/fail.h"
#include "runner/solver.h"

// \b -> ()
static Exp *fassert (Arr *exps) {
  CHECK_PARS ("sys.assert", 1, exps);
  if (exp_rget_bool(arr_get(exps, 0))) return exp_empty();
  EXC_KUT("Assert failed");
  return NULL; // Unreachable
}

// \s -> * | ()
static Exp *eval (Arr *exps) {
  CHECK_PARS ("sys.eval", 1, exps);
  Cdr *cdr = cdr_new(-1, exp_rget_string(arr_get(exps, 0)));
  Exp *exp = ex_reader_read(cdr);
  return solver_solve_isolate(exp);
}

// \* -> ()
static Exp *print (Arr *exps) {
  CHECK_PARS ("sys.print", 1, exps);
  printf("%s", (exp_to_str(arr_get(exps, 0))));
  return exp_empty();
}

// \* -> ()
static Exp *printError (Arr *exps) {
  CHECK_PARS ("sys.printError", 1, exps);
  fprintf(stderr, "%s", (exp_to_str(arr_get(exps, 0))));
  return exp_empty();
}

// \* -> ()
static Exp *println (Arr *exps) {
  CHECK_PARS ("sys.println", 1, exps);
  puts(exp_to_str(arr_get(exps, 0)));
  return exp_empty();
}

// \*, * -> ()
static Exp *test (Arr *exps) {
  CHECK_PARS ("sys.test", 2, exps);
  Exp *ex0 = arr_get(exps, 0);
  Exp *ex1 = arr_get(exps, 1);
  if (exp_rget_bool(solver_solve_isolate(exp_eq(ex0, ex1)))) return exp_empty();

  EXC_KUT(str_f("Test failed.\n  Expected: %s (%s)\n    Actual: %s (%s)",
    exp_to_str(ex1), exp_type_to_str(ex1),
    exp_to_str(ex0), exp_type_to_str(ex0)
  ));
  return NULL; // Unreachable
}

// \* -> ()
static Exp *throw (Arr *exps) {
  CHECK_PARS ("sys.println", 1, exps);
  EXC_KUT(exp_to_str(arr_get(exps, 0)));
  return NULL; // Unreachable
}

// \* -> s
static Exp *to_str (Arr *exps) {
  CHECK_PARS ("sys.toStr", 1, exps);
  return exp_string(exp_to_str(arr_get(exps, 0)));
}

// \* -> s
static Exp *type (Arr *exps) {
  CHECK_PARS ("sys.type", 0, exps);
  return exp_string(exp_type_to_str(arr_get(exps, 0)));
}

Bfunction md_sys_get (char *fname) {
  if (!strcmp(fname, "assert")) return fassert;
  if (!strcmp(fname, "eval")) return eval;
  if (!strcmp(fname, "print")) return print;
  if (!strcmp(fname, "printError")) return printError;
  if (!strcmp(fname, "println")) return println;
  if (!strcmp(fname, "test")) return test;
  if (!strcmp(fname, "throw")) return throw;
  if (!strcmp(fname, "toStr")) return to_str;
  if (!strcmp(fname, "type")) return type;
  EXC_KUT(fail_bfunction("sys", fname));
  return NULL; // Unreachable
}
