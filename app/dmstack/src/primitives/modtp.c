// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modtp.h"
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

static void new2 (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(0, arr_new_from(machine_pop(m), tk, NULL)));
}

static void new3 (Machine *m) {
  Token *tk3 = machine_pop(m);
  Token *tk2 = machine_pop(m);
  machine_push(m, token_new_list(
    0, arr_new_from(machine_pop(m), tk2, tk3, NULL)
  ));
}

// Resturns Map<primitives_Fn>
Map *modtp_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "none", none); // [] - OPT ([] -> empty list)
  map_put(r, "none?", isnone); // OPT - INT
  map_put(r, "some", some); // * - OPT (* -> (*))
  map_put(r, "some?", issome); // OPT - INT
  map_put(r, "option", option); // [(->B?), (OPT->B?), (->OPT)] - B?
  map_put(r, "left", left); // STRING -> EITHER (s -> (s, 0))
  map_put(r, "left?", isleft); // EITHER - INT
  map_put(r, "right", some); // * -> EITHER (* -> (*)) -some is ok-
  map_put(r, "right?", isright); // EITHER - INT
  map_put(r, "either", either);
    // [(LEFT->B?), (RIGHT->B?), (->EITHER)] - B?
  map_put(r, "new2", new2); // [A, B] -> [(A, B)]
  map_put(r, "new3", new3); // [A, B, C] -> [(A, B, C)]

  return r;
}



