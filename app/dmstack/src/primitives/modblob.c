// Copyright 03-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modblob.h"
#include "fails.h"
#include "Machine.h"

static void new (Machine *m) {
  machine_push(m, token_new_blob(
    0, bytes_bf_new(token_int(machine_pop_exc(m, token_INT)))
  ));
}

static void from (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int size = arr_size(a);
  Bytes *r = bytes_bf_new(size);
  unsigned char *p = bytes_bs(r);
  EACH(a, Token, tk) {
    if (token_type(tk) != token_INT)
      fails_type(m, token_INT);
    *p++ = token_int(tk);
  }_EACH;
  machine_push(m, token_new_blob(0, r));
}

static void to (Machine *m) {
  Bytes *bs = token_blob(machine_pop_exc(m, token_BLOB));
  int size = bytes_len(bs);
  unsigned char *pbs = bytes_bs(bs);
  Token **tks = GC_MALLOC(size * sizeof(Token *));
  Token **ptk = tks;
  REPEAT(size) {
    *ptk++ = token_new_int(0, *pbs++);
  }_REPEAT
  machine_push(m, token_new_list(0, arr_new_c(size, (void **)tks)));
}

static void subaux (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  Token *tk = machine_pop_exc(m, token_BLOB);
  Bytes *bs = token_blob(tk);

  bounds(bytes_len(bs));
  int df = end - begin;

  if (df < 0) df = 0;
  Bytes *r = bytes_bf_new(df);
  memcpy(bytes_bs(r), bytes_bs(bs) + begin, df);
  machine_push(m, token_new_blob(0, r));
}

static void sub (Machine *m) {
  Token *tk3 = machine_pop_opt(m, token_INT);
  int end = token_int(tk3);
  Token *tk2 = machine_pop_opt(m, token_INT);
  int begin = token_int(tk2);
  subaux(m, begin, end, 0);
}

static void left (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  subaux(m, 0, cut, 0);
}

static void right (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  subaux(m, cut, 0, 1);
}

// Resturns Map<primitives_Fn>
Map *modblob_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new); // INT - BLOB
  map_put(r, "from", from); // LIST - BLOB
  map_put(r, "to", to); // BLOB - LIST
  map_put(r, "sub", sub);
  map_put(r, "left", left);
  map_put(r, "right", right);

  return r;
}

