// Copyright 31-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal1.h"
#include "fails.h"

void modglobal1_and (Machine *m) {
  Token *tk2 = machine_pop_exc(m, token_INT);
  Token *tk1 = machine_pop_exc(m, token_INT);
  machine_push(m, token_new_int(0, token_int(tk1) && token_int(tk2)));
}

void modglobal1_or (Machine *m) {
  Token *tk2 = machine_pop_exc(m, token_INT);
  Token *tk1 = machine_pop_exc(m, token_INT);
  machine_push(m, token_new_int(0, token_int(tk1) || token_int(tk2)));
}

void modglobal1_not (Machine *m) {
  Token *tk = machine_pop_exc(m, token_INT);
  machine_push(m, token_new_int(0, token_int(tk) == 0 ? 1 : 0));
}

void modglobal1_greater (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_int(tk1) > token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) > token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_int(tk1) > token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) > token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    Token *tk1 = machine_pop_exc(m, token_STRING);
    machine_push(m, token_new_int(
      0, strcmp(token_string(tk1), token_string(tk2)) > 0
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
      machine_push(m, token_new_int(0, token_int(tk1) >= token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) >= token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_int(tk1) >= token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) >= token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    Token *tk1 = machine_pop_exc(m, token_STRING);
    machine_push(m, token_new_int(
      0, strcmp(token_string(tk1), token_string(tk2)) >= 0
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
      machine_push(m, token_new_int(0, token_int(tk1) < token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) < token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_int(tk1) < token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) < token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    Token *tk1 = machine_pop_exc(m, token_STRING);
    machine_push(m, token_new_int(
      0, strcmp(token_string(tk1), token_string(tk2)) < 0
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
      machine_push(m, token_new_int(0, token_int(tk1) <= token_int(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) <= token_int(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_FLOAT);
  if (tk2) {
    Token *tk1 = machine_pop_opt(m, token_INT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_int(tk1) <= token_float(tk2)));
      return;
    }
    tk1 = machine_pop_opt(m, token_FLOAT);
    if (tk1) {
      machine_push(m, token_new_int(0, token_float(tk1) <= token_float(tk2)));
      return;
    }
    fails_types(m, 2, (enum token_Type[]){token_INT, token_FLOAT});
  }

  tk2 = machine_pop_opt(m, token_STRING);
  if (tk2) {
    Token *tk1 = machine_pop_exc(m, token_STRING);
    machine_push(m, token_new_int(
      0, strcmp(token_string(tk1), token_string(tk2)) <= 0
    ));
    return;
  }

  fails_types(m, 3, (enum token_Type[]){token_INT, token_FLOAT, token_STRING});
}
