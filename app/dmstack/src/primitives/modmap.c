// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modmap.h"
#include "fails.h"
#include "tk.h"
#include "primitives/modjs.h"

// Tp<Token>
static char *kvk (Machine *m, Token *tk) {
  // Arr<Token>
  Arr *a = tk_list(m, tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  return tk_string(m, *arr_start(a));
}

static void new (Machine *m) {
  machine_push(m, token_new_list(0, arr_new()));
}

static void get (Machine *m) {
  char *key = tk_pop_string(m);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  Token *tk = opt_nget(it_find(it_from(a), (FPRED)fn));
  if (tk) {
    machine_push(m, token_new_list(0, arr_new_from(
      arr_get(token_list(tk), 1), NULL)
    ));
  } else {
    machine_push(m, token_new_list(0, arr_new()));
  }
}

static void haskey (Machine *m) {
  char *key = tk_pop_string(m);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  machine_push(m, token_new_int(0, arr_index(a, (FPRED)fn) != -1));
}

static void putboth (Machine *m, int isplus) {
  Token *tk = machine_pop(m);
  char *key = tk_pop_string(m);
  // Arr<Token>
  Arr *a = isplus
    ? tk_peek_list(m)
    : tk_pop_list(m)
  ;
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  Token *t = opt_nget(it_find(it_from(a), (FPRED)fn));
  if (t)
    arr_set(token_list(t), 1, tk);
  else
    arr_push(a, token_new_list(0, arr_new_from(
      token_new_string(0, key), tk, NULL
    )));
}

static void put (Machine *m) {
  putboth(m, 0);
}

static void putplus (Machine *m) {
  putboth(m, 1);
}

static void upboth (Machine *m, int isplus) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_peek(m);
  machine_push(m, tk2);
  get(m);
  // Arr<Token>
  Arr *a =tk_pop_list(m);
  if (!arr_size(a))
    machine_fail(m, str_f("Key '%s' not found", token_string(tk2)));
  machine_push(m, *arr_start(a));

  machine_process("", machine_pmachines(m), prg);

  Token *tk3 = machine_pop(m);
  machine_push(m, tk1);
  machine_push(m, tk2);
  machine_push(m, tk3);
  putboth(m, isplus);
}

static void up (Machine *m) {
  upboth(m, 0);
}

static void upplus (Machine *m) {
  upboth(m, 1);
}

static void mremove (Machine *m) {
  char *key = tk_pop_string(m);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int fn (Token *tk) { return !str_eq(kvk(m, tk), key); }
  arr_filter_in(a, (FPRED)fn);
}

static void keys (Machine *m) {
  // Arr<Token>
  Arr *r = arr_new();
  EACH(tk_pop_list(m), Token, tk) {
    arr_push(r, token_new_string(0, kvk(m, tk)));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void values (Machine *m) {
  // Arr<Token>
  Arr *r = arr_new();
  EACH(tk_pop_list(m), Token, tk) {
    // Arr<Token>
    Arr *a = tk_list(m, tk);
    if (arr_size(a) != 2) fails_size_list(m, a, 2);
    arr_push(r, arr_get(a, 1));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void sortboth (Machine *m, int (*greater)(Token *e1, Token *e2)) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  arr_sort(a, (FCMP)greater);
}

static void sort (Machine *m) {
  int greater (Token *e1, Token *e2) {
    return strcmp(kvk(m, e1), kvk(m, e2)) > 0;
  }
  sortboth(m, greater);
}

static void sortlocale (Machine *m) {
  int greater (Token *e1, Token *e2) {
    return strcoll(kvk(m, e1), kvk(m, e2)) > 0;
  }
  sortboth(m, greater);
}

static void fromjs (Machine *m) {
  modjs_to_map(m);
}

static void tojs (Machine *m) {
  modjs_from_map(m);
}

Pmodule *modmap_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // [] - MAP
  add("get", get); // [MAP, STRING] - LIST
                          // ([map, key] - [unary list] or [empty list])
  add("hasKey", haskey); // [MAP, STRING] - INT
  add("put", put); // [MAP - STRING - *] - []
  add("put+", putplus); // [MAP - STRING - *] - MAP
  add("up", up); // [MAP - STRING - LIST] - []
  add("up+", upplus); // [MAP - STRING - LIST] - MAP
  add("remove", mremove); // [MAP, STRING] - []
  add("keys", keys); // MAP - LIST
  add("values", values); // MAP - LIST
  add("sort", sort); // MAP - []
  add("sortLocale", sortlocale); // MAP - []

  add("fromJs", fromjs);
  add("toJs", tojs);

  return r;
}


