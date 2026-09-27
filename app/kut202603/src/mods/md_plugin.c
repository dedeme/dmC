// Copyright 03-Feb-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include <dlfcn.h>
#include "mods/md_plugin.h"
#include "runner/fail.h"
#include "iarr.h"
#include "function.h"

// \<plugin> -> ()
static Exp *close (Arr *exps) {
  CHECK_PARS ("plugin.close", 1, exps);
  void *handle = exp_get_object("<plugin>", arr_get(exps, 0));
  dlclose(handle);
  return exp_empty();
}

// pars is Arr<Exp>
static void run_closure (Exp *efn, Arr *pars) {
  Function *fn = exp_get_function(efn);
  function_run(fn, pars);
}

// \ -> <pclosure>
static Exp *closure (Arr *exps) {
  CHECK_PARS ("plugin.closure", 0, exps);
  return exp_object("<plclosure>", run_closure);
}

// \<plugin>, s, Arr<Exp> -> *
static Exp *exec (Arr *exps) {
  CHECK_PARS ("plugin.exec", 3, exps);
  void *handle = exp_get_object("<plugin>", arr_get(exps, 0));
  char *fn = exp_get_string(arr_get(exps, 1));
  // Arr<Exp>
  Arr *pars = exp_get_array(arr_get(exps, 2));
  Bfunction bfn = (Bfunction) dlsym(handle, fn);
  if (!bfn) EXC_KUT(dlerror());
  return bfn(pars);
}

// \s -> <plugin>
static Exp *open (Arr *exps) {
  CHECK_PARS ("plugin.open", 1, exps);
  char *path = exp_get_string(arr_get(exps, 0));
  void *handle = dlopen(path, RTLD_NOW);
  if (!handle) EXC_KUT(dlerror());
  return exp_object("<plugin>", handle);
}

Bfunction md_plugin_get (char *fname) {
  if (!strcmp(fname, "close")) return close;
  if (!strcmp(fname, "closure")) return closure;
  if (!strcmp(fname, "exec")) return exec;
  if (!strcmp(fname, "open")) return open;

  EXC_KUT(fail_bfunction("plugin", fname));
  return NULL; // Unreachable
}
