// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modstr.h"
#include "dmc/Dec.h"
#include "fails.h"
#include "Machine.h"

static char *getstr (Machine *m) {
  return token_string(machine_pop_exc(m, token_STRING));
}

static void pushstr (Machine *m, char *s) {
  machine_push(m, token_new_string(0, s));
}

static void cmp (Machine *m) {
  char *s2 = getstr(m);
  char *s1 = getstr(m);
  machine_push(m, token_new_int(0, str_cmp_locale(s1, s2)));
}

static void ends (Machine *m) {
  char *sub = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_ends(s, sub)));
}

static void from_iso (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_from_iso(s));
}

static void from_unicode (Machine *m) {
  Bytes *bs = fails_read_pointer(m, symbol_BYTES_, machine_pop(m));
  int len = bytes_len(bs);
  if (len % 2) machine_fail(m, "Blob is not an Unicode string");
  unsigned *u = (unsigned *)bytes_bs(bs);
  char *r = opt_nget(str_from_unicode(u));
  if (r) pushstr(m, r);
  else machine_fail(m, "Blob is not an Unicode string");
}

static void greater (Machine *m) {
  char *s2 = getstr(m);
  char *s1 = getstr(m);
  machine_push(m, token_new_int(0, str_greater_locale(s1, s2)));
}

static void sindex (Machine *m) {
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_index(s, subs)));
}

static void index_from (Machine *m) {
  int ix = token_int(machine_pop_exc(m, token_INT));
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_index_from(s, subs, ix)));
}

static void last_index (Machine *m) {
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_last_index(s, subs)));
}

static void join (Machine *m) {
  char *sep = getstr(m);
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  Buf *bf = buf_new();
  int first = 1;
  EACH(a, Token, tk)
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep);
    }
    if (token_type(tk) != token_STRING) fails_type(m, token_STRING);
    buf_add(bf, token_string(tk));
  _EACH
  pushstr(m, buf_to_str(bf));
}

static void replace (Machine *m) {
  char *new = getstr(m);
  char *old = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_string(0, str_replace(s, old, new)));
}

static void next_rune (Machine *m) {
  char *s = getstr(m);
  char *rune;
  machine_push(m, token_new_string(0, str_next_rune(&rune, s)));
  machine_push(m, token_new_string(0, rune));
}

static void runes (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_runes(s)));
}

static void split (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  Token * fn(char *s) { return token_new_string(0, s); }
  machine_push(m, token_new_list(
    0, arr_map(str_split(s, sep), (FCOPY)fn)
  ));
}

static void split_trim (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  Token * fn(char *s) { return token_new_string(0, s); }
  machine_push(m, token_new_list(
    0, arr_map(str_split_trim(s, sep), (FCOPY)fn)
  ));
}

static void starts (Machine *m) {
  char *sub = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(0, str_starts(s, sub)));
}

static void trim (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_trim(s));
}

static void ltrim (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_ltrim(s));
}

static void rtrim (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_rtrim(s));
}

static void to_lower (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_to_lower(s));
}

static void to_unicode (Machine *m) {
  char *s = getstr(m);
  unsigned *u = opt_nget(str_to_unicode(s));
  if (u) {
    int size = 0;
    unsigned *p = u;
    while (*p++) ++size;
    int len = size * 4 + 4;
    Bytes *bs = bytes_bf_new(len);
    memcpy(bytes_bs(bs), u, len);
    machine_push(m, token_from_pointer(symbol_BYTES_, bs));
    return;
  }
  machine_fail(m, "String is no a valid UTF-8 one");
}

static void to_upper (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_to_upper(s));
}

static void subaux (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  Token *tk = machine_pop_exc(m, token_STRING);
  char *s = token_string(tk);
  bounds(strlen(s));
  machine_push(m, token_new_string(0, str_sub(s, begin, end)));
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

static void isdigits (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(0, dec_digits(s)));
}

static void isnumber (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(0, dec_number(s)));
}

static void riso (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_string(0, dec_regularize_iso(s)));
}

static void rus (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_string(0, dec_regularize_us(s)));
}

Pmodule *modstr_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("cmp", cmp); // in locale
  add("ends?", ends);
  add("fromIso", from_iso);
  add("fromUnicode", from_unicode);
  add("greater?", greater); // in locale
  add("index", sindex);
  add("indexFrom", index_from);
  add("lastIndex", last_index);
  add("join", join);
  add("replace", replace);
  add("nextRune", next_rune);
  add("runes", runes);
  add("split", split);
  add("splitTrim", split_trim);
  add("starts?", starts);
  add("trim", trim);
  add("ltrim", ltrim);
  add("rtrim", rtrim);
  add("toLower", to_lower);
  add("toUnicode", to_unicode);
  add("toUpper", to_upper);

  add("sub", sub);
  add("left", left);
  add("right", right);

  add("digits?", isdigits);
  add("number?", isnumber);
  add("regularizeIso", riso);
  add("regularizeUs", rus);

  return r;
}


