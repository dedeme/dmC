// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modsys2.h"
#include "fails.h"

void modsys2_size (Machine *m) {
  Token *tk = machine_pop(m);
  enum token_Type t = token_type(tk);
  if (t == token_STRING)
    machine_push(m, token_new_int(strlen(token_string(tk))));
  else if (t == token_BLOB)
    machine_push(m, token_new_int(bytes_len(token_blob(tk))));
  else if (t == token_LIST)
    machine_push(m, token_new_int(arr_size(token_list(tk))));
  else {
    machine_push(m, tk);
    fails_types(
      m, 3, (enum token_Type[]){token_STRING, token_BLOB, token_LIST}
    );
  }
}

void modsys2_get (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    int ix = token_int(tk2);
    Token *tk1 = machine_pop_opt(m, token_BLOB);
    if (tk1) {
      Bytes *bs = token_blob(tk1);
      if (ix < 0 || ix >= bytes_len(bs))
        fails_range(m, 0, bytes_len(bs) - 1, ix);

      machine_push(m, token_new_int(bytes_bs(bs)[ix]));
      return;
    }
    tk1 = machine_pop_opt(m, token_LIST);
    if (tk1) {
      // Arr<Token>
      Arr *a = token_list(tk1);
      if (ix < 0 || ix >= arr_size(a))
        fails_range(m, 0, arr_size(a) - 1, ix);

      machine_push(m, arr_start(a)[ix]);
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_BLOB, token_LIST});
  }
  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    Token *tk1 = machine_pop_exc(m, token_LIST);
    // Arr<Token>
    Arr *a = token_list(tk1);
    char *key = token_string(machine_pop_exc(m, token_STRING));
    int i;
    for (i = 0; i < arr_size(a); i += 2) {
      Token *tk = arr_get(a, i);
      if (token_type(tk) != token_STRING)
        machine_fail(m, "function get: List is not a map");
      if (str_eq(key, token_string(tk))) {
        ++i;
        if (i >= arr_size(a))
          machine_fail(m, "function get: Value at end of list is missing");
        machine_push(m, arr_get(a, i));
        return;
      }
    }
    machine_fail(m, str_f("Key '%s' not found", key));
  }
  fails_types(m, 2, (enum token_Type[]){token_INT, token_STRING});
}

void modsys2_set (Machine *m) {
  Token *tk3 = machine_pop(m);
  Token *tk2 = machine_pop_exc(m, token_INT);
  int ix = token_int(tk2);
  Token *tk1 = machine_peek_opt(m, token_BLOB);
  if (tk1) {
    if (token_type(tk3) != token_INT) {
      machine_push(m, tk2);
      fails_type(m, token_INT);
    }
    Bytes *bs = token_blob(tk1);
    if (ix < 0 || ix >= bytes_len(bs))
      fails_range(m, 0, bytes_len(bs) - 1, ix);

    bytes_bs(bs)[ix] = token_int(tk3);
    return;
  }
  tk1 = machine_peek_opt(m, token_LIST);
  if (tk1) {
    // Arr<Token>
    Arr *a = token_list(tk1);
    if (ix < 0 || ix >= arr_size(a))
      fails_range(m, 0, arr_size(a) - 1, ix);

    arr_start(a)[ix] = tk3;
    return;
  }
  fails_types(m, 2, (enum token_Type[]){token_BLOB, token_LIST});
}

static void sub (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  Token *tk = machine_pop(m);
  enum token_Type t = token_type(tk);
  if (t == token_STRING) {
    char *s = token_string(tk);
    bounds(strlen(s));
    machine_push(m, token_new_string(str_sub(s, begin, end)));
  } else if (t == token_BLOB) {
    Bytes *bs = token_blob(tk);

    bounds(bytes_len(bs));
    int df = end - begin;

    Token *tk = token_new_blob(df);
    if (df) memcpy(bytes_bs(token_blob(tk)), bytes_bs(bs) + begin, df);
    machine_push(m, tk);
  } else if (t == token_LIST) {
    // Arr<Token>
    Arr *a = token_list(tk);

    bounds(arr_size(a));
    int df = end - begin;

    Arr *ra;
    if (df) {
      Token **tks = GC_MALLOC(df * sizeof(Token *));
      memcpy(tks, (Token **)arr_start(a), df);
      ra = arr_new_c(df, (void **)tks);
    } else {
      ra = arr_new();
    }
    machine_push(m, token_new_list(ra));
  } else {
    machine_push(m, tk);
    fails_types(
      m, 3, (enum token_Type[]){token_STRING, token_BLOB, token_LIST}
    );
  }
}

void modsys2_sub (Machine *m) {
  Token *tk3 = machine_pop_opt(m, token_INT);
  int end = token_int(tk3);
  Token *tk2 = machine_pop_opt(m, token_INT);
  int begin = token_int(tk2);
  sub(m, begin, end, 0);
}

void modsys2_left (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  sub(m, 0, cut, 0);
}

void modsys2_right (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  sub(m, cut, 0, 1);
}
