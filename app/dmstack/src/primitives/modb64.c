// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modb64.h"
#include "dmc/b64.h"
#include "Machine.h"

static void decode (Machine *m) {
  machine_push(m, token_new_string(0, b64_decode(
    token_string(machine_pop_exc(m, token_STRING))
  )));
}

static void decode_bytes (Machine *m) {
  machine_push(m, token_new_blob(0, b64_decode_bytes(
    token_string(machine_pop_exc(m, token_STRING))
  )));
}

static void encode (Machine *m) {
  machine_push(m, token_new_string(0, b64_encode(
    token_string(machine_pop_exc(m, token_STRING))
  )));
}

static void encode_bytes (Machine *m) {
  machine_push(m, token_new_string(0, b64_encode_bytes(
    token_blob(machine_pop_exc(m, token_BLOB))
  )));
}

// Resturns Map<primitives_Fn>
Map *modb64_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "decode", decode); // STRING - STRING
  map_put(r, "decodeBytes", decode_bytes); // STRING - BLOB
  map_put(r, "encode", encode); // STRING - STRING
  map_put(r, "encodeBytes", encode_bytes); // BLOB - STRING

  return r;
}


