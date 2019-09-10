// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modmap.h"
#include "fails.h"
#include "Machine.h"

static void fromStr (Machine *m) {
  machine_fail(m, "Witout implementation");
}

// Resturns Map<primitives_Fn>
Map *modmap_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "map.fromStr", fromStr);

  return r;
}


