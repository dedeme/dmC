// Copyright 03-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modblob.h"
#include "fails.h"
#include "Machine.h"

static void new (Machine *m) {
  Int sz = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_from_pointer(symbol_BYTES_, bytes_bf_new(sz)));
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
  machine_push(m, token_from_pointer(symbol_BYTES_, r));
}

static void get (Machine *m) {
  Int ix = token_int(machine_pop_exc(m, token_INT));
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  if (ix < 0 || ix >= bytes_len(bs))
    fails_range(m, 0, bytes_len(bs) - 1, ix);

  machine_push(m, token_new_int(0, bytes_bs(bs)[ix]));
}

static void setboth (Machine *m, int isplus) {
  Int value = token_int(machine_pop_exc(m, token_INT));
  Int ix = token_int(machine_pop_exc(m, token_INT));
  Bytes *bs = fails_read_pointer(
    m, symbol_BYTES_,
    isplus ? machine_peek(m) : machine_pop(m)
  );
  if (ix < 0 || ix >= bytes_len(bs))
    fails_range(m, 0, bytes_len(bs) - 1, ix);

  bytes_bs(bs)[ix] = value;
}

static void set (Machine *m) {
  setboth(m, 0);
}

static void setplus (Machine *m) {
  setboth(m, 1);
}

static void upboth (Machine *m, int isplus) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_peek(m);
  machine_push(m, tk2);
  get(m);

  machine_process("", machine_pmachines(m), prg);

  Token *tk3 = machine_pop(m);
  machine_push(m, tk1);
  machine_push(m, tk2);
  machine_push(m, tk3);
  setboth(m, isplus);
}

static void up (Machine *m) {
  upboth(m, 0);
}

static void upplus (Machine *m) {
  upboth(m, 1);
}

static void size (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  machine_push(m, token_new_int(0, bytes_len(bs)));
}

static int bseq (Bytes *b1, Bytes *b2) {
  int len = bytes_len(b1);
  return len == bytes_len(b2) && !memcmp(bytes_bs(b1), bytes_bs(b2), len);
}

static void equals (Machine *m) {
  Bytes *bs2 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  Bytes *bs1 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  machine_push(m, token_new_int(0, bseq(bs1, bs2)));
}

static void notequals (Machine *m) {
  Bytes *bs2 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  Bytes *bs1 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  machine_push(m, token_new_int(0, !bseq(bs1, bs2)));
}

static void to (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
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

  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));

  bounds(bytes_len(bs));
  int df = end - begin;

  if (df < 0) df = 0;
  Bytes *r = bytes_bf_new(df);
  memcpy(bytes_bs(r), bytes_bs(bs) + begin, df);
  machine_push(m, token_from_pointer(symbol_BYTES_, r));
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

static void copy (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  machine_push(m, token_from_pointer(
    symbol_BYTES_, bytes_from_bytes(bytes_bs(bs), bytes_len(bs))
  ));
}

static void plus (Machine *m) {
  Bytes *bs2 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  Bytes *bs1 = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));

  int len1 = bytes_len(bs1);
  int len2 = bytes_len(bs2);
  Bytes *r = bytes_bf_new(len1 + len2);
  memcpy(bytes_bs(r), bytes_bs(bs1), len1);
  memcpy(bytes_bs(r) + len1, bytes_bs(bs2), len2);

  machine_push(m, token_from_pointer(symbol_BYTES_, r));
}

static void fromjs (Machine *m) {
  char *js = token_string(machine_pop_exc(m, token_STRING));
  machine_push(m, token_from_pointer(symbol_BYTES_, bytes_from_js((Js *)js)));
}

static void tojs (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  machine_push(m, token_new_string(0, (char *)bytes_to_js(bs)));
}

Pmodule *modblob_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // INT - BLOB
  add("from", from); // LIST - BLOB
  add("get", get); // [BLOB, INT] - INT
  add("set", set); // [BLOB, INT, INT] - []
  add("set+", setplus); // [BLOB, INT, INT] - [BLOB]
  add("up", up); // [BLOB, INT, INT] - []
  add("up+", upplus); // [BLOB, INT, INT] - []

  add("size", size); // BLOB - INT
  add("==", equals); // [BLOB, BLOB] - INT
  add("!=", notequals); // [BLOB, BLOB] - INT

  add("to", to); // BLOB - LIST
  add("sub", sub); // [BLOB, INT, INT] - [BLOB]
  add("left", left); // [BLOB, INT] - [BLOB]
  add("right", right); // [BLOB, INT] - [BLOB]
  add("copy", copy); // [BLOB] - [BLOB]
  add("+", plus); // [BLOB, BLOB] - [BLOB]

  add("fromJs", fromjs); // STRING - BLOB
  add("toJs", tojs); // BLOB - STRING

  return r;
}

