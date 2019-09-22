// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modpath.h"
#include "Machine.h"

static void canonical (Machine *m) {
  char *path = opt_nget(
    path_canonical(token_string(machine_pop_exc(m, token_STRING)))
  );
  if (path)
    machine_push(
      m, token_new_list(0, arr_new_from(token_new_string(0, path), NULL))
    );
  else
    machine_push(m, token_new_list(0, arr_new()));
}

static void plus (Machine *m) {
  char *s = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(
    0, path_cat(token_string(machine_pop_exc(m, token_STRING)), s , NULL)
  ));
}

static void extension (Machine *m) {
  machine_push(m, token_new_string(
    0, path_extension(token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void name (Machine *m) {
  machine_push(m, token_new_string(
    0, path_name(token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void only_name (Machine *m) {
  machine_push(m, token_new_string(
    0, path_only_name(token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void parent (Machine *m) {
  machine_push(m, token_new_string(
    0, path_parent(token_string(machine_pop_exc(m, token_STRING)))
  ));
}

// Resturns Map<primitives_Fn>
Map *modpath_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "canonical", canonical); // STRING - OPT<STRING>
  map_put(r, "+", plus); // STRING - STRING
  map_put(r, "extension", extension); // STRING - STRING
  map_put(r, "name", name); // STRING - STRING
  map_put(r, "onlyName", only_name); // STRING - STRING
  map_put(r, "parent", parent); // STRING - STRING

  return r;
}

