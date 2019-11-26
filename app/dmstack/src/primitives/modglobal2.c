// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal2.h"
#include "fails.h"
#include "tk.h"

void modglobal2_get (Machine *m) {
  machine_push(m, tk_pop_native(m, symbol_REF_));
}

static void refsetboth (Machine *m, int isplus) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = isplus
    ? machine_peek(m)
    : machine_pop(m)
  ;
  tk_native(m, tk1, symbol_REF_);
  token_set_native_pointer(tk1, tk2);
}

void modglobal2_set (Machine *m) {
  refsetboth(m, 0);
}

void modglobal2_setplus (Machine *m) {
  refsetboth(m, 1);
}

static void refupboth (Machine *m, int isplus) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *tk1 = machine_peek(m);
  modglobal2_get(m);

  machine_process("", machine_pmachines(m), prg);

  Token *tk2 = machine_pop(m);
  machine_push(m, tk1);
  machine_push(m, tk2);
  refsetboth(m, isplus);
}

void modglobal2_up (Machine *m) {
  refupboth(m, 0);
}

void modglobal2_upplus (Machine *m) {
  refupboth(m, 1);
}
