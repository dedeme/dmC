// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modmap.h"
#include "fails.h"
#include "tk.h"
#include "primitives/modjs.h"

static void new (Machine *m) {
  machine_push(m, token_from_pointer(symbol_MAP_, map_new()));
}

static void from (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Machine *m2 = machine_isolate_process("", machine_pmachines(m), prg);
  // Arr<Token>
  Arr *a = machine_stack(m2);
  int sz = arr_size(a);
  if (sz % 2) fails_list_size(m, a, sz + 1);

  // Map<Token>
  Map *r = map_new();
  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    char *k = tk_string(m, *tks++);
    map_put(r, k, *tks++);
  }
  machine_push(m, token_from_pointer(symbol_MAP_, r));
}

static void size (Machine *m) {
  Map *mp = tk_pop_native(m, symbol_MAP_);
  machine_push(m, token_new_int(map_size(mp)));
}

static int eqaux (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", machine_pmachines(m), prg);
    return tk_pop_int(m);
  }
  // Map<Token>
  Map *m1 = tk_pop_native(m, symbol_MAP_);
  // Map<Token>
  Map *m2 = tk_pop_native(m, symbol_MAP_);
  if (map_size(m1) != map_size(m2)) return 0;
  EACH(map_kvs(m1), Kv, kv) {
    Token *tk = opt_nget(map_get(m2, kv_key(kv)));
    if (!tk || !fn(kv_value(kv), tk)) return 0;
  }_EACH
  return 1;
}

static void eq (Machine *m) {
  machine_push(m, token_new_int(eqaux(m)));
}

static void neq (Machine *m) {
  machine_push(m, token_new_int(!eqaux(m)));
}

static void get (Machine *m) {
  char *key = tk_pop_string(m);
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  Token *tk = opt_nget(map_get(mp, key));
  if (tk) machine_push(m, tk);
  else machine_fail(m, str_f("Key '%s' not found", key));
}

static void oget (Machine *m) {
  char *key = tk_pop_string(m);
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  Token *tk = opt_nget(map_get(mp, key));
  if (tk) machine_push(m, token_new_list(arr_new_from(tk, NULL)));
  else machine_push(m, token_new_list(arr_new()));
}

static void haskey (Machine *m) {
  char *key = tk_pop_string(m);
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  machine_push(m, token_new_int(map_has_key(mp, key)));
}

static void putboth (Machine *m, int isplus) {
  Token *tk = machine_pop(m);
  char *key = tk_pop_string(m);
  // Map<Token>
  Map *mp = isplus
    ? tk_peek_native(m, symbol_MAP_)
    : tk_pop_native(m, symbol_MAP_)
  ;

  map_put(mp, key, tk);
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
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  map_remove(mp, key);
}

static void keys (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  // Arr<Token>
  Arr *r = arr_new();
  EACH(map_keys(mp), char, k) {
    arr_push(r, token_new_string(k));
  }_EACH
  machine_push(m, token_new_list(r));
}

static void values (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);

  // Arr<Token>
  Arr *r = arr_new();
  EACH(map_kvs(mp), Kv, kv) {
    arr_push(r, kv_value(kv));
  }_EACH
  machine_push(m, token_new_list(r));
}

// Map<Token>. Return Arr<Token> where Token is List<String, *>
static Arr *pairsaux (Map *mp) {
  // Arr<Token>
  Arr *a = arr_new();
  EACH(mp, Kv, kv) {
    arr_push(a, token_new_list(
      arr_new_from(token_new_string(kv_key(kv)), kv_value(kv), NULL)
    ));
  }_EACH
  return a;
}

static void pairs (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  machine_push(m, token_new_list(pairsaux(mp)));
}

static void sort (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  map_sort(mp);
  machine_push(m, token_new_list(pairsaux(mp)));
}

static void sortlocale (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  map_sort_locale(mp);
  machine_push(m, token_new_list(pairsaux(mp)));
}

static void copy (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  machine_push(m, token_from_pointer(symbol_MAP_, arr_copy((Arr *)mp)));
}

static void to (Machine *m) {
  // Map<Token>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  // Arr<Token>
  Arr *a = arr_new();
  EACH(mp, Kv, kv) {
    arr_push(a, token_new_string(kv_key(kv)));
    arr_push(a, kv_value(kv));
  }_EACH
  machine_push(m, token_new_list(a));
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
  add("from", from); // LIST - MAP
  add("size", size); // [MAP] - INT
  add("eq?", eq); // [MAP, MAP] - INT
  add("neq?", neq); // [MAP, MAP] - INT
  add("get", get); // [MAP, STRING] - *
  add("oget", oget); // [MAP, STRING] - LIST  ([map, key] - OPT)
  add("has?", haskey); // [MAP, STRING] - INT
  add("put", put); // [MAP - STRING - *] - []
  add("put+", putplus); // [MAP - STRING - *] - MAP
  add("up", up); // [MAP - STRING - LIST] - []
  add("up+", upplus); // [MAP - STRING - LIST] - MAP
  add("remove", mremove); // [MAP, STRING] - []
  add("keys", keys); // MAP - LIST
  add("values", values); // MAP - LIST
  add("pairs", pairs); // MAP - LIST (map - list<list<key, value>>
  add("sort", sort); // MAP - LIST (map - list<list<key, value>>
  add("sortLocale", sortlocale); // MAP - LIST (map - list<list<key, value>>
  add("copy", copy); // MAP - MAP
  add("to", to); // MAP - LIST

  add("fromJs", fromjs);
  add("toJs", tojs);

  return r;
}


