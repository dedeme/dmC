// Copyright 03-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modblob.h"
#include "fails.h"
#include "Machine.h"

static void fromStr (Machine *m) {
  machine_fail(m, "Witout implementation");
}

static void from (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int size = arr_size(a);
  Token *r = token_new_blob(size);
  unsigned char *p = bytes_bs(token_blob(r));
  EACH(a, Token, tk) {
    if (token_type(tk) != token_INT)
      fails_type(m, token_INT);
    *p++ = token_int(tk);
  }_EACH;
  machine_push(m, r);
}

static void to (Machine *m) {
  Bytes *bs = token_blob(machine_pop_exc(m, token_BLOB));
  int size = bytes_len(bs);
  unsigned char *pbs = bytes_bs(bs);
  Token **tks = GC_MALLOC(size * sizeof(Token *));
  Token **ptk = tks;
  REPEAT(size) {
    *ptk++ = token_new_int(*pbs++);
  }_REPEAT
  machine_push(m, token_new_list(arr_new_c(size, (void **)tks)));
}

// Resturns Map<primitives_Fn>
Map *modblob_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "blob.fromStr", fromStr);
  map_put(r, "blob.from", from);
  map_put(r, "blob.to", to);

  return r;
}

