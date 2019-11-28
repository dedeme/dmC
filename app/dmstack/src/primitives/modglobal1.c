// Copyright 31-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal1.h"
#include "fails.h"
#include "tk.h"

void modglobal1_and (Machine *m) {
  Token *tk = machine_pop_opt(m, token_LIST);
  if (tk) {
    Int i1 = tk_pop_int(m);
    if (i1) {
      Machine *m2 = machine_isolate_process("", machine_pmachines(m), tk);
      if (arr_size(machine_stack(m2)) != 1)
        machine_fail(m, str_f(
          "Lazy '&&' stack. Expected size: 1, actual size: %d.",
          arr_size(machine_stack(m2))
      ));
      machine_push(m, machine_pop_exc(m2, token_INT));
    } else {
      machine_push(m, token_new_int(0));
    }
  } else {
    Int i2 = tk_pop_int(m);
    Int i1 = tk_pop_int(m);
    machine_push(m, token_new_int(i1 && i2));
  }
}

void modglobal1_or (Machine *m) {
  Token *tk = machine_pop_opt(m, token_LIST);
  if (tk) {
    Int i1 = tk_pop_int(m);
    if (i1) {
      machine_push(m, token_new_int(1));
    } else {
      Machine *m2 = machine_isolate_process("", machine_pmachines(m), tk);
      if (arr_size(machine_stack(m2)) != 1)
        machine_fail(m, str_f(
          "Lazy '||' stack. Expected size: 1, actual size: %d.",
          arr_size(machine_stack(m2))
      ));
      machine_push(m, machine_pop_exc(m2, token_INT));
    }
  } else {
    Int i2 = tk_pop_int(m);
    Int i1 = tk_pop_int(m);
    machine_push(m, token_new_int(i1 || i2));
  }
}

void modglobal1_not (Machine *m) {
  Int i = tk_pop_int(m);
  machine_push(m, token_new_int(!i));
}

void modglobal1_greater (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) > token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) > token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) > token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) > token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    machine_push(m, token_new_int(
      strcmp(tk_pop_string(m), token_string(tk2)) > 0
    ));
    return;
  }

  fails_types(m, 3, (enum token_Type[]){token_INT, token_FLOAT, token_STRING});
}


void modglobal1_greater_eq (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) >= token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) >= token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) >= token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) >= token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    machine_push(m, token_new_int(
      strcmp(tk_pop_string(m), token_string(tk2)) >= 0
    ));
    return;
  }

  fails_types(m, 3, (enum token_Type[]){token_INT, token_FLOAT, token_STRING});
}

void modglobal1_less (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) < token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) < token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) < token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) < token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    machine_push(m, token_new_int(
      strcmp(tk_pop_string(m), token_string(tk2)) < 0
    ));
    return;
  }

  fails_types(m, 3, (enum token_Type[]){token_INT, token_FLOAT, token_STRING});
}

void modglobal1_less_eq (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) <= token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) <= token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(token_int(tk1) <= token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(token_float(tk1) <= token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    machine_push(m, token_new_int(
      strcmp(tk_pop_string(m), token_string(tk2)) <= 0
    ));
    return;
  }

  fails_types(m, 3, (enum token_Type[]){token_INT, token_FLOAT, token_STRING});
}
