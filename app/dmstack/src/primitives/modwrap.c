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
  machine_push(m, token_new_list(arr_new()));
}

static void isnone (Machine *m) {
  machine_push(m, token_new_int(
    !arr_size(tk_pop_list(m))
  ));
}

static void some (Machine *m) {
  machine_push(m, token_new_list(arr_new_from(machine_pop(m), NULL)));
}

static void issome (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (sz > 1) fails_list_size(m, a, 1);
  machine_push(m, token_new_int(sz));
}

static void option (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *prgfail = poplist(m);
  Token *prgok = poplist(m);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (!sz) {
    machine_process("", pmachines, prgfail);
  } else if (sz == 1) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prgok);
  } else {
    fails_list_size(m, a, 1);
  }
}

static void left (Machine *m) {
  Token *s = machine_pop_exc(m, token_STRING);
  machine_push(m, token_new_list(
    arr_new_from(s, token_new_int(0), NULL)
  ));
}

static void isleft (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (sz != 1 && sz != 2) fails_list_size(m, a, 2);
  machine_push(m, token_new_int(sz == 2));
}

static void isright (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (sz != 1 && sz != 2) fails_list_size(m, a, 2);
  machine_push(m, token_new_int(sz == 1));
}

static void either (Machine *m) {
  // List<Machine>
  List *pmachines = machine_pmachines(m);
  Token *prgfail = poplist(m);
  Token *prgok = poplist(m);
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (sz == 1) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prgok);
  } else if (sz == 2) {
    machine_push(m, *arr_start(a));
    machine_process("", pmachines, prgfail);
  } else {
    fails_list_size(m, a, 2);
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
  add("ref", some); // * - REF (* -> (*))
  add("left", left); // STRING -> EITHER (s -> (s, 0))
  add("left?", isleft); // EITHER - INT
  add("right", some); // * -> EITHER (* -> (*)) -some is ok-
  add("right?", isright); // EITHER - INT
  add("either", either); // <EITHER, (RIGHT->B?), (LEFT->B?)> - B?
  add("tp", tp); // [A, B] -> [(A, B)]
  add("tp3", tp3); // [A, B, C] -> [(A, B, C)]

  return r;
}




