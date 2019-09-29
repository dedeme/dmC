// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modobj.h"
#include "fails.h"
#include "Machine.h"

static void new (Machine *m) {
  machine_push(m, token_new_list(0, arr_new()));
}

static void has (Machine *m) {
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  for (int i = 0; i < arr_size(a); i +=2)
    if (str_eq(key, token_string(arr_get(a, i)))) {
      machine_push(m, token_new_int(0, 1));
      return;
    }
  machine_push(m, token_new_int(0, 0));
}

static void get (Machine *m) {
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz & 1) fails_size_list(m, a, sz + 1);
  for (int i = 0; i < sz; i +=2)
    if (str_eq(key, token_string(arr_get(a, i)))) {
      machine_push(m, arr_get(a, i + 1));
      return;
    }
  machine_fail(m, str_f("Key '%s' not found", key));
}

static void putboth (Machine *m, int isplus) {
  Token *tk = machine_pop(m);
  Token *keytk = machine_pop_exc(m, token_STRING);
  char *key = token_string(keytk);
  // Arr<Token>
  Arr *a = isplus
    ? token_list(machine_peek_exc(m, token_LIST))
    : token_list(machine_pop_exc(m, token_LIST))
  ;
  int sz = arr_size(a);
  if (sz & 1) fails_size_list(m, a, sz + 1);
  for (int i = 0; i < sz; i +=2)
    if (str_eq(key, token_string(arr_get(a, i)))) {
      arr_set(a, i + 1, tk);
      return;
    }
  arr_push(a, keytk);
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
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
    // Arr<Token>
    Arr *a = token_list(tk);
    if (arr_size(a) != 2) fails_size_list(m, a, 2);
    Token *tk1 = *arr_start(a);
    if (token_type(tk1) != token_STRING) fails_type_in(m, token_STRING, tk1);
    arr_push(r, tk1);
    arr_push(r, arr_get(a, 1));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void tomap (Machine *m) {
  // Arr<Token>
  Arr *r = arr_new();
  Token *tk1 = NULL;
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    if (tk1) {
      arr_push(r, token_new_list(0, arr_new_from(tk1, tk, NULL)));
      tk1 = NULL;
    } else {
      if (token_type(tk) != token_STRING) fails_type_in(m, token_STRING, tk);
      tk1 = tk;
    }
  }_EACH
  if (tk1) {
    Arr *a = token_list(machine_pop_exc(m, token_LIST));
    fails_size_list(m, a, arr_size(a) + 1);
  }
  machine_push(m, token_new_list(0, r));
}

Pmodule *modobj_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // [] - OBJ
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



