// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modjs.h"
#include "fails.h"
#include "tk.h"

static void isnull (Machine *m) {
  machine_push(m, token_new_int(
    js_is_null((Js *)tk_pop_string(m))
  ));
}

static void rb (Machine *m) {
  machine_push(m, token_new_int(
    js_rb((Js *)tk_pop_string(m))
  ));
}

static void ri (Machine *m) {
  machine_push(m, token_new_int(
    js_rl((Js *)tk_pop_string(m))
  ));
}

static void rf (Machine *m) {
  machine_push(m, token_new_float(
    js_rd((Js *)tk_pop_string(m))
  ));
}

static void rs (Machine *m) {
  machine_push(m, token_new_string(
    js_rs((Js *)tk_pop_string(m))
  ));
}

static void ra (Machine *m) {
  Token *fn (char *js) { return token_new_string(js); }
  machine_push(m, token_new_list(arr_map(
    js_ra((Js *)tk_pop_string(m)), (FCOPY)fn
  )));
}

static void ro (Machine *m) {
  // Map<Js>
  Map *mp = js_ro((Js *)tk_pop_string(m));
  // Map<Token>
  Map *r = map_new();
  EACH(mp, Kv, kv) {
    map_put(r, kv_key(kv), token_new_string(kv_value(kv)));
  }_EACH
  machine_push(m, token_from_pointer(symbol_MAP_, r));
}

static void wn (Machine *m) {
  machine_push(m, token_new_string((char *)js_wn()));
}

static void wb (Machine *m) {
  machine_push(m, token_new_string((char *)js_wb(
    tk_pop_int(m)
  )));
}

static void wi (Machine *m) {
  machine_push(m, token_new_string((char *)js_wl(
    tk_pop_int(m)
  )));
}

static void wf (Machine *m) {
  machine_push(m, token_new_string((char *)js_wd(
    tk_pop_float(m)
  )));
}

static void ws (Machine *m) {
  machine_push(m, token_new_string((char *)js_ws(
    tk_pop_string(m)
  )));
}

static void wa (Machine *m) {
  char *fn (Token *tk) {
    return tk_string(m, tk);
  }
  machine_push(m, token_new_string((char *)js_wa(
    arr_map(tk_pop_list(m), (FCOPY)fn)
  )));
}

static void wo (Machine *m) {
  // Map<char>
  Map *mp = tk_pop_native(m, symbol_MAP_);
  // Map<char>
  Map *r = map_new();
  EACH(mp, Kv, kv) {
    map_put(r, kv_key(kv), tk_string(m, kv_value(kv)));
  }_EACH

  machine_push(m, token_new_string((char *)js_wo(r)));
}

void modjs_from_list (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // Arr<Token>
  Arr *r = arr_new();
  EACH(tk_pop_list(m), Token, tk) {
    machine_push(m, tk);
    machine_process("", machine_pmachines(m), prg);
    arr_push(r, machine_pop(m));
  }_EACH
  machine_push(m, token_new_list(r));
  wa(m);
}

void modjs_to_list (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  ra(m);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(tk_pop_list(m), Token, tk) {
    machine_push(m, tk);
    machine_process("", machine_pmachines(m), prg);
    arr_push(r, machine_pop(m));
  }_EACH
  machine_push(m, token_new_list(r));
}

void modjs_from_map (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // Map<Js>
  Map *r = map_new();
  EACH(map_kvs(tk_pop_native(m, symbol_MAP_)), Kv, kv) {
    machine_push(m, kv_value(kv));
    machine_process("", machine_pmachines(m), prg);
    map_put(r, kv_key(kv), tk_string(m, machine_pop(m)));
  }_EACH

  machine_push(m, token_new_string((char *)js_wo(r)));
}

void modjs_to_map (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // Map<Js>
  Map *js = js_ro((Js *)tk_pop_string(m));

  // Map<Token>
  Map *r = map_new();
  EACH(map_kvs(js), Kv, kv) {
    machine_push(m, token_new_string((char *)kv_value(kv)));
    machine_process("", machine_pmachines(m), prg);
    map_put(r, kv_key(kv), machine_pop(m));
  }_EACH

  machine_push(m, token_from_pointer(symbol_MAP_, r));
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
  add("ro", ro); // STRING - MAP

  add("wn", wn); // INT - STRING
  add("wb", wb); // INT - STRING
  add("wi", wi); // INT - STRING
  add("wf", wf); // FLOAT - STRING
  add("ws", ws); // STRING - STRING
  add("wa", wa); // LIST - STRING
  add("wo", wo); // MAP- STRING

  add("fromList", modjs_from_list); // LIST - STRING
  add("toList", modjs_to_list); // STRING - LIST
  add("fromMap", modjs_from_map); // MAP - STRING
  add("toMap", modjs_to_map); // STRING - MAP


  return r;
}


