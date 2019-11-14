// Copyright 03-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modblob.h"
#include "fails.h"
#include "tk.h"

static void new (Machine *m) {
  Int sz = tk_pop_int(m);
  machine_push(m, token_from_pointer(symbol_BLOB_, bytes_bf_new(sz)));
}

static void make (Machine *m) {
  Int sz = tk_pop_int(m);
  Int value = tk_pop_int(m);
  Bytes *bs = bytes_bf_new(sz);
  memset(bytes_bs(bs), value, sz);
  machine_push(m, token_from_pointer(symbol_BLOB_, bs));
}

static void fill (Machine *m) {
  Int value = tk_pop_int(m);
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  memset(bytes_bs(bs), value, bytes_len(bs));
}

static void from (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int size = arr_size(a);
  Bytes *r = bytes_bf_new(size);
  unsigned char *p = bytes_bs(r);
  EACH(a, Token, tk) {
    if (token_type(tk) != token_INT)
      fails_type(m, token_INT);
    *p++ = token_int(tk);
  }_EACH;
  machine_push(m, token_from_pointer(symbol_BLOB_, r));
}

static void get (Machine *m) {
  Int ix = tk_pop_int(m);
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  if (ix < 0 || ix >= bytes_len(bs))
    fails_range(m, 0, bytes_len(bs) - 1, ix);

  machine_push(m, token_new_int(bytes_bs(bs)[ix]));
}

static void setboth (Machine *m, int isplus) {
  Int value = tk_pop_int(m);
  Int ix = tk_pop_int(m);
  Bytes *bs = isplus
    ? tk_peek_native(m, symbol_BLOB_)
    : tk_pop_native(m, symbol_BLOB_)
  ;
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
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  machine_push(m, token_new_int(bytes_len(bs)));
}

static int bseq (Bytes *b1, Bytes *b2) {
  int len = bytes_len(b1);
  return len == bytes_len(b2) && !memcmp(bytes_bs(b1), bytes_bs(b2), len);
}

static void equals (Machine *m) {
  Bytes *bs2 = tk_pop_native(m, symbol_BLOB_);
  Bytes *bs1 = tk_pop_native(m, symbol_BLOB_);
  machine_push(m, token_new_int(bseq(bs1, bs2)));
}

static void notequals (Machine *m) {
  Bytes *bs2 = tk_pop_native(m, symbol_BLOB_);
  Bytes *bs1 = tk_pop_native(m, symbol_BLOB_);
  machine_push(m, token_new_int(!bseq(bs1, bs2)));
}

static void to (Machine *m) {
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  int size = bytes_len(bs);
  unsigned char *pbs = bytes_bs(bs);
  Token **tks = GC_MALLOC(size * sizeof(Token *));
  Token **ptk = tks;
  REPEAT(size) {
    *ptk++ = token_new_int(*pbs++);
  }_REPEAT
  machine_push(m, token_new_list(arr_new_c(size, (void **)tks)));
}

static void subaux (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  Bytes *bs = tk_pop_native(m, symbol_BLOB_);

  bounds(bytes_len(bs));
  int df = end - begin;

  if (df < 0) df = 0;
  Bytes *r = bytes_bf_new(df);
  memcpy(bytes_bs(r), bytes_bs(bs) + begin, df);
  machine_push(m, token_from_pointer(symbol_BLOB_, r));
}

static void sub (Machine *m) {
  Int end = tk_pop_int(m);
  Int begin = tk_pop_int(m);
  subaux(m, begin, end, 0);
}

static void left (Machine *m) {
  Int cut = tk_pop_int(m);
  subaux(m, 0, cut, 0);
}

static void right (Machine *m) {
  Int cut = tk_pop_int(m);
  subaux(m, cut, 0, 1);
}

static void copy (Machine *m) {
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  machine_push(m, token_from_pointer(
    symbol_BLOB_, bytes_from_bytes(bytes_bs(bs), bytes_len(bs))
  ));
}

static void plus (Machine *m) {
  Bytes *bs2 = tk_pop_native(m, symbol_BLOB_);
  Bytes *bs1 = tk_pop_native(m, symbol_BLOB_);

  int len1 = bytes_len(bs1);
  int len2 = bytes_len(bs2);
  Bytes *r = bytes_bf_new(len1 + len2);
  memcpy(bytes_bs(r), bytes_bs(bs1), len1);
  memcpy(bytes_bs(r) + len1, bytes_bs(bs2), len2);

  machine_push(m, token_from_pointer(symbol_BLOB_, r));
}

static void plus2 (Machine *m) {
  Machine *m2 = machine_isolate_process(
    "", machine_pmachines(m), machine_pop_exc(m, token_LIST)
  );
  // Arr<Token>
  Arr *a = machine_stack(m2);
  if (!arr_size(a)) fails_list_size(m, a, 1);

  machine_push(m, *(arr_start(a)));
  for (int i = 1; i < arr_size(a); ++i) {
    machine_push(m, arr_get(a, i));
    plus(m);
  }
}

static void fromjs (Machine *m) {
  char *js = tk_pop_string(m);
  machine_push(m, token_from_pointer(symbol_BLOB_, bytes_from_js((Js *)js)));
}

static void tojs (Machine *m) {
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
  machine_push(m, token_new_string((char *)bytes_to_js(bs)));
}

Pmodule *modblob_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // INT - BLOB
  add("make", make); // <INT, INT> - <BLOB>  (value, size) - (blob)
  add("fill", fill); // <BLOB, INT> - <BLOB>
  add("from", from); // LIST - BLOB
  add("get", get); // [BLOB, INT] - INT
  add("set", set); // [BLOB, INT, INT] - []
  add("set+", setplus); // [BLOB, INT, INT] - [BLOB]
  add("up", up); // [BLOB, INT, LIST] - []
  add("up+", upplus); // [BLOB, INT, LIST] - [BLOB]

  add("size", size); // BLOB - INT
  add("==", equals); // [BLOB, BLOB] - INT
  add("!=", notequals); // [BLOB, BLOB] - INT

  add("to", to); // BLOB - LIST
  add("sub", sub); // [BLOB, INT, INT] - [BLOB]
  add("left", left); // [BLOB, INT] - [BLOB]
  add("right", right); // [BLOB, INT] - [BLOB]
  add("copy", copy); // [BLOB] - [BLOB]
  add("+", plus); // [BLOB, BLOB] - [BLOB]
  add("++", plus2); // [(BLOB, BLOB, ...)] - [BLOB]

  add("fromJs", fromjs); // STRING - BLOB
  add("toJs", tojs); // BLOB - STRING

  return r;
}

