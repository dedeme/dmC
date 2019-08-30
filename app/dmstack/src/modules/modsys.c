// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modsys.h"
#include "fails.h"


static void println (Machine *m) {
  puts(token_to_str(machine_pop(m)));
}

static void add (Machine *m) {
  Token *tk = machine_pop_opt(m, token_INT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_int(token_int(tk) + token_int(tk2)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_int(tk) + token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_FLOAT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk) + token_int(tk2)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk) + token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_STRING);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_STRING);
    if (tk2) {
      machine_push(m, token_new_string(str_cat(
        token_string(tk), token_string(tk2), NULL
      )));
      return;
    }
    fails_type(m, token_STRING);
  }

  tk = machine_pop_opt(m, token_LIST);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_LIST);
    if (tk2) {
      machine_push(m, token_new_list(it_to(it_cat(
        it_from(token_list(tk)), it_from(token_list(tk2))
      ))));
      return;
    }
    fails_type(m, token_STRING);
  }

  fails_types(m, 4, (enum token_Type[]){
    token_INT, token_FLOAT, token_STRING, token_LIST
  });
}

// Resturns Map<primitives_Fn>
Map *modsys_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "println", println);

  map_put(r, "+", add);

  return r;
}
