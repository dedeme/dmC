// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modpath.h"
#include "tk.h"

static void canonical (Machine *m) {
  char *path = opt_nget(
    path_canonical(tk_pop_string(m))
  );
  if (path)
    machine_push(
      m, token_new_list(arr_new_from(token_new_string(path), NULL))
    );
  else
    machine_push(m, token_new_list(arr_new()));
}

static void plus (Machine *m) {
  char *s = tk_pop_string(m);
  machine_push(m, token_new_string(
    path_cat(tk_pop_string(m), s , NULL)
  ));
}

static void cat (Machine *m) {
  // Arr<!Token>
  Arr *a = tk_pop_list(m);
  char *fn (Token *tk) { return tk_string(m, tk); }
  char *r = str_cjoin(arr_map(a, (FCOPY)fn), '/');
  if (*r == '/') r = str_right(r, 1);
  machine_push(m, token_new_string(r));
}

static void extension (Machine *m) {
  machine_push(m, token_new_string(
    path_extension(tk_pop_string(m))
  ));
}

static void name (Machine *m) {
  machine_push(m, token_new_string(
    path_name(tk_pop_string(m))
  ));
}

static void only_name (Machine *m) {
  machine_push(m, token_new_string(
    path_only_name(tk_pop_string(m))
  ));
}

static void parent (Machine *m) {
  machine_push(m, token_new_string(
    path_parent(tk_pop_string(m))
  ));
}

Pmodule *modpath_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("canonical", canonical); // STRING - OPT<STRING>
  add("+", plus); // STRING - STRING
  add("cat", cat); // LIST - STRING
  add("extension", extension); // STRING - STRING
  add("name", name); // STRING - STRING
  add("onlyName", only_name); // STRING - STRING
  add("parent", parent); // STRING - STRING

  return r;
}

