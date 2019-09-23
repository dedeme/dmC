// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives.h"
#include "primitives/modglobal.h"
#include "primitives/modstk.h"
#include "primitives/modblob.h"
#include "primitives/modint.h"
#include "primitives/modfloat.h"
#include "primitives/modlong.h"
#include "primitives/moddec.h"
#include "primitives/modmath.h"
#include "primitives/modlist.h"
#include "primitives/modtp.h"
#include "primitives/modmap.h"
#include "primitives/modobj.h"
#include "primitives/modstr.h"
#include "primitives/modsys.h"
#include "primitives/modjs.h"
#include "primitives/modb64.h"
#include "primitives/modcryp.h"
#include "primitives/modtime.h"
#include "primitives/modclock.h"
#include "primitives/modpath.h"
#include "primitives/modfile.h"
#include "primitives/modiserver.h"

/// Map<Map<primitives_Fn>>
static Map *modules = NULL;

void primitives_init (void) {
  modules = map_new();
  map_put(modules, "", modglobal_mk());
  map_put(modules, "stk", modstk_mk());
  map_put(modules, "blob", modblob_mk());
  map_put(modules, "int", modint_mk());
  map_put(modules, "float", modfloat_mk());
  map_put(modules, "long", modlong_mk());
  map_put(modules, "dec", moddec_mk());
  map_put(modules, "math", modmath_mk());
  map_put(modules, "lst", modlist_mk());
  map_put(modules, "tp", modtp_mk());
  map_put(modules, "map", modmap_mk());
  map_put(modules, "obj", modobj_mk());
  map_put(modules, "str", modstr_mk());
  map_put(modules, "sys", modsys_mk());
  map_put(modules, "js", modjs_mk());
  map_put(modules, "b64", modb64_mk());
  map_put(modules, "cryp", modcryp_mk());
  map_put(modules, "time", modtime_mk());
  map_put(modules, "clock", modclock_mk());
  map_put(modules, "path", modpath_mk());
  map_put(modules, "file", modfile_mk());
  map_put(modules, "iserver", modiserver_mk());
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
      map_put(r, k, token_new_list(0, arr_new_from(
        token_new_string(0, module),
        token_new_string(0, k),
        token_new_symbol(0, symbol_new("mrun")),
        NULL
      )));
    }_EACH
    return opt_new(r);
  }
  return opt_empty();
}
