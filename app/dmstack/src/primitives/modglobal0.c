// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal0.h"
#include "fails.h"
#include "tk.h"

void modglobal0_add (Machine *m) {
  Token *tk = machine_pop_opt(m, token_INT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_int(token_int(tk2) + token_int(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m,
        token_new_float(token_float(tk2) + (double)token_int(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }
  tk = machine_pop_opt(m, token_FLOAT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m,
        token_new_float((double)token_int(tk2) + token_float(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk2) + token_float(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_STRING);
  if (tk) {
    machine_push(m, token_new_string(str_cat(
      tk_pop_string(m), token_string(tk), NULL
    )));
    return;
  }

  tk = machine_pop_opt(m, token_LIST);
  if (tk) {
    machine_push(m, token_new_list(it_to(it_cat(
      it_from(tk_pop_list(m)), it_from(token_list(tk))
    ))));
    return;
  }

  fails_types(m, 4, (enum token_Type[]){
    token_INT, token_FLOAT, token_STRING, token_LIST
  });
}

void modglobal0_sub (Machine *m) {
  Token *tk = machine_pop_opt(m, token_INT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_int(token_int(tk2) - token_int(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m,
        token_new_float(token_float(tk2) - (double)token_int(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_FLOAT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m,
        token_new_float((double)token_int(tk2) - token_float(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk2) - token_float(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  fails_types(m, 2, (enum token_Type[]){
    token_INT, token_FLOAT
  });
}

void modglobal0_mul (Machine *m) {
  Token *tk = machine_pop_opt(m, token_INT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_int(token_int(tk2) * token_int(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m,
        token_new_float(token_float(tk2) * (double)token_int(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_FLOAT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m,
        token_new_float((double)token_int(tk2) * token_float(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk2) * token_float(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  fails_types(m, 2, (enum token_Type[]){
    token_INT, token_FLOAT
  });
}

void modglobal0_div (Machine *m) {
  Token *tk = machine_pop_opt(m, token_INT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m, token_new_int(token_int(tk2) / token_int(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m,
        token_new_float(token_float(tk2) / (double)token_int(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk = machine_pop_opt(m, token_FLOAT);
  if (tk) {
    Token *tk2 = machine_pop_opt(m, token_INT);
    if (tk2) {
      machine_push(m,
        token_new_float((double)token_int(tk2) / token_float(tk)));
      return;
    }
    tk2 = machine_pop_opt(m, token_FLOAT);
    if (tk2) {
      machine_push(m, token_new_float(token_float(tk2) / token_float(tk)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  fails_types(m, 2, (enum token_Type[]){
    token_INT, token_FLOAT
  });
}

void modglobal0_mod (Machine *m) {
  Int i = tk_pop_int(m);
  Int i2 = tk_pop_int(m);
  machine_push(m, token_new_int(i2 % i));
}

void modglobal0_incr (Machine *m) {
  machine_push(m, token_new_int(tk_pop_int(m) + 1));
}

void modglobal0_decr (Machine *m) {
  machine_push(m, token_new_int( tk_pop_int(m) - 1));
}
