// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/stat.h"
#include "kut/js.h"
#include "data/tp.h"
#include "data/tp3.h"
#include "data/tp4.h"
#include "data/tp5.h"
#include "cts.h"

static Stat *new (int ln, StatT tp, void *value) {
  Stat *this = MALLOC(Stat);
  this->ln = ln;
  this->tp = tp;
  this->value = value;
  return this;
}

Stat *stat_new_error (int ln, char *msg) {
  return new(ln, stat_error, msg);
}

Stat *stat_new_indexed (int ln, Type *type, char *fn, Arr* pars) {
  return new(ln, stat_indexed, tp3_mk(type, fn, pars));
}

Stat *stat_new_assign (int ln, Exp *symSq, Exp *value) {
  return new(ln, stat_assign, tp_mk(symSq, value));
}

Stat *stat_new_multiassign (int ln, Arr *syms, Exp *value) {
  return new(ln, stat_multiassign, tp_mk(syms, value));
}

Stat *stat_new_addas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_addas, tp_mk(var, value));
}

Stat *stat_new_subas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_subas, tp_mk(var, value));
}

Stat *stat_new_mulas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_mulas, tp_mk(var, value));
}

Stat *stat_new_divas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_divas, tp_mk(var, value));
}

Stat *stat_new_modas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_modas, tp_mk(var, value));
}

Stat *stat_new_andas (int ln, Exp *var, Exp *value) {
  return new(ln, stat_andas, tp_mk(var, value));
}

Stat *stat_new_oras (int ln, Exp *var, Exp *value) {
  return new(ln, stat_oras, tp_mk(var, value));
}

Stat *stat_new_function (int ln, Exp *fn) {
    return new(ln, stat_function, fn);
}

Stat *stat_new_block (int ln, Arr *sts) {
  return new(ln, stat_block, sts);
}

Stat *stat_new_close_block (int ln) {
  return new(ln, stat_close_block, "");
}

Stat *stat_new_break (int ln) {
  return new(ln, stat_break, "");
}

Stat *stat_new_continue (int ln) {
  return new(ln, stat_continue, "");
}

Stat *stat_new_return  (int ln, Opt *value) {
  return new(ln, stat_return, value);
}

Stat *stat_new_throw (int ln, Exp *value) {
  return new(ln, stat_throw, value);
}

Stat *stat_new_trace (int ln, int is_complete, Exp *value) {
  return new(ln, stat_trace, tp_mk(is_complete ? cts_true(): cts_false(), value));
}

Stat *stat_new_while (int ln, Opt* cond, Stat *st) {
  return new(ln, stat_while, tp_mk(cond, st));
}

Stat *stat_new_if (int ln, Exp *cond, Stat *st_true, Opt *st_false) {
  return new(ln, stat_if, tp3_mk(cond, st_true, st_false));
}

Stat *stat_new_switch (int ln, Exp *val, Arr* entries) {
  return new(ln, stat_switch, tp_mk(val, entries));
}

Stat *stat_new_for (int ln, Opt *ix, char *var, Exp *ct, Stat *st) {
  return new(ln, stat_for, tp4_mk(ix, var, ct, st));
}

Stat *stat_new_for_range (
  int ln, char *var, Exp *v1, Exp *v2, Opt *v3, Stat *st
)  {
  return new(ln, stat_for_range, tp5_mk(var, v1, v2, v3, st));
}

Stat *stat_new_try (int ln, Stat *st, char *var, Stat *st_catch) {
  return new(ln, stat_try, tp3_mk(st, var, st_catch));
}

Stat *stat_new_import (int ln, char *path, Opt *name) {
  return new(ln, stat_import, tp_mk(path, name));
}

char *stat_to_str (Stat *this) {
  char *estr (Exp *e) { return exp_to_str(e); }
  char *sstr (Stat *st) { return stat_to_str(st); }

  void *v = this->value;
  switch (this->tp) {
    case stat_error: {
      return v;
    }
    case stat_indexed: {
      Tp3 *t = v;
      return str_f("%s:%s%s", t->e2, type_to_str(t->e1), arr_cjoin(t->e3, ','));
    }
    case stat_assign: {
      Tp *t = v;
      return str_f("%s=%s", estr(t->e1), estr(t->e2));
    }
    case stat_multiassign: {
      Tp *t = v;
      return str_f("%s=%s", arr_cjoin(t->e1, ','), estr(t->e2));
    }
    case stat_addas: {
      Tp *t = v;
      return str_f("%s+=%s", estr(t->e1), estr(t->e2));
    }
    case stat_subas: {
      Tp *t = v;
      return str_f("%s-=%s", estr(t->e1), estr(t->e2));
    }
    case stat_mulas: {
      Tp *t = v;
      return str_f("%s*=%s", estr(t->e1), estr(t->e2));
    }
    case stat_divas: {
      Tp *t = v;
      return str_f("%s/=%s", estr(t->e1), estr(t->e2));
    }
    case stat_modas: {
      Tp *t = v;
      return str_f("%s%=%s", estr(t->e1), estr(t->e2));
    }
    case stat_andas: {
      Tp *t = v;
      return str_f("%s&=%s", estr(t->e1), estr(t->e2));
    }
    case stat_oras: {
      Tp *t = v;
      return str_f("%s|=%s", estr(t->e1), estr(t->e2));
    }
    case stat_function: return estr((Exp *)v);
    case stat_block:
      return str_f("{\n%s\n}", arr_cjoin(arr_map((Arr *)v, (FMAP)sstr), '\n'));
    case stat_close_block: return "}";
    case stat_break: return "break";
    case stat_continue: return "continue";
    case stat_return: {
      Exp *e = opt_get(v);
      return e ? str_f("return %s", estr(e)) : "return";
    }
    case stat_throw: return str_f("throw %s", estr(v));
    case stat_trace: {
      Tp *t = v;
      char *sep = *((int *)t->e1) ? ":" : "";
      return str_f("trace%s%s", sep, estr(t->e2));
    }
    case stat_while: {
      Tp *t = v;
      Exp *econd = opt_get(t->e1);
      char *cond = econd ? estr(econd) : "";
      return str_f("while(%s)%s", cond, sstr(t->e2));
    }
    case stat_if: {
      Tp3 *t = v;
      Stat *st_else = opt_get(t->e3);
      char *els = st_else ? str_f("else %s", sstr(st_else)) : "";
      return str_f("if(%s)%s%s", estr(t->e1), sstr(t->e2), els);
    }
    case stat_switch: {
      Tp *t = v;
      char *fmap(Tp *tp) {
        Arr *cs = tp->e1;
        return str_f("%s:%s",
          arr_cjoin(arr_map(cs, (FMAP)estr), ','), sstr(tp->e2)
        );
      }
      return str_f("switch(%s){%s}",
        estr(t->e1),
        arr_join(arr_map((Arr *)t->e2, (FMAP)fmap), ";\n")
      );
    }
    case stat_for: {
      Tp4 *t = v;
      char *ix = opt_get(t->e1);
      ix = ix ? str_f("%s,", ix ): "";
      return str_f("for(%s%s=%s)%s",
        ix, t->e2, estr(t->e3), sstr(t->e4)
      );
    }
    case stat_for_range: {
      Tp5 *t = v;
      char *step = opt_get(t->e4);
      step = step ? str_f(":%s", step ): "";
      return str_f("for(%s=%s:%s%s)%s",
        t->e1, estr(t->e2), estr(t->e3), step, sstr(t->e5)
      );
    }
    case stat_try: {
      Tp4 *t = v;
      Stat *sfinally = opt_get(t->e4);
      char *fin = sfinally ? str_f(" finally %s", sstr(sfinally)) : "";
      return str_f("try %s catch(%s) %s%s",
        sstr(t->e1), t->e2, sstr(t->e3), fin
      );
    }
    case stat_import:
      Tp *t = v;
      char *local_name = opt_get(t->e2);
      local_name = local_name ? str_f(":%s", local_name): "";
      return str_f("import %s%s", js_ws(t->e1), local_name);

    default: {
      EXC_KUTT(str_f("Unknown statement type %d", this->tp));
      return NULL; // Unreachable
    }
  };
}
