// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modstr.h"
#include "dmc/Dec.h"
#include "fails.h"
#include "tk.h"

static char *getstr (Machine *m) {
  return tk_pop_string(m);
}

static void pushstr (Machine *m, char *s) {
  machine_push(m, token_new_string(s));
}

static void cmp (Machine *m) {
  char *s2 = getstr(m);
  char *s1 = getstr(m);
  machine_push(m, token_new_int(str_cmp_locale(s1, s2)));
}

static void ends (Machine *m) {
  char *sub = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_ends(s, sub)));
}

static void from_iso (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_from_iso(s));
}

static void from_unicode (Machine *m) {
  Bytes *bs = tk_pop_native(m, symbol_BLOB_);
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
  machine_push(m, token_new_int(str_greater_locale(s1, s2)));
}

static void sindex (Machine *m) {
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_index(s, subs)));
}

static void index_from (Machine *m) {
  Int ix = tk_pop_int(m);
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_index_from(s, subs, ix)));
}

static void last_index (Machine *m) {
  char *subs = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_last_index(s, subs)));
}

static void join (Machine *m) {
  char *sep = getstr(m);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  Buf *bf = buf_new();
  int first = 1;
  EACH(a, Token, tk)
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep);
    }
    buf_add(bf, tk_string(m, tk));
  _EACH
  pushstr(m, buf_to_str(bf));
}

static void replace (Machine *m) {
  char *new = getstr(m);
  char *old = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_string(str_replace(s, old, new)));
}

static void next_rune (Machine *m) {
  char *s = getstr(m);
  char *rune;
  machine_push(m, token_new_string(str_next_rune(&rune, s)));
  machine_push(m, token_new_string(rune));
}

static void split (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  Token * fn(char *s) { return token_new_string(s); }
  machine_push(m, token_new_list(
    arr_map(str_split(s, sep), (FCOPY)fn)
  ));
}

static void split_trim (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  Token * fn(char *s) { return token_new_string(s); }
  machine_push(m, token_new_list(
    arr_map(str_split_trim(s, sep), (FCOPY)fn)
  ));
}

static void starts (Machine *m) {
  char *sub = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_starts(s, sub)));
}

static void reverse (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_reverse(s));
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
    machine_push(m, token_from_pointer(symbol_BLOB_, bs));
    return;
  }
  machine_fail(m, "String is no a valid UTF-8 one");
}

static void to_upper (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_to_upper(s));
}

static void get (Machine *m) {
  Int ix = tk_pop_int(m);
  char *s = tk_pop_string(m);
  fails_check_range(m, 0, strlen(s) - 1, ix);
  pushstr(m, str_c(s[ix]));
}

static void len (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(strlen(s)));
}

static void getrune (Machine *m) {
  Int ix = tk_pop_int(m);
  char *s = tk_pop_string(m);
  if (ix < 0) fails_range(m, 0, str_runes(s) - 1, ix);
  int r = 0;
  Int c = 0;
  char *rune;
  char *p = s;
  for (;;) {
    p = str_next_rune(&rune, p);
    if (!*rune) {
      r = 1;
      break;
    }
    if (c == ix) break;
    ++c;
  }
  if (r) {
    int len = str_runes(s);
    if (len == -1) machine_fail(m, str_f("Wrong UTF-8 string '%s'", s));
    else fails_range(m, 0, len - 1, ix);
  }
  pushstr(m, rune);
}

static void runes (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(str_runes(s)));
}

static void subaux (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  char *s = getstr(m);
  bounds(strlen(s));
  machine_push(m, token_new_string(str_sub(s, begin, end)));
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

static void code (Machine *m) {
  char *ch = tk_pop_string(m);
  if (!*ch) machine_fail(m, "String is empty");
  machine_push(m, token_new_int(*ch));
}

static void fromcode (Machine *m) {
  Int code = tk_pop_int(m);
  machine_push(m, token_new_string(str_c(code)));
}

static void runecode (Machine *m) {
  char *ch = tk_pop_string(m);
  if (!*ch) machine_fail(m, "String is empty");
  char *rune;
  str_next_rune(&rune, ch);
  if (!*rune) machine_fail(m, "Wrong UTF8 code");
  unsigned char *rn = (unsigned char *) rune;
  Int c;
  switch (strlen(rune)) {
    case 1: c = *rn; break;
    case 2: c = *rn * 256 + rn[1]; break;
    case 3: c = *rn * 65536 + rn[1] * 256 + rn[2]; break;
    default: c = *rn * 16777216 + rn[1] * 65536 + rn[2] * 256 + rn[3];
  }
  machine_push(m, token_new_int(c));
}

static void fromrunecode (Machine *m) {
  Int code = tk_pop_int(m);
  char *r;
  if (code < 256) {
    r = ATOMIC(2);
    r[1] = 0;
    *r = code;
  } else if (code < 65536) {
    r = ATOMIC(3);
    r[2] = 0;
    r[1] = code % 256;
    *r = code / 256;
  } else if (code < 16777216) {
    r = ATOMIC(4);
    r[3] = 0;
    r[2] = code % 256;
    code = code / 256;
    r[1] = code % 256;
    *r = code / 256;
  } else {
    r = ATOMIC(5);
    r[4] = 0;
    r[3] = code % 256;
    code = code / 256;
    r[2] = code % 256;
    code = code / 256;
    r[1] = code % 256;
    *r = code / 256;
  }
  machine_push(m, token_new_string(r));
}

static void isdigits (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(dec_digits(s)));
}

static void isnumber (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(dec_number(s)));
}

static void riso (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_string(dec_regularize_iso(s)));
}

static void rus (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_string(dec_regularize_us(s)));
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
  add("split", split);
  add("splitTrim", split_trim);
  add("starts?", starts);
  add("reverse", reverse);
  add("trim", trim);
  add("ltrim", ltrim);
  add("rtrim", rtrim);
  add("toLower", to_lower);
  add("toUnicode", to_unicode);
  add("toUpper", to_upper);

  add("get", get);
  add("len", len);
  add("getRune", getrune);
  add("runes", runes);
  add("sub", sub);
  add("left", left);
  add("right", right);
  add("code", code);
  add("fromCode", fromcode);
  add("runeCode", runecode);
  add("fromRuneCode", fromrunecode);

  add("digits?", isdigits);
  add("number?", isnumber);
  add("regularizeIso", riso);
  add("regularizeUs", rus);

  return r;
}


