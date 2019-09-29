// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modwrap.h"
#include "Machine.h"
#include "fails.h"

static void none (Machine *m) {
  machine_push(m, token_new_list(0, arr_new()));
}

static void isnone (Machine *m) {
  machine_push(m, token_new_int(
    0, !arr_size(token_list(machine_pop_exc(m, token_LIST)))
  ));
}

static void some (Machine *m) {
  machine_push(m, token_new_list(0, arr_new_from(machine_pop(m), NULL)));
}

static void issome (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz > 1) fails_size_list(m, a, 1);
  machine_push(m, token_new_int(0, sz));
}

static void option (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *cond = machine_pop_exc(m, token_LIST);
  Token *prg2 = machine_pop_exc(m, token_LIST);
  Token *prg1 = machine_pop_exc(m, token_LIST);
  machine_process("", pmachines, cond);
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (!sz) {
    machine_process("", pmachines, prg1);
  } else if (sz == 1) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prg2);
  } else {
    fails_size_list(m, a, 1);
  }
}

static void left (Machine *m) {
  machine_push(m, token_new_list(
    0, arr_new_from(machine_pop_exc(m, token_STRING), token_new_int(0, 0), NULL)
  ));
}

static void isleft (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz != 1 && sz != 2) fails_size_list(m, a, 2);
  machine_push(m, token_new_int(0, sz == 2));
}

static void isright (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz != 1 && sz != 2) fails_size_list(m, a, 2);
  machine_push(m, token_new_int(0, sz == 1));
}

static void either (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *cond = machine_pop_exc(m, token_LIST);
  Token *prg2 = machine_pop_exc(m, token_LIST);
  Token *prg1 = machine_pop_exc(m, token_LIST);
  machine_process("", pmachines, cond);
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz == 1) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prg2);
  } else if (sz == 2) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prg1);
  } else {
    fails_size_list(m, a, 2);
  }
}

static void tp (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(0, arr_new_from(machine_pop(m), tk, NULL)));
}

static void tp3 (Machine *m) {
  Token *tk3 = machine_pop(m);
  Token *tk2 = machine_pop(m);
  machine_push(m, token_new_list(
    0, arr_new_from(machine_pop(m), tk2, tk3, NULL)
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
  add("option", option); // [(->B?), (OPT->B?), (->OPT)] - B?
  add("left", left); // STRING -> EITHER (s -> (s, 0))
  add("left?", isleft); // EITHER - INT
  add("right", some); // * -> EITHER (* -> (*)) -some is ok-
  add("right?", isright); // EITHER - INT
  add("either", either);
    // [(LEFT->B?), (RIGHT->B?), (->EITHER)] - B?
  add("tp", tp); // [A, B] -> [(A, B)]
  add("tp3", tp3); // [A, B, C] -> [(A, B, C)]

  return r;
}




