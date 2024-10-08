// Copyright 07-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_thread.h"
#include "exp.h"
#include "function.h"
#include "obj.h"
#include "runner/fail.h"
#include "runner/runner.h"

static void thfn (void *fn) {
  TRY {
    function_run((Function *)fn, arr_new());
  } CATCH(e) {
    char *msg = exc_msg(e);
    msg = str_right(msg, str_index(msg, ": ") + 2);
    puts(msg);
    exit(0);
  }_TRY
}

static void thbf (void *fn) {
  TRY {
    ((Bfunction)fn)(arr_new());
  } CATCH(e) {
    char *msg = exc_msg(e);
    msg = str_right(msg, str_index(msg, ": ") + 2);
    puts(msg);
    exit(0);
  }_TRY
}

// \<trhead> -> ()
static Exp *join (Arr *exps) {
  CHECK_PARS ("thread.join", 1, exps);
  thread_join(obj_get_thread(arr_get(exps, 0)));
  return exp_empty();
}

// \(\->()) -> ()
static Exp *run (Arr *exps) {
  CHECK_PARS ("thread.run", 1, exps);
  Exp *exp = arr_get(exps, 0);
  if (exp_is_function(exp)) {
    Function *fn = exp_get_function(exp);
    thread_run2(thfn, fn);
    return exp_empty();
  }
  if (obj_is_bfunction(exp)) {
    Bfunction fn = obj_get_bfunction(exp);
    thread_run2(thbf, fn);
    return exp_empty();
  }
  EXC_KUT(fail_type("function", exp));
  return NULL; // Unreachable
}

// \(\->()) -> <thread>
static Exp *start (Arr *exps) {
  CHECK_PARS ("thread.start", 1, exps);
  Exp *exp = arr_get(exps, 0);
  if (exp_is_function(exp)) {
    Function *fn = exp_get_function(exp);
    return obj_thread(thread_start2(thfn, fn));
  }
  if (obj_is_bfunction(exp)) {
    Bfunction fn = obj_get_bfunction(exp);
    return obj_thread(thread_start2(thbf, fn));
  }
  EXC_KUT(fail_type("function", exp));
  return NULL; // Unreachable
}

// \(\->()) -> ()
static Exp *sync (Arr *exps) {
  CHECK_PARS ("thread.sync", 1, exps);
  Exp *fn = arr_get(exps, 0);
    //--
    void fn2(void) {
      // Exp
      Arr *ps = arr_new();
      if (exp_is_function(fn)) function_run(exp_get_function(fn), ps);
      else if (obj_is_bfunction(fn)) obj_get_bfunction(fn)(ps);
      else EXC_KUT(fail_type("function", fn));
    }
    thread_sync(fn2);
    return exp_empty();
}

Bfunction md_thread_get (char *fname) {
  if (!strcmp(fname, "join")) return join;
  if (!strcmp(fname, "run")) return run;
  if (!strcmp(fname, "start")) return start;
  if (!strcmp(fname, "sync")) return sync;

  EXC_KUT(fail_bfunction("thread", fname));
  return NULL; // Unreachable
}
