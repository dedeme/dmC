// Copyright 11-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules.h"
#include "modules/modit.h"

/// Map<Map<Token>>
static Map *modules = NULL;

void modules_init (void) {
  modules = map_new();
  map_put(modules, "it", modit_mk());
}

Opt *modules_module (char *module) {
  return map_get(modules, module);
}
