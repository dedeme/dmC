// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modcryp.h"
#include "dmc/cryp.h"
#include "tk.h"

static void genk (Machine *m) {
  machine_push(m, token_new_string(
    0, cryp_genk(tk_pop_int(m))
  ));
}

static void key (Machine *m) {
  Int len = tk_pop_int(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_key(tx, len)));
}

static void cryp (Machine *m) {
  char *k = tk_pop_string(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_cryp(tx, k)));
}

static void decryp (Machine *m) {
  char *k = tk_pop_string(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_decryp(tx, k)));
}

static void autocryp (Machine *m) {
  Int len = tk_pop_int(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_auto_cryp(tx, len)));
}

static void autodecryp (Machine *m) {
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_auto_decryp(tx)));
}

static void encode (Machine *m) {
  char *k = tk_pop_string(m);
  Int len = tk_pop_int(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_encode(tx, len, k)));
}

static void decode (Machine *m) {
  char *k = tk_pop_string(m);
  char *tx = tk_pop_string(m);
  machine_push(m, token_new_string(0, cryp_decode(tx, k)));
}

Pmodule *modcryp_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("genk", genk); // INT - STRING
  add("key", key); // [STRING, INT] - STRING
  add("cryp", cryp); // [STRING, STRING] - STRING ([tx, key] - tx)
  add("decryp", decryp); // [STRING, STRING] - STRING ([tx, key] - tx)
  add("autoCryp", autocryp); // [STRING, INT] - STRING
  add("autoDecryp", autodecryp); // STRING - STRING
  add("encode", encode); // [STRING, INT, STRING] - STRING
                                // ([tx, len, key] - tx)
  add("decode", decode); // [STRING, STRING] - STRING ([tx, key] - tx)

  return r;
}


