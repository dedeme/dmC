// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modjs.h"
#include "fails.h"
#include "Machine.h"

static void isnull (Machine *m) {
  machine_push(m, token_new_int(0,
    js_is_null((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void rb (Machine *m) {
  machine_push(m, token_new_int(0,
    js_rb((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void ri (Machine *m) {
  machine_push(m, token_new_int(0,
    js_ri((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void rf (Machine *m) {
  machine_push(m, token_new_float(0,
    js_rd((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void rs (Machine *m) {
  machine_push(m, token_new_string(0,
    js_rs((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void ra (Machine *m) {
  Token *fn (char *js) { return token_new_string(0, js); }
  machine_push(m, token_new_list(0,
    arr_map(
      js_ra((Js *)token_string(machine_pop_exc(m, token_STRING))), (FCOPY)fn
    )
  ));
}

static void ro (Machine *m) {
  // Map<Js>
  Map *jsm = js_ro((Js *)token_string(machine_pop_exc(m, token_STRING)));
  // Arr<Token>
  Arr *r = arr_new();
  EACH(jsm, Kv, kv) {
    arr_push(r, token_new_string(0, kv_key(kv)));
    arr_push(r, token_new_string(0, kv_value(kv)));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void wn (Machine *m) {
  machine_push(m, token_new_string(0, (char *)js_wn()));
}

static void wb (Machine *m) {
  machine_push(m, token_new_string(0, (char *)js_wb(
    token_int(machine_pop_exc(m, token_INT))
  )));
}

static void wi (Machine *m) {
  machine_push(m, token_new_string(0, (char *)js_wi(
    token_int(machine_pop_exc(m, token_INT))
  )));
}

static void wf (Machine *m) {
  machine_push(m, token_new_string(0, (char *)js_wd(
    token_float(machine_pop_exc(m, token_FLOAT))
  )));
}

static void ws (Machine *m) {
  machine_push(m, token_new_string(0, (char *)js_ws(
    token_string(machine_pop_exc(m, token_STRING))
  )));
}

static void wa (Machine *m) {
  char *fn (Token *tk) {
    if (token_type(tk) != token_STRING) fails_type_in(m, token_STRING, tk);
    return token_string(tk);
  }
  machine_push(m, token_new_string(0, (char *)js_wa(
    arr_map(token_list(machine_pop_exc(m, token_LIST)), (FCOPY)fn)
  )));
}

static void wo (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  // Map<char>
  Map *r = map_new();
  char *key = NULL;
  EACH(a, Token, tk) {
    if (token_type(tk) != token_STRING) fails_type_in(m, token_STRING, tk);
    char *s = token_string(tk);
    if (key) {
      arr_push((Arr *)r, kv_new(key, s));
      key = NULL;
    } else {
      key = s;
    }
  }_EACH
  if (key) fails_size_list(m, a, arr_size(a) + 1);
  machine_push(m, token_new_string(0, (char *)js_wo(r)));
}

static void fromlist (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    machine_push(m, tk);
    machine_process("", machine_pmachines(m), prg);
    arr_push(r, machine_pop(m));
  }_EACH
  machine_push(m, token_new_list(0, r));
  wa(m);
}

static void tolist (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  ra(m);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    machine_push(m, tk);
    machine_process("", machine_pmachines(m), prg);
    arr_push(r, machine_pop(m));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void frommap (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
    // Arr<Token>
    Arr *a = token_list(tk);
    if (arr_size(a) != 2) fails_size_list(m, a, 2);
    arr_push(r, *arr_start(a));
    machine_push(m, arr_get(a, 1));
    machine_process("", machine_pmachines(m), prg);
    arr_push(r, machine_pop(m));
  }_EACH
  machine_push(m, token_new_list(0, r));
  wo(m);
}

static void tomap (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  ro(m);
  // Arr<Token>
  Arr *r = arr_new();
  Token *key = NULL;
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  EACH(a, Token, tk) {
    if (key) {
      machine_push(m, tk);
      machine_process("", machine_pmachines(m), prg);
      arr_push(r, token_new_list(0, arr_new_from(key, machine_pop(m), NULL)));
      key = NULL;
    } else {
      key = tk;
    }
  }_EACH
  if (key) fails_size_list(m, a, arr_size(a) + 1);
  machine_push(m, token_new_list(0, r));
}

// Resturns Map<primitives_Fn>
Map *modjs_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "null?", isnull); // STRING - INT
  map_put(r, "rb", rb); // STRING - INT
  map_put(r, "ri", ri); // STRING - INT
  map_put(r, "rf", rf); // STRING - FLOAT
  map_put(r, "rs", rs); // STRING - STRING
  map_put(r, "ra", ra); // STRING - LIST
  map_put(r, "ro", ro); // STRING - OBJ

  map_put(r, "wn", wn); // INT - STRING
  map_put(r, "wb", wb); // INT - STRING
  map_put(r, "wi", wi); // INT - STRING
  map_put(r, "wf", wf); // FLOAT - STRING
  map_put(r, "ws", ws); // STRING - STRING
  map_put(r, "wa", wa); // LIST - STRING
  map_put(r, "wo", wo); // OBJ- STRING

  map_put(r, "fromList", fromlist); // LIST - STRING
  map_put(r, "toList", tolist); // STRING - LIST
  map_put(r, "fromMap", frommap); // MAP - STRING
  map_put(r, "toMap", tomap); // STRING - MAP


  return r;
}


