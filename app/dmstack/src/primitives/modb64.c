// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modb64.h"
#include "dmc/b64.h"
#include "tk.h"
#include "fails.h"

static void decode (Machine *m) {
  machine_push(m, token_new_string(b64_decode(tk_pop_string(m))));
}

static void decode_bytes (Machine *m) {
  machine_push(m, token_from_pointer(symbol_BLOB_, b64_decode_bytes(
    tk_pop_string(m)
  )));
}

static void encode (Machine *m) {
  machine_push(m, token_new_string(b64_encode(
    tk_pop_string(m)
  )));
}

static void encode_bytes (Machine *m) {
  machine_push(m, token_new_string(b64_encode_bytes(
    tk_pop_native(m, symbol_BLOB_)
  )));
}


Pmodule *modb64_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("decode", decode); // STRING - STRING
  add("decodeBytes", decode_bytes); // STRING - BLOB
  add("encode", encode); // STRING - STRING
  add("encodeBytes", encode_bytes); // BLOB - STRING

  return r;
}


