// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives.h"
#include "primitives/modglobal.h"
#include "primitives/modstk.h"
#include "primitives/modblob.h"
#include "primitives/modint.h"
#include "primitives/modfloat.h"
#include "primitives/modmath.h"
#include "primitives/modlist.h"
#include "primitives/modwrap.h"
#include "primitives/modmap.h"
#include "primitives/modobj.h"
#include "primitives/modstr.h"
#include "primitives/modsys.h"
#include "primitives/modjs.h"
#include "primitives/modexc.h"
#include "primitives/modb64.h"
#include "primitives/modcryp.h"
#include "primitives/modtime.h"
#include "primitives/modclock.h"
#include "primitives/modpath.h"
#include "primitives/modit.h"
#include "primitives/modfile.h"
#include "primitives/modiserver.h"

struct primitives_Entry {
  Symbol id;
  Pmodule *mod;
};

/// Arr<struct primitives_Entry>
static Arr *modules = NULL;

static void add (char *name, Pmodule *mod) {
  struct primitives_Entry *e = MALLOC(struct primitives_Entry);
  e->id = symbol_new(name);
  e->mod = mod;
  arr_push(modules, e);
}

void primitives_init (void) {
  modules = arr_new();
  add("", modglobal_mk());  // Must go at the first position!!!

  add("stk", modstk_mk());
  add("blob", modblob_mk());
  add("int", modint_mk());
  add("float", modfloat_mk());
  add("math", modmath_mk());
  add("lst", modlist_mk());
  add("wrap", modwrap_mk());
  add("map", modmap_mk());
  add("obj", modobj_mk());
  add("str", modstr_mk());
  add("sys", modsys_mk());
  add("js", modjs_mk());
  add("exc", modexc_mk());
  add("b64", modb64_mk());
  add("cryp", modcryp_mk());
  add("time", modtime_mk());
  add("clock", modclock_mk());
  add("path", modpath_mk());
  add("it", modit_mk());
  add("file", modfile_mk());
  add("iserver", modiserver_mk());
}

/// Returns Opt<pmodule_Fn>
Opt *primitives_get (Symbol module, Symbol fn) {
  EACH(modules, struct primitives_Entry, e) {
    if (symbol_eq(module, e->id)) {
      pmodule_Fn pfn = opt_nget(pmodule_get(e->mod, fn));
      if (pfn) return opt_new(pfn);
      return opt_empty();
    }
  }_EACH
  return opt_empty();
}

void primitives_add_base (Heap *heap) {
  Symbol module = symbol_new("");
  struct primitives_Entry *global = *arr_start(modules);
  if (global->id != module)
    EXC_ILLEGAL_STATE("'global->id' is not symbol_new(\"\")")
  EACH(pmodule_list(global->mod), PmoduleEntry, e) {
    Symbol fn = pmoduleEntry_id(e);
    Token *tk = token_new_list(arr_new_from(
      token_new_list(arr_new_from(
        token_new_symbol(module), token_new_symbol(fn), NULL
      )),
      token_new_symbol(symbol_MRUN),
      NULL
    ));
    heap_add(heap, fn, tk);
  }_EACH
}

void primitives_add_lib (Lib *lib) {
  RANGE(i, 1, arr_size(modules)) {
    struct primitives_Entry *module = arr_get(modules, i);
    Heap *heap = heap_new();
    EACH(pmodule_list(module->mod), PmoduleEntry, e) {
      Symbol fn = pmoduleEntry_id(e);
      Token *tk = token_new_list(arr_new_from(
        token_new_list(arr_new_from(
          token_new_symbol(module->id), token_new_symbol(fn), NULL
        )),
        token_new_symbol(symbol_MRUN),
        NULL
      ));
      heap_add(heap, fn, tk);
    }_EACH
    lib_add(lib, module->id, heap);
  }_RANGE
}
