// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modobj.h"
#include "fails.h"
#include "tk.h"

static void new (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Machine *m2 = machine_isolate_process("", machine_pmachines(m), prg);
  // Arr<Token>
  Arr *a = machine_stack(m2);
  int sz = arr_size(a);
  if (sz % 2) fails_list_size(m, a, sz + 1);

  // Arr<char | Token>
  Arr *r = arr_new();
  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    arr_push(r, tk_string(m, *tks++));
    arr_push(r, *tks++);
  }
  machine_push(m, token_from_pointer(symbol_OBJECT_, r));
}

static void size (Machine *m) {
  // Arr<char | Token>
  Arr *a = tk_pop_native(m, symbol_OBJECT_);
  machine_push(m, token_new_int(arr_size(a) / 2));
}

static void has (Machine *m) {
  char *key = tk_pop_string(m);
  // Arr<char | Token>
  Arr *a = tk_pop_native(m, symbol_OBJECT_);

  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    if (str_eq(key, *tks)) {
      machine_push(m, token_new_int(1));
      return;
    }
    tks += 2;
  }
  machine_push(m, token_new_int(0));
}

static void get (Machine *m) {
  char *key = tk_pop_string(m);
  // Arr<char | Token>
  Arr *a = tk_pop_native(m, symbol_OBJECT_);

  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    if (str_eq(key, *tks)) {
      machine_push(m, *++tks);
      return;
    }
    tks += 2;
  }
  machine_fail(m, str_f("Key '%s' not found", key));
}

static void putboth (Machine *m, int isplus) {
  Token *tk = machine_pop(m);

  char *key = tk_string(m, machine_pop(m));
  // Arr<char | Token>
  Arr *a = isplus
    ? tk_peek_native(m, symbol_OBJECT_)
    : tk_pop_native(m, symbol_OBJECT_)
  ;

  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    if (str_eq(key, *tks)) {
      *++tks = tk;
      return;
    }
    tks += 2;
  }

  arr_push(a, key);
  arr_push(a, tk);
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

static void frommap (Machine *m) {
  // Arr<char | Token>
  Arr *r = arr_new();
  EACH(map_kvs(tk_pop_native(m, symbol_MAP_)), Kv, kv) {
    arr_push(r, kv_key(kv));
    arr_push(r, kv_value(kv));
  }_EACH
  machine_push(m, token_from_pointer(symbol_OBJECT_, r));
}

static void tomap (Machine *m) {
  // Arr<char | Token>
  Arr *a = tk_pop_native(m, symbol_OBJECT_);

  // Map<Token>
  Map *mp = map_new();
  void **tks = arr_start(a);
  void **end = arr_end(a);
  while (tks < end) {
    char *k = *tks++;
    map_put(mp, k, *tks++);
  }

  machine_push(m, token_from_pointer(symbol_MAP_, mp));
}

Pmodule *modobj_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // [] - OBJ
  add("size", size);
  add("has?", has); // [OBJ, STRING] - INT
  add("get", get); // [OBJ, STRING] - *
  add("put", put); // [OBJ - STRING - *] - []
  add("put+", putplus); // [OBJ - STRING - *] - OBJ
  add("up", up); // [OBJ - STRING - LIST] - []
  add("up+", upplus); // [OBJ - STRING - LIST] - OBJ
  add("fromMap", frommap); // MAP - OBJ
  add("toMap", tomap); // OBJ - MAP

  return r;
}



