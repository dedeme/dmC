// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_sys.h"
#include "kut/buf.h"
#include "exp.h"
#include "runner/fail.h"

// \s, s -> i
static Exp *cmp (Arr *exps) {
  CHECK_PARS ("str.cmp", 2, exps);
  return exp_int(str_cmp_locale(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1))
  ));
}

// \s, s -> b
static Exp *ends (Arr *exps) {
  CHECK_PARS ("str.ends", 2, exps);
  return exp_bool(str_ends(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1))
  ));
}

// \s, a -> s
static Exp *fmt (Arr *exps) {
  CHECK_PARS ("str.fmt", 2, exps);
  char *s = exp_rget_string(arr_get(exps, 0));
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 1));
  int len = strlen(s);
  int ix = 0;
  int p = 0;
  Buf *r = buf_new();
  for (;;) {
    int i = str_cindex_from(s, '%', p);
    if (i == -1) break;

    buf_add(r, str_sub(s, p, i));
    ++i;
    if (i == len)
      EXC_KUT("Bad '%' sequence at end of string");

    char ch = s[i];
    if (ch == '%') {
      buf_cadd(r, ch);
      p = i + 1;
      continue;
    }

    if (
      ch == 't' || ch == 'd' || ch == 's' || ch == 'f' || ch == '.' || ch == 'v'
    ) {
      if (arr_size(a) <= ix)
        EXC_KUT(
          str_f("There are values missing in '%s'", exp_to_js(arr_get(exps, 1)))
        );

      switch (ch) {
      case 't' :
        buf_add(r, exp_rget_bool(arr_get(a, ix)) ? "true" : "false");
        break;
      case 'd':
        buf_add(r, str_f("%ld", exp_rget_int(arr_get(a, ix))));
        break;
      case 'f':
        buf_add(r, exp_to_str(exp_float(exp_rget_float(arr_get(a, ix)))));
        break;
      case 's':
        buf_add(r, exp_rget_string(arr_get(a, ix)));
        break;
      case 'v':
        buf_add(r, exp_to_str(arr_get(a, ix)));
        break;
      default: { // ('.')
          int j = i + 1;
          for (j = i + 1; j < len; ++j) {
            if (s[j] >= '0' && s[j] <= '9') continue;
            break;
          }
          if (s[j] != 'f') EXC_KUT("Bad '%' sequence");
          else {
            char *tpl = str_f("%%%s", str_sub(s, i, j + 1));
            buf_add(r, str_f(tpl, exp_rget_float(arr_get(a, ix))));
            i = j;
          }
        }
      }

      ++ix;
      p = i + 1;
      continue;
    }

    EXC_KUT("Bad '%' sequence");
  }
  if (ix < arr_size(a))
    EXC_KUT(
      str_f("There are too many values in %s", exp_to_js(arr_get(exps, 1)))
    );
  buf_add(r, str_right(s, p));
  return exp_string(str_new(buf_str(r)));
}

// \s -> s
static Exp *from_iso (Arr *exps) {
  CHECK_PARS ("str.fromIso", 1, exps);
  return exp_string(str_from_iso(exp_rget_string(arr_get(exps, 0))));
}

// \[s...] -> s
static Exp *from_runes (Arr *exps) {
  CHECK_PARS ("str.fromRunes", 1, exps);
  // <char>
  Arr *a = arr_map(exp_rget_array(arr_get(exps, 0)), (FMAP)exp_rget_string);
  return exp_string(arr_join(a, ""));
}

// \[i...] -> s
static Exp *from_utf16 (Arr *exps) {
  CHECK_PARS ("str.fromUtf16", 1, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int sz = arr_size(a);
  unsigned *r = ATOMIC(sz * sizeof(unsigned) + 1);
  unsigned *p = r;
  EACH(a, Exp, e) {
    *p++ = exp_rget_int(e);
  }_EACH
  *p = 0;
  return exp_string(str_from_unicode(r));
}

// \s, s -> i
static Exp *findex (Arr *exps) {
  CHECK_PARS ("str.index", 2, exps);
  return exp_int(str_index(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1))
  ));
}

// \s, s, i -> i
static Exp *index_from (Arr *exps) {
  CHECK_PARS ("str.indexFrom", 3, exps);
  return exp_int(str_index_from(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1)),
    exp_rget_int(arr_get(exps, 2))
  ));
}

// \s, s -> i
static Exp *last_index (Arr *exps) {
  CHECK_PARS ("str.lastIndex", 2, exps);
  return exp_int(str_last_index(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1))
  ));
}

// \s -> i
static Exp *len (Arr *exps) {
  CHECK_PARS ("str.len", 1, exps);
  return exp_int(strlen(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *ltrim (Arr *exps) {
  CHECK_PARS ("str.ltrim", 1, exps);
  return exp_string(str_ltrim(exp_rget_string(arr_get(exps, 0))));
}

// \s, s, s -> s
static Exp *replace (Arr *exps) {
  CHECK_PARS ("str.replace", 3, exps);
  return exp_string(str_replace(
    exp_rget_string(arr_get(exps, 0)),
    exp_rget_string(arr_get(exps, 1)),
    exp_rget_string(arr_get(exps, 2))
  ));
}

// \s -> s
static Exp *rtrim (Arr *exps) {
  CHECK_PARS ("str.rtrim", 1, exps);
  return exp_string(str_rtrim(exp_rget_string(arr_get(exps, 0))));
}

// \s, s -> [s...]
static Exp *split (Arr *exps) {
  CHECK_PARS ("str.split", 2, exps);
  return exp_array(arr_map(str_split(
    exp_rget_string(arr_get(exps, 0)),
    exp_rget_string(arr_get(exps, 1))
  ), (FMAP)exp_string));
}

// \s, s -> [s...]
static Exp *split_trim (Arr *exps) {
  CHECK_PARS ("str.splitTrim", 2, exps);
  return exp_array(arr_map(str_split_trim(
    exp_rget_string(arr_get(exps, 0)),
    exp_rget_string(arr_get(exps, 1))
  ), (FMAP)exp_string));
}

// \s, s -> b
static Exp *starts (Arr *exps) {
  CHECK_PARS ("str.starts", 2, exps);
  return exp_bool(str_starts(
    exp_rget_string(arr_get(exps, 0)), exp_rget_string(arr_get(exps, 1))
  ));
}

// \s -> s
static Exp *to_lower (Arr *exps) {
  CHECK_PARS ("str.toLower", 1, exps);
  return exp_string(str_to_lower(exp_rget_string(arr_get(exps, 0))));
}

// \s -> [s...]
static Exp *to_runes (Arr *exps) {
  CHECK_PARS ("str.toRunes", 1, exps);
  // <char>
  Arr *a = str_runes(exp_rget_string(arr_get(exps, 0)));
  return exp_array(arr_map(a, (FMAP)exp_string));
}

// \s -> s
static Exp *to_upper (Arr *exps) {
  CHECK_PARS ("str.toUpper", 1, exps);
  return exp_string(str_to_upper(exp_rget_string(arr_get(exps, 0))));
}

// \s -> [i...]
static Exp *to_utf16 (Arr *exps) {
  CHECK_PARS ("str.toRunes", 1, exps);
  unsigned *u = str_to_unicode(exp_rget_string(arr_get(exps, 0)));
  // <Exp>
  Arr *r = arr_new();
  while (*u) arr_push(r, exp_int(*u++));
  return exp_array(r);
}

// \s -> s
static Exp *trim (Arr *exps) {
  CHECK_PARS ("str.trim", 1, exps);
  return exp_string(str_trim(exp_rget_string(arr_get(exps, 0))));
}

Bfunction md_str_get (char *fname) {
  if (!strcmp(fname, "cmp")) return cmp;
  if (!strcmp(fname, "ends")) return ends;
  if (!strcmp(fname, "fmt")) return fmt;
  if (!strcmp(fname, "fromIso")) return from_iso;
  if (!strcmp(fname, "fromRunes")) return from_runes;
  if (!strcmp(fname, "fromUtf16")) return from_utf16;
  if (!strcmp(fname, "index")) return findex;
  if (!strcmp(fname, "indexFrom")) return index_from;
  if (!strcmp(fname, "lastIndex")) return last_index;
  if (!strcmp(fname, "len")) return len;
  if (!strcmp(fname, "ltrim")) return ltrim;
  if (!strcmp(fname, "replace")) return replace;
  if (!strcmp(fname, "rtrim")) return rtrim;
  if (!strcmp(fname, "split")) return split;
  if (!strcmp(fname, "splitTrim")) return split_trim;
  if (!strcmp(fname, "starts")) return starts;
  if (!strcmp(fname, "toLower")) return to_lower;
  if (!strcmp(fname, "toRunes")) return to_runes;
  if (!strcmp(fname, "toUpper")) return to_upper;
  if (!strcmp(fname, "toUtf16")) return to_utf16;
  if (!strcmp(fname, "trim")) return trim;
  EXC_KUT(fail_bfunction("str", fname));
  return NULL; // Unreachable
}

Exp *md_str_at (char *s, int64_t i) {
  EXC_RANGE(i, 0, strlen(s) - 1);
  return exp_string(str_new_c(s[i]));
}
