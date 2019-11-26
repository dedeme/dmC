// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modwrap.h"
#include "tk.h"
#include "fails.h"

static Token *poplist (Machine *m) {
  Token *r = machine_pop_exc(m, token_LIST);
  return r;
}

static void none (Machine *m) {
  machine_push(m, token_from_pointer(symbol_OPTION_, NULL));
}

static void isnone (Machine *m) {
  Token *value = tk_pop_native(m, symbol_OPTION_);
  machine_push(m, token_new_int(!value));
}

static void some (Machine *m) {
  machine_push(m, token_from_pointer(symbol_OPTION_, machine_pop(m)));
}

static void issome (Machine *m) {
  Token *value = tk_pop_native(m, symbol_OPTION_);
  machine_push(m, token_new_int((Int)value));
}

static void option (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *prgfail = poplist(m);
  Token *prgok = poplist(m);
  Token *value = tk_pop_native(m, symbol_OPTION_);
  if (value) {
    machine_push(m, value);
    machine_process("", pmachines, prgok);
  } else {
    machine_process("", pmachines, prgfail);
  }
}

static void ref (Machine *m) {
  machine_push(m, token_from_pointer(symbol_REF_, machine_pop(m)));
}

static void left (Machine *m) {
  Token **values = ATOMIC(sizeof(Token *) + sizeof(Token *));
  values[0] = machine_pop(m);
  values[1] = NULL;
  machine_push(m, token_from_pointer(symbol_EITHER_, values));
}

static void isleft (Machine *m) {
  Token **values = tk_pop_native(m, symbol_EITHER_);
  machine_push(m, token_new_int((Int)*values));
}

static void right (Machine *m) {
  Token **values = ATOMIC(sizeof(Token *) + sizeof(Token *));
  values[0] = NULL;
  values[1] = machine_pop(m);
  machine_push(m, token_from_pointer(symbol_EITHER_, values));
}

static void isright (Machine *m) {
  Token **values = tk_pop_native(m, symbol_EITHER_);
  machine_push(m, token_new_int(!*values));
}

static void either (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *prgfail = poplist(m);
  Token *prgok = poplist(m);
  Token **values = tk_pop_native(m, symbol_EITHER_);
  if (*values) {
    machine_push(m, values[0]);
    machine_process("", pmachines, prgfail);
  } else {
    machine_push(m, values[1]);
    machine_process("", pmachines, prgok);
  }
}

static void tp (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(arr_new_from(machine_pop(m), tk, NULL)));
}

static void tp3 (Machine *m) {
  Token *tk3 = machine_pop(m);
  Token *tk2 = machine_pop(m);
  machine_push(m, token_new_list(
    arr_new_from(machine_pop(m), tk2, tk3, NULL)
  ));
}

Pmodule *modwrap_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("none", none); // [] - OPT ([] -> empty list)
  add("none?", isnone); // OPT - INT
  add("some", some); // * - OPT (* -> (*))
  add("some?", issome); // OPT - INT
  add("option", option); // <OPT, (OPT >> -> B?), (B?)> - B?
  add("ref", ref); // * - REF (* -> (*))
  add("left", left); // STRING -> EITHER (s -> (s, 0))
  add("left?", isleft); // EITHER - INT
  add("right", right); // * -> EITHER (* -> (*)) -some is ok-
  add("right?", isright); // EITHER - INT
  add("either", either); // <EITHER, (RIGHT->B?), (LEFT->B?)> - B?
  add("tp", tp); // [A, B] -> [(A, B)]
  add("tp3", tp3); // [A, B, C] -> [(A, B, C)]

  return r;
}




