// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_sys.h"
#include <unistd.h>
#include "kut/rs.h"
#include "kut/sys.h"
#include "main.h"
#include "exp.h"
#include "exmodule.h"
#include "reader/cdr/cdr.h"
#include "reader/ex_reader.h"
#include "reader/types.h"
#include "runner/fail.h"
#include "runner/solver.h"

// \-> [s...]
static Exp *args (Arr *exps) {
  CHECK_PARS ("sys.args", 0, exps);
  // <Exp>
  Arr *a = arr_new();
  EACH(main_args(), char, arg) {
    if (_i > 1) arr_push(a, exp_string(arg));
  }_EACH
  return exp_array(a);
}

// \b -> ()
static Exp *fassert (Arr *exps) {
  CHECK_PARS ("sys.assert", 1, exps);
  if (exp_rget_bool(arr_get(exps, 0))) return exp_empty();
  EXC_KUT("Assert failed");
  return NULL; // Unreachable
}

// \s, * -> *
static Exp *c (Arr *exps) {
  CHECK_PARS ("sys.c", 2, exps);
  return exmodule_run(exp_rget_string(arr_get(exps, 0)), arr_get(exps, 1));
}

// \s, [s...] -> [s, b]
static Exp *cmd (Arr *exps) {
  CHECK_PARS ("sys.cmd", 2, exps);
  char *c = exp_rget_string(arr_get(exps, 0));
    //--
    char *fn (Exp *e) { return str_to_escape(exp_rget_string(e)); }
  // <char>
  Arr *ps = arr_map(exp_rget_array(arr_get(exps, 1)), (FMAP)fn);
  arr_insert(ps, 0, c);
  // <char>
  Rs *rs = sys_cmd(arr_join(ps, " "));
  char *r = rs_get(rs);
  if (r) return exp_array(arr_new_from(exp_string(r), exp_bool(1), NULL));
  return  exp_array(arr_new_from(exp_string(rs_error(rs)), exp_bool(0), NULL));
}

// \s, [s...] -> [s, s]
static Exp *cmd2 (Arr *exps) {
  CHECK_PARS ("sys.cmd", 2, exps);
  char *c = exp_rget_string(arr_get(exps, 0));
    //--
    char *fn (Exp *e) { return str_to_escape(exp_rget_string(e)); }
  // <char>
  Arr *ps = arr_map(exp_rget_array(arr_get(exps, 1)), (FMAP)fn);
  arr_insert(ps, 0, c);
  // <<char>, <char>
  Tp *tp = sys_cmd2(arr_join(ps, " "));

  return exp_array(arr_new_from(
    exp_string(tp_e1(tp)),
    exp_string(tp_e2(tp)),
    NULL
  ));
}

// \-> {s}
static Exp *environ (Arr *exps) {
  CHECK_PARS ("sys.environ", 0, exps);
  // <char>
  Map *env = sys_environ();
  // <Kv<Exp>>
  Arr *env2 = arr_new();
  // e is Kv<char>
  EACH(map_to_array(env), Kv, e) {
    arr_push(env2, kv_new(kv_key(e), exp_string(kv_value(e))));
  }_EACH
  return exp_dic(map_from_array(env2));
}

// \s -> * | ()
static Exp *eval (Arr *exps) {
  CHECK_PARS ("sys.eval", 1, exps);
  Cdr *cdr = cdr_new(-1, exp_rget_string(arr_get(exps, 0)));
  Exp *exp = ex_reader_read(types_new(), cdr);
  return solver_solve_isolate(exp);
}

// \i -> ()
static Exp *fexit (Arr *exps) {
  CHECK_PARS ("sys.exit", 1, exps);
  exit(exp_rget_int(arr_get(exps, 0)));
}

// \-> s
static Exp *home (Arr *exps) {
  CHECK_PARS ("sys.home", 0, exps);
  return exp_string(sys_user_home());
}

// \-> s
static Exp *get_locale (Arr *exps) {
  CHECK_PARS ("sys.getLocale", 0, exps);
  return exp_string(sys_get_locale());
}

// \-> s
static Exp *main_path (Arr *exps) {
  CHECK_PARS ("sys.mainPath", 0, exps);
  return exp_string(str_f("%s.kut", arr_get(main_args(), 1)));
}

// \ -> i
static Exp *pid (Arr *exps) {
  CHECK_PARS ("sys.pid", 0, exps);
  return exp_int(getpid());
}

// \* -> ()
static Exp *print (Arr *exps) {
  CHECK_PARS ("sys.print", 1, exps);
  printf("%s", (exp_to_str(arr_get(exps, 0))));
  return exp_empty();
}

// \* -> ()
static Exp *print_error (Arr *exps) {
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

// \-> s
static Exp *read_line (Arr *exps) {
  CHECK_PARS ("sys.readLine", 0, exps);
  return exp_string(sys_read_line());
}

// \s-> ()
static Exp *set_locale (Arr *exps) {
  CHECK_PARS ("sys.setLocale", 1, exps);
  sys_set_locale(exp_rget_string(arr_get(exps, 0)));
  return exp_empty();
}

// \i-> ()
static Exp *f_sleep (Arr *exps) {
  CHECK_PARS ("sys.sleep", 1, exps);
  sys_sleep(exp_rget_int(arr_get(exps, 0)));
  return exp_empty();
}

// \*, * -> ()
static Exp *test (Arr *exps) {
  CHECK_PARS ("sys.test", 2, exps);
  Exp *ex0 = arr_get(exps, 0);
  Exp *ex1 = arr_get(exps, 1);
  if (exp_rget_bool(solver_solve_isolate(exp_eq(ex0, ex1)))) return exp_empty();

  EXC_KUT(str_f("Test failed.\n  Expected: %s\n    Actual: %s",
    exp_to_js(ex1), exp_to_js(ex0)
  ));
  return NULL; // Unreachable
}

// \s -> ()
static Exp *throw (Arr *exps) {
  CHECK_PARS ("sys.throw", 1, exps);
  EXC_KUT(exp_rget_string(arr_get(exps, 0)));
  return NULL; // Unreachable
}

// \* -> s
static Exp *to_str (Arr *exps) {
  CHECK_PARS ("sys.toStr", 1, exps);
  return exp_string(exp_to_str(arr_get(exps, 0)));
}

// \* -> s
static Exp *type (Arr *exps) {
  CHECK_PARS ("sys.type", 1, exps);
  return exp_string(exp_type_to_str(arr_get(exps, 0)));
}

// \-> s
static Exp *user (Arr *exps) {
  CHECK_PARS ("sys.user", 0, exps);
  return exp_string(sys_user_name());
}

Bfunction md_sys_get (char *fname) {
  if (!strcmp(fname, "args")) return args;
  if (!strcmp(fname, "assert")) return fassert;
  if (!strcmp(fname, "c")) return c;
  if (!strcmp(fname, "cmd")) return cmd;
  if (!strcmp(fname, "cmd2")) return cmd2;
  if (!strcmp(fname, "environ")) return environ;
  if (!strcmp(fname, "eval")) return eval;
  if (!strcmp(fname, "exit")) return fexit;
  if (!strcmp(fname, "getLocale")) return get_locale;
  if (!strcmp(fname, "home")) return home;
  if (!strcmp(fname, "mainPath")) return main_path;
  if (!strcmp(fname, "pid")) return pid;
  if (!strcmp(fname, "print")) return print;
  if (!strcmp(fname, "printError")) return print_error;
  if (!strcmp(fname, "println")) return println;
  if (!strcmp(fname, "readLine")) return read_line;
  if (!strcmp(fname, "setLocale")) return set_locale;
  if (!strcmp(fname, "sleep")) return f_sleep;
  if (!strcmp(fname, "test")) return test;
  if (!strcmp(fname, "throw")) return throw;
  if (!strcmp(fname, "toStr")) return to_str;
  if (!strcmp(fname, "type")) return type;
  if (!strcmp(fname, "user")) return user;
  EXC_KUT(fail_bfunction("sys", fname));
  return NULL; // Unreachable
}
