// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modstr.h"
#include "fails.h"
#include "Machine.h"

static char *getstr (Machine *m) {
  return token_string(machine_pop_exc(m, token_STRING));
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
  Bytes *bs = token_blob(machine_pop_exc(m, token_BLOB));
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
  int ix = token_int(machine_pop_exc(m, token_INT));
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

static void locale (Machine *m) {
  char *l = getstr(m);
  sys_locale(l);
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

static void runes (Machine *m) {
  char *s = getstr(m);
  machine_push(m, token_new_int(str_runes(s)));
}

static void split (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_list(
    arr_map(str_split(s, sep), (FCOPY)token_new_string)
  ));
}

static void split_trim (Machine *m) {
  char *sep = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_list(
    arr_map(str_split_trim(s, sep), (FCOPY)token_new_string)
  ));
}

static void starts (Machine *m) {
  char *sub = getstr(m);
  char *s = getstr(m);
  machine_push(m, token_new_int(str_starts(s, sub)));
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
    Token *tk = token_new_blob(len);
    memcpy(bytes_bs(token_blob(tk)), u, len);
    machine_push(m, tk);
    return;
  }
  machine_fail(m, "String is no a valid UTF-8 one");
}

static void to_upper (Machine *m) {
  char *s = getstr(m);
  pushstr(m, str_to_upper(s));
}

// Resturns Map<primitives_Fn>
Map *modstr_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "str.cmp", cmp); // in locale
  map_put(r, "str.ends", ends);
  map_put(r, "str.fromIso", from_iso);
  map_put(r, "str.fromUnicode", from_unicode);
  map_put(r, "str.greater", greater); // in locale
  map_put(r, "str.index", sindex);
  map_put(r, "str.indexFrom", index_from);
  map_put(r, "str.lastIndex", last_index);
  map_put(r, "str.join", join);
  map_put(r, "str.locale", locale);
  map_put(r, "str.replace", replace);
  map_put(r, "str.nextRune", next_rune);
  map_put(r, "str.runes", runes);
  map_put(r, "str.split", split);
  map_put(r, "str.splitTrim", split_trim);
  map_put(r, "str.starts", starts);
  map_put(r, "str.trim", trim);
  map_put(r, "str.ltrim", ltrim);
  map_put(r, "str.rtrim", rtrim);
  map_put(r, "str.toLower", to_lower);
  map_put(r, "str.toUnicode", to_unicode);
  map_put(r, "str.toUpper", to_upper);

  return r;
}


