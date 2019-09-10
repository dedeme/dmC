// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives.h"
#include "modules/modsys.h"
#include "modules/modstk.h"
#include "modules/modblob.h"
#include "modules/modint.h"
#include "modules/modfloat.h"
#include "modules/modmath.h"
#include "modules/modlist.h"
#include "modules/modmap.h"
#include "modules/modstr.h"

/// Map<Map<primitives_Fn>>
static Map *modules = NULL;

void primitives_init (void) {
  modules = map_new();
  map_put(modules, "sys", modsys_mk());
  map_put(modules, "stk", modstk_mk());
  map_put(modules, "blob", modblob_mk());
  map_put(modules, "int", modint_mk());
  map_put(modules, "float", modfloat_mk());
  map_put(modules, "math", modmath_mk());
  map_put(modules, "lst", modlist_mk());
  map_put(modules, "map", modmap_mk());
  map_put(modules, "str", modstr_mk());
}

/// Returns Opt<primitives_Fn>
Opt *primitives_get (char *module, char *id) {
  return map_get(opt_oget(map_get(modules, module), map_new()), id);
}

// Returns Opt<Map<Token>>
Opt *primitives_module (char *module) {
  // Map<primitives_Fn>
  Map *fns = opt_nget(map_get(modules, module));
  if (fns) {
    // Map<Token>
    Map *r = map_new();
    EACH(map_keys(fns), char, k) {
      map_put(r, k, token_new_list(arr_new_from(
        token_new_string(module),
        token_new_string(k),
        token_new_symbol(symbol_new("mrun")),
        NULL
      )));
    }_EACH
    return opt_new(r);
  }
  return opt_empty();
}
