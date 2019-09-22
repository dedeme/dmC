// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfile.h"
#include <unistd.h>
#include "Machine.h"

static void cwd (Machine *m) {
  machine_push(m, token_new_string(0, file_cwd()));
}

// Resturns Map<primitives_Fn>
Map *modfile_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "cwd", cwd);

  return r;
}




