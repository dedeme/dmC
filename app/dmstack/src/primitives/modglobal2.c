// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal2.h"
#include "fails.h"

void modglobal2_size (Machine *m) {
  Token *tk = machine_pop(m);
  enum token_Type t = token_type(tk);
  if (t == token_STRING)
    machine_push(m, token_new_int(0, strlen(token_string(tk))));
  else if (t == token_BLOB)
    machine_push(m, token_new_int(0, bytes_len(token_blob(tk))));
  else if (t == token_LIST)
    machine_push(m, token_new_int(0, arr_size(token_list(tk))));
  else {
    machine_push(m, tk);
    fails_types(
      m, 3, (enum token_Type[]){token_STRING, token_BLOB, token_LIST}
    );
  }
}

void modglobal2_get (Machine *m) {
  Token *tk2 = machine_pop_exc(m, token_INT);
  int ix = token_int(tk2);
  Token *tk1 = machine_pop_opt(m, token_BLOB);
  if (tk1) {
    Bytes *bs = token_blob(tk1);
    if (ix < 0 || ix >= bytes_len(bs))
      fails_range(m, 0, bytes_len(bs) - 1, ix);

    machine_push(m, token_new_int(0, bytes_bs(bs)[ix]));
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

static void setboth (Machine *m, int isplus) {
  Token *tk3 = machine_pop(m);
  Token *tk2 = machine_pop_exc(m, token_INT);
  int ix = token_int(tk2);
  Token *tk1 = isplus
    ? machine_peek_opt(m, token_BLOB)
    : machine_pop_opt(m, token_BLOB)
  ;
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
  tk1 = isplus
    ? machine_peek_opt(m, token_LIST)
    : machine_pop_opt(m, token_LIST)
  ;
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

void modglobal2_set (Machine *m) {
  setboth(m, 0);
}

void modglobal2_setplus (Machine *m) {
  setboth(m, 1);
}

static void upboth (Machine *m, int isplus) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_peek(m);
  machine_push(m, tk2);
  modglobal2_get(m);

  machine_process("", machine_pmachines(m), prg);

  Token *tk3 = machine_pop(m);
  machine_push(m, tk1);
  machine_push(m, tk2);
  machine_push(m, tk3);
  setboth(m, isplus);
}

void modglobal2_up (Machine *m) {
  upboth(m, 0);
}

void modglobal2_upplus (Machine *m) {
  upboth(m, 1);
}

void modglobal2_ref_get (Machine *m) {
  Token *tk = machine_pop_exc(m, token_LIST);
  // Arr<Token>
  Arr *a = token_list(tk);
  if (arr_size(a) != 1) fails_size_list(m, a, 1);

  machine_push(m, *arr_start(a));
}

static void refsetboth (Machine *m, int isplus) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = isplus
    ? machine_peek_opt(m, token_LIST)
    : machine_pop_opt(m, token_LIST)
  ;
  // Arr<Token>
  Arr *a = token_list(tk1);
  if (arr_size(a) != 1) fails_size_list(m, a, 1);

  *arr_start(a) = tk2;
}

void modglobal2_ref_set (Machine *m) {
  refsetboth(m, 0);
}

void modglobal2_ref_setplus (Machine *m) {
  refsetboth(m, 1);
}

static void refupboth (Machine *m, int isplus) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *tk1 = machine_peek(m);
  modglobal2_ref_get(m);

  machine_process("", machine_pmachines(m), prg);

  Token *tk2 = machine_pop(m);
  machine_push(m, tk1);
  machine_push(m, tk2);
  refsetboth(m, isplus);
}

void modglobal2_ref_up (Machine *m) {
  refupboth(m, 0);
}

void modglobal2_ref_upplus (Machine *m) {
  refupboth(m, 1);
}
