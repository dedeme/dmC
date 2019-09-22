// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modcryp.h"
#include "dmc/cryp.h"
#include "Machine.h"

static void genk (Machine *m) {
  machine_push(m, token_new_string(
    0, cryp_genk(token_int(machine_pop_exc(m, token_INT)))
  ));
}

static void key (Machine *m) {
  int len = token_int(machine_pop_exc(m, token_INT));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_key(tx, len)));
}

static void cryp (Machine *m) {
  char *k = token_string(machine_pop_exc(m, token_STRING));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_cryp(tx, k)));
}

static void decryp (Machine *m) {
  char *k = token_string(machine_pop_exc(m, token_STRING));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_decryp(tx, k)));
}

static void autocryp (Machine *m) {
  int len = token_int(machine_pop_exc(m, token_INT));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_auto_cryp(tx, len)));
}

static void autodecryp (Machine *m) {
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_auto_decryp(tx)));
}

static void encode (Machine *m) {
  char *k = token_string(machine_pop_exc(m, token_STRING));
  int len = token_int(machine_pop_exc(m, token_INT));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_encode(tx, len, k)));
}

static void decode (Machine *m) {
  char *k = token_string(machine_pop_exc(m, token_STRING));
  char *tx = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_new_string(0, cryp_decode(tx, k)));
}

// Resturns Map<primitives_Fn>
Map *modcryp_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "genk", genk); // INT - STRING
  map_put(r, "key", key); // [STRING, INT] - STRING
  map_put(r, "cryp", cryp); // [STRING, STRING] - STRING ([tx, key] - tx)
  map_put(r, "decryp", decryp); // [STRING, STRING] - STRING ([tx, key] - tx)
  map_put(r, "autoCryp", autocryp); // [STRING, INT] - STRING
  map_put(r, "autoDecryp", autodecryp); // STRING - STRING
  map_put(r, "encode", encode); // [STRING, INT, STRING] - STRING
                                // ([tx, len, key] - tx)
  map_put(r, "decode", decode); // [STRING, STRING] - STRING ([tx, key] - tx)

  return r;
}


