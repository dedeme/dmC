// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/dicLayers.h"
#include "kut/map.h"

DicLayers *dicLayers_new (void) {
  return arr_new_from(map_new(), NULL);
}

void dicLayers_add_layer (DicLayers *this) {
  arr_push(this, map_new());
}

Opt *dicLayers_get_element (DicLayers *this, char *key) {
  if (*key) {
    EACHR(this, Map, m) {
      void *e = opt_get(map_get(m, key));
      if (e) return opt_some(e);
    }_EACH
  }
  return opt_none();
}

Map *dicLayers_peek_layer (DicLayers *this) {
  return arr_peek(this);
}

void dicLayers_put_element (DicLayers *this, char *key, void *value) {
  map_put(dicLayers_peek_layer(this), key, value);
}

void dicLayers_remove_layer (DicLayers *this) {
  arr_pop(this);
}
