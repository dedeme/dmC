// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/exp.h"
#include "kut/js.h"
#include "data/tp.h"
#include "data/tp3.h"
#include "data/stat.h"

static Exp *new (int ln, ExpT tp, void *value) {
  Exp *this = MALLOC(Exp);
  this->ln = ln;
  this->tp = tp;
  this->value = value;
  return this;
}

Exp *exp_new_error (int ln, char *msg) {
  return new(ln, exp_error, msg);
}

Exp *exp_new_boolean (int ln, char *v) {
  return new(ln, exp_boolean, *v == 't' ? "1" : "0");
}

Exp *exp_new_integer (int ln, char *v) {
  return new(ln, exp_integer, v);
}

Exp *exp_new_float (int ln, char *v) {
  return new(ln, exp_float, v);
}

Exp *exp_new_string (int ln, char *v) {
  return new(ln, exp_string, v);
}

Exp *exp_new_array (int ln, Arr *v) {
  return new(ln, exp_array, v);
}

Exp *exp_new_array0 (int ln, Opt *tp) {
  return new(ln, exp_array0, tp);
}

Exp *exp_new_dictionary (int ln, Arr *v) {
  return new(ln, exp_dictionary, v);
}

Exp *exp_new_dictionary0 (int ln, Opt *tp) {
  return new(ln, exp_dictionary0, tp);
}

Exp *exp_new_tuple (int ln, Arr *v) {
  return new(ln, exp_tuple, v);
}

Exp *exp_new_function (int ln, Type *tp, Arr *pars, Stat *code) {
  return new(ln, exp_function, tp3_mk(tp, pars, code));
}

Exp *exp_new_symbol (int ln, char *v) {
  return new(ln, exp_symbol, v);
}

Exp *exp_new_range (int ln, Arr *v) {
  return new(ln, exp_range, v);
}

Exp *exp_new_point (int ln, char *md, char *fn) {
  return new(ln, exp_point, tp_mk(md, fn));
}

Exp *exp_new_square (int ln, Exp *ct, Exp *ix) {
  return new(ln, exp_square, tp_mk(ct, ix));
}

Exp *exp_new_slice (int ln, Exp *ct, Opt *start, Opt *end) {
  return new(ln, exp_slice, tp3_mk(ct, start, end));
}

Exp *exp_new_parenthesis (int ln, Exp *fn, Arr *pars) {
  return new(ln, exp_parenthesis, tp_mk(fn, pars));
}

Exp *exp_new_group (int ln, Exp *v) {
  return new(ln, exp_group, v);
}

Exp *exp_new_unary (int ln, char *sym, Exp *v) {
  return new(ln, exp_unary, tp_mk(sym, v));
}

Exp *exp_new_binary (int ln, char *sym, Exp *v1, Exp *v2) {
  return new(ln, exp_binary, tp3_mk(sym, v1, v2));
}

Exp *exp_new_ternary  (int ln, Exp *v1, Exp *v2, Exp *v3) {
  return new(ln, exp_ternary, tp3_mk(v1, v2, v3));
}

Exp *exp_new_switch (int ln, Exp *ex, Arr *es) {
  return new(ln, exp_switch, tp_mk(ex, es));
}

char *exp_type_to_str (Exp *this) {
  return ((char*[]) {
    "error", "boolean", "integer", "float", "string",
    "array", "array0", "dictionary", "dictionary0",
    "tuple", "function", "symbol", "range",
    "point", "subindex", "slice", "function call",
    "group", "unary", "binary", "ternary", "switcht"
  })[this->tp];
}

char *exp_to_str (Exp *this) {
  switch (this->tp) {
    case exp_boolean:
      return *((char *)this->value) == '1' ? "true" : "false";
    case exp_string:
      return js_ws(this->value);
    case exp_array:
      return str_f("[%s]",
        arr_cjoin(arr_map(this->value, (FMAP)exp_to_str), ','));
    case exp_array0: {
      Type *tp = opt_get(this->value);
      return str_f("[]/%s/", tp ? type_to_str(tp) : "?");
    }
    case exp_dictionary: {
      char *fmap (Tp *kv) {
        return str_f("%s:%s", exp_to_str(kv->e1), exp_to_str(kv->e2));
      }
      return str_f("{%s}",
        arr_cjoin(arr_map(this->value, (FMAP)fmap), ','));
      }
    case exp_dictionary0: {
      Type *tp = opt_get(this->value);
      return str_f("{}/%s/", tp ? type_to_str(tp) : "?");
    }
    case exp_tuple:
      return str_f("[.%s]",
        arr_cjoin(arr_map(this->value, (FMAP)exp_to_str), ','));
    case exp_function: {
      Tp3 *tp = this->value;
      return str_f("\\%s%s->%s",
        type_to_str(tp->e1), arr_cjoin(tp->e2, ','), stat_to_str(tp->e3));
    }
    case exp_range:
      return str_f("[!%s]",
        arr_cjoin(arr_map(this->value, (FMAP)exp_to_str), ','));
    case exp_point: {
      Tp *tp = this->value;
      return str_f("%s.%s", tp->e1, tp->e2);
    }
    case exp_square: {
      Tp *tp = this->value;
      return str_f("%s[%s]", exp_to_str(tp->e1), exp_to_str(tp->e2));
    }
    case exp_slice: {
      Tp3 *tp = this->value;
      Exp *ix1 = opt_get(tp->e2);
      Exp *ix2 = opt_get(tp->e3);
      return str_f("%s[%s:%s]",
        exp_to_str(tp->e1),
        ix1 ? exp_to_str(ix1) : "",
        ix2 ? exp_to_str(ix2) : "");
    }
    case exp_parenthesis: {
      Tp *tp = this->value;
      return str_f("%s(%s)",
        exp_to_str(tp->e1),
        arr_cjoin(arr_map(tp->e2, (FMAP)exp_to_str), ','));
    }
    case exp_group:
      return str_f("(%s)", exp_to_str(this->value));
    case exp_unary: {
      Tp *sym_ex = this->value;
      return str_f("%s%s", sym_ex->e1, exp_to_str(sym_ex->e2));
    }
    case exp_binary: {
      Tp3 *tp = this->value;
      return str_f("(%s%s%s)", exp_to_str(tp->e2), tp->e1, exp_to_str(tp->e3));
    }
    case exp_ternary: {
      Tp3 *tp = this->value;
      return str_f("%s?%s:%s",
        exp_to_str(tp->e1), exp_to_str(tp->e2), exp_to_str(tp->e3));
    }
    case exp_switch: {
      Tp *cd_es = this->value;
      char *fmap (Tp *exs_ex) {
        return str_f("%s:%s",
          arr_join(arr_map(exs_ex->e1, (FMAP)exp_to_str),","),
          exp_to_str(exs_ex->e2));
      }
      return str_f("switch(%s){%s}",
        exp_to_str(cd_es->e1),
        arr_cjoin(arr_map(cd_es->e2, (FMAP)fmap), ';'));
    }
    default:
      // exp_error
      // exp_int
      // exp_float
      // exp_symbol
      return this->value;
  }
}
