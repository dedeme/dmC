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
    js_rl((Js *)token_string(machine_pop_exc(m, token_STRING)))
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
  machine_push(m, token_new_list(0, arr_map(
    js_ra((Js *)token_string(machine_pop_exc(m, token_STRING))), (FCOPY)fn
  )));
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

static void rm (Machine *m) {
  // Map<Js>
  Map *jsm = js_ro((Js *)token_string(machine_pop_exc(m, token_STRING)));
  // Arr<Token>
  Arr *r = arr_new();
  EACH(jsm, Kv, kv) {
    // Arr<Token>
    Arr *e = arr_new();
    arr_push(e, token_new_string(0, kv_key(kv)));
    arr_push(e, token_new_string(0, kv_value(kv)));
    arr_push(r, token_new_list(0, e));
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
  machine_push(m, token_new_string(0, (char *)js_wl(
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

static void wm (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  // Map<char>
  Map *r = map_new();
  EACH(a, Token, tk) {
    if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
    // Arr<Token>
    Arr *e = token_list(tk);
    if (arr_size(e) != 2) fails_size_list(m, e, 2);
    Token *tkkey = *arr_start(e);
    if (token_type(tkkey) != token_STRING)
      fails_type_in(m, token_STRING, tkkey);
    Token *tkval = *(arr_start(e) + 1);
    if (token_type(tkval) != token_STRING)
      fails_type_in(m, token_STRING, tkval);
    map_put(r, token_string(tkkey), token_string(tkval));
  }_EACH
  machine_push(m, token_new_string(0, (char *)js_wo(r)));
}

void modjs_from_list (Machine *m) {
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

void modjs_to_list (Machine *m) {
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

void modjs_from_map (Machine *m) {
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

void modjs_to_map (Machine *m) {
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

Pmodule *modjs_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("null?", isnull); // STRING - INT
  add("rb", rb); // STRING - INT
  add("ri", ri); // STRING - INT
  add("rf", rf); // STRING - FLOAT
  add("rs", rs); // STRING - STRING
  add("ra", ra); // STRING - LIST
  add("ro", ro); // STRING - OBJ
  add("rm", rm); // STRING - MAP

  add("wn", wn); // INT - STRING
  add("wb", wb); // INT - STRING
  add("wi", wi); // INT - STRING
  add("wf", wf); // FLOAT - STRING
  add("ws", ws); // STRING - STRING
  add("wa", wa); // LIST - STRING
  add("wo", wo); // OBJ- STRING
  add("wm", wm); // MAP- STRING

  add("fromList", modjs_from_list); // LIST - STRING
  add("toList", modjs_to_list); // STRING - LIST
  add("fromMap", modjs_from_map); // MAP - STRING
  add("toMap", modjs_to_map); // STRING - MAP


  return r;
}


