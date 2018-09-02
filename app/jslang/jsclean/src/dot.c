// Copyright 01-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dot.h"

static char *db = ".jsclean_dots.db";

static Json *to_json(void *a) {
  return achar_to_json(a, json_wstring);
}

static void *from_json(Json *js) {
  return achar_from_json(js, json_rstring);
}

static bool is_prop_start(char ch) {
  return
    (ch >= 'a' && ch <= 'z') ||
    (ch >= 'A' && ch <= 'Z') ||
    ch == '_' || ch == '$' || ch == '('
  ;
}

static bool is_prop_char(char ch) {
  return is_prop_start(ch) || (ch >= '0' && ch <= '9');
}

// ix is after dot
static char *get_id(char *s) {
  int ix = 0;
  Buf *bf = buf_new();
  char ch = s[ix++];
  if (is_prop_start(ch)) {
    buf_cadd(bf, ch);
  } else {
    return "";
  }

  ch = s[ix++];
  while (ch) {
    if (is_prop_char(ch)) {
      buf_cadd(bf, ch);
      ch = s[ix++];
    } else {
      break;
    }
  }
  return buf_to_str(bf);
}

static void print_and_update(
  P *file,
  Achar *old_dot,
  Achar *new_dot,
  char *prop,
  int nline
) {
  if (!ichar_contains(achar_to_it(old_dot), prop)) {
    printf("%s[%d]: Unknown .%s\n", p_relative(file), nline, prop);
  }
  if (!ichar_contains(achar_to_it(new_dot), prop)) {
    achar_add(new_dot, prop);
  }
}

void dot_init (void) {
  if (!file_is_directory("src")) {
    exc_illegal_state("Directory 'src' does not exist");
  }
  if (!file_is_directory("www")) {
    exc_illegal_state("Directory 'www' does not exist");
  }

  if (!file_exists(db)) {
    Map/*Achar*/ *m = map_new();
    file_write(db, (char *)map_to_json(m, to_json));
    return;
  }

  Map/*Achar*/ *props = map_from_json((Json *)file_read(db), from_json);
  Map/*Achar*/ *new_props = map_new();
  bool changed = false;
  EACH(props, Kv, f_ps) {
    char *f = kv_key(f_ps);
    if (!file_exists(f)) {
      changed = true;
    } else {
      arr_add((Arr*)new_props, f_ps);
    }
  }_EACH

  if (changed) {
    file_write(db, (char *)map_to_json(new_props, to_json));
  }
}

Achar *dot_read (P *file) {
  Map/*Achar*/ *m = map_from_json((Json *)file_read(db), from_json);
  return map_oget(m, p_absolute(file), achar_new());
}

void dot_write (P *file, Achar *list) {
  Map/*Achar*/ *m = map_from_json((Json *)file_read(db), from_json);
  map_put(m, p_absolute(file), list);
  file_write(db, (char *)map_to_json(m, to_json));
}

void dot_print_and_update(
  P *file,
  Achar *old_dot,
  Achar *new_dot,
  char *line,
  int nline
) {
  int ix;
  while (true) {
    ix = str_cindex(line, '.');
    if (ix == -1) {
      break;
    }
    line = str_sub_end(line, ix + 1);
    char *prop = get_id(line);
    if (*prop) {
      print_and_update(file, old_dot, new_dot, prop, nline);
    } else {
      int i = 0;
      while (line[i++] == '.') {}
      line = str_sub_end(line, i - 1);
    }
  }
}
