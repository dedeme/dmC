// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modsys0.h"
#include "fails.h"

void modsys0_add (Machine *m) {
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
    Token *tk2 = machine_pop_opt(m, token_STRING);
    if (tk2) {
      machine_push(m, token_new_string(str_cat(
        token_string(tk2), token_string(tk), NULL
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
        it_from(token_list(tk2)), it_from(token_list(tk))
      ))));
      return;
    }
    fails_type(m, token_STRING);
  }

  fails_types(m, 4, (enum token_Type[]){
    token_INT, token_FLOAT, token_STRING, token_LIST
  });
}

void modsys0_sub (Machine *m) {
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

void modsys0_mul (Machine *m) {
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

void modsys0_div (Machine *m) {
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

void modsys0_mod (Machine *m) {
  Token *tk = machine_pop_exc(m, token_INT);
  Token *tk2 = machine_pop_exc(m, token_INT);
  machine_push(m, token_new_int(token_int(tk2) % token_int(tk)));
}

void modsys0_incr (Machine *m) {
  machine_push(
    m, token_new_int(token_int(machine_pop_exc(m, token_INT)) + 1)
  );
}

void modsys0_decr (Machine *m) {
  machine_push(
    m, token_new_int(token_int(machine_pop_exc(m, token_INT)) - 1)
  );
}
