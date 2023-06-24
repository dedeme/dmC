// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "exp.h"
#include "kut/js.h"
#include "kut/dec.h"
#include "kut/buf.h"
#include "DEFS.h"
#include "function.h"

enum exp_Exp_t {
  EXP_NULL, EXP_CLASS, EXP_NEW, EXP_EVAL,
  EXP_BOOL, EXP_INT, EXP_FLOAT, EXP_STRING,
  EXP_ARR, EXP_MAP, EXP_FUNC, EXP_SYM,
  EXP_RANGE,
  EXP_PT, EXP_SQ, EXP_SLICE, EXP_PR, // Point, Square braket - subindex, Sqare braket - slice, Parentheses
  EXP_SWITCH,
  EXP_NOT, EXP_MINUS,
  EXP_ADD, EXP_SUB, EXP_MUL, EXP_DIV, EXP_MOD,
  EXP_AND, EXP_OR,
  EXP_GREATER, EXP_GREATER_EQ, EXP_LESS, EXP_LESS_EQ, EXP_EQ, EXP_NEQ,
  EXP_TERNARY
};

typedef enum exp_Exp_t Exp_t;

struct exp_Exp {
  Exp_t type;
  union {
    int b;
    int64_t i;
    double d;
    void *value;
  };
  char *js;
};

static char *type_to_str (Exp_t type) {
  switch (type) {
    case EXP_NULL: return "null";
    case EXP_CLASS: return "class";
    case EXP_NEW: return "new";
    case EXP_EVAL: return "eval";
    case EXP_BOOL: return "bool";
    case EXP_INT: return "int";
    case EXP_FLOAT: return "float";
    case EXP_STRING: return "str";
    case EXP_ARR: return "arr";
    case EXP_MAP: return "dic";
    case EXP_FUNC: return "function";
    case EXP_SYM: return "symbol";
    case EXP_RANGE: return "range";
    case EXP_PT: return "point";
    case EXP_SQ: return "square";
    case EXP_SLICE: return "slice";
    case EXP_PR: return "parenthesis";
    case EXP_SWITCH: return "switch";
    case EXP_NOT: return "not";
    case EXP_MINUS: return "minus";
    case EXP_ADD: return "add";
    case EXP_SUB: return "sub";
    case EXP_MUL: return "mul";
    case EXP_DIV: return "div";
    case EXP_MOD: return "mod";
    case EXP_AND: return "and";
    case EXP_OR: return "or";
    case EXP_GREATER: return "greater";
    case EXP_GREATER_EQ: return "greater_eq";
    case EXP_LESS: return "less";
    case EXP_LESS_EQ: return "less_eq";
    case EXP_EQ: return "eq";
    case EXP_NEQ: return "neq";
    case EXP_TERNARY: return "ternary";
  }
  EXC_ILLEGAL_ARGUMENT("Bad expression type identifier",
    str_f("(0 to %d)", EXP_TERNARY), str_f("%d", type)
  );
  return NULL;
}

char *exp_get_js(Exp *this) {
  return this->js;
}

void exp_set_js (Exp *this, char *js) {
  this->js = js;
}

char *exp_js_insert (Exp *this, char *ins) {
  Buf *bf = buf_new();
  char *p = this->js;
  while (*p) {
    char ch = *p++;
    if (ch <= ' ') buf_cadd(bf, ch);
    else {
      --p;
      break;
    }
  }
  return str_f("%s%s%s", buf_str(bf), ins, p);
}

char *exp_js_replace (Exp *this, char *repl) {
  Buf *bf = buf_new();
  char *p = this->js;
  while (*p) {
    char ch = *p++;
    if (ch <= ' ') buf_cadd(bf, ch);
    else break;
  }
  return str_f("%s%s", buf_str(bf), repl);
}

static Exp *newb(Exp_t type, int value, char *js) {
  Exp *this = MALLOC(Exp);
  this->type = type;
  this->b = value;
  this->js = js;
  return this;
}

static Exp *newi(Exp_t type, int64_t value, char *js) {
  Exp *this = MALLOC(Exp);
  this->type = type;
  this->i = value;
  this->js = js;
  return this;
}

static Exp *newd(Exp_t type, double value, char *js) {
  Exp *this = MALLOC(Exp);
  this->type = type;
  this->d = value;
  this->js = js;
  return this;
}

static Exp *new(Exp_t type, void *value, char *js) {
  Exp *this = MALLOC(Exp);
  this->type = type;
  this->value = value;
  this->js = js;
  return this;
}

Exp *exp_null (Exp *value, char *js) {
  return new(EXP_NULL, value, js);
}

Exp *exp_get_null (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_null, "null", this);
  return this->value;
}

int exp_is_null (Exp *this) {
  return this->type == EXP_NULL;
}

Exp *exp_class (char *value, char *js) {
  return new(EXP_CLASS, value, js);
}

char *exp_get_class (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_class, "class", this);
  return this->value;
}

int exp_is_class (Exp *this) {
  return this->type == EXP_CLASS;
}

// value is Arr<Exp>
Exp *exp_new (char *id, Arr *value, char *js) {
  return new(EXP_NEW, tp_new(id, value), js);
}

// Tp<char, Arr<Exp>>
Tp *exp_get_new (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_new, "new", this);
  return this->value;
}

int exp_is_new (Exp *this) {
  return this->type == EXP_NEW;
}

Exp *exp_eval (char *value, char *js) {
  return new(EXP_EVAL, value, js);
}

char *exp_get_eval (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_eval, "eval", this);
  return this->value;
}

int exp_is_eval (Exp *this) {
  return this->type == EXP_EVAL;
}

Exp *exp_bool (int value, char *js) {
  return newb(EXP_BOOL, value, js);
}

int exp_get_bool (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_bool, "bool", this);
  return this->b;
}

int exp_is_bool(Exp *this) {
  return this->type == EXP_BOOL;
}

Exp *exp_int (int64_t value, char *js) {
  return newi(EXP_INT, value, js);
}

int64_t exp_get_int (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_int, "int", this);
  return this->i;
}

int exp_is_int(Exp *this) {
  return this->type == EXP_INT;
}

Exp *exp_float (double value, char *js) {
  return newd(EXP_FLOAT, value, js);
}

double exp_get_float (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_float, "float", this);
  return this->d;
}

int exp_is_float (Exp *this) {
  return this->type == EXP_FLOAT;
}

Exp *exp_string (char *value, char *js) {
  return new(EXP_STRING, value, js);
}

char *exp_get_string (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_string, "string", this);
  return this->value;
}

int exp_is_string (Exp *this) {
  return this->type == EXP_STRING;
}

Exp *exp_array (Arr *value, char *js) {
  return new(EXP_ARR, value, js);
}

// <Exp>
Arr *exp_get_array (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_array, "array", this);
  return this->value;
}

int exp_is_array (Exp *this) {
  return this->type == EXP_ARR;
}

Exp *exp_map (Map *value, char *js) {
  return new(EXP_MAP, value, js);
}

// <Exp>
Map *exp_get_map (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_map, "map", this);
  return this->value;
}

int exp_is_map (Exp *this) {
  return this->type == EXP_MAP;
}

Exp *exp_function (Function *value, char *js) {
  return new(EXP_FUNC, value, js);
}

Function *exp_get_function (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_function, "function", this);
  return this->value;
}

int exp_is_function (Exp *this) {
  return this->type == EXP_FUNC;
}

Exp *exp_sym (char *value, char *js) {
   return new(EXP_SYM, value, js);
}

char *exp_get_sym (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_sym, "symbol", this);
  return this->value;
}

int exp_is_sym (Exp *this) {
  return this->type == EXP_SYM;
}

/// v3 is Opt<Exp>
Exp *exp_range (Exp *v1, Exp *v2, Opt *v3, char *js) {
  return new(EXP_RANGE, tp3_new(v1, v2, v3), js);
}

// <Exp, Exp, Opt<Exp>>
Tp3 *exp_get_range (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_range, "range", this);
  return this->value;
}

int exp_is_range (Exp *this) {
  return this->type == EXP_RANGE;
}

Exp *exp_pt (Exp *v1, Exp *v2, char *js) {
  return new(EXP_PT, tp_new(v1, v2), js);
}

Tp *exp_get_pt (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_pt, "point", this);
  return this->value;
}

int exp_is_pt (Exp *this) {
  return this->type == EXP_PT;
}

Exp *exp_sq (Exp *v1, Exp *v2, char *js) {
  return new(EXP_SQ, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_sq(Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_sq, "square", this);
  return this->value;
}

int exp_is_sq (Exp *this) {
  return this->type == EXP_SQ;
}

// v2 and v3 are Opt<Exp>
Exp *exp_slice (Exp *v1, Opt *v2, Opt *v3, char *js) {
  return new(EXP_SLICE, tp3_new(v1, v2, v3), js);
}

// <Exp, Opt<Exp>, Opt<Exp>>
Tp3 *exp_get_slice(Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_slice, "slice", this);
  return this->value;
}

int exp_is_slice (Exp *this) {
  return this->type == EXP_SLICE;
}

Exp *exp_pr (Exp *value, Arr *arguments, char *js) {
  return new(EXP_PR, tp_new(value, arguments), js);
}

// <Exp, Arr<Exp>>
Tp *exp_get_pr (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_pr, "parenthesis", this);
  return this->value;
}

int exp_is_pr (Exp *this) {
  return this->type == EXP_PR;
}

Exp *exp_switch (Exp *cond, Arr *cases, char *js) {
  return new(EXP_SWITCH, tp_new(cond, cases), js);
}

// <Exp, Arr<Tp<Arr<Exp>, Exp>>>
Tp *exp_get_switch (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_switch, "switch", this);
  return this->value;
}

int exp_is_switch (Exp *this) {
  return this->type == EXP_SWITCH;
}

Exp *exp_not (Exp *value, char *js) {
   return new(EXP_NOT, value, js);
}

Exp *exp_get_not (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_not, "not", this);
  return this->value;
}

int exp_is_not (Exp *this) {
  return this->type == EXP_NOT;
}

Exp *exp_minus (Exp *value, char *js) {
   return new(EXP_MINUS, value, js);
}

Exp *exp_get_minus (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_minus, "minus", this);
  return this->value;
}

int exp_is_minus (Exp *this) {
  return this->type == EXP_MINUS;
}

Exp *exp_add (Exp *v1, Exp *v2, char *js) {
  return new(EXP_ADD, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_add (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_add, "add", this);
  return this->value;
}

int exp_is_add (Exp *this) {
  return this->type == EXP_ADD;
}

Exp *exp_sub (Exp *v1, Exp *v2, char *js) {
  return new(EXP_SUB, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_sub (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_sub, "sub", this);
  return this->value;
}

int exp_is_sub (Exp *this) {
  return this->type == EXP_SUB;
}

Exp *exp_mul (Exp *v1, Exp *v2, char *js) {
  return new(EXP_MUL, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_mul (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_mul, "mul", this);
  return this->value;
}

int exp_is_mul (Exp *this) {
  return this->type == EXP_MUL;
}

Exp *exp_div (Exp *v1, Exp *v2, char *js) {
  return new(EXP_DIV, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_div (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_div, "div", this);
  return this->value;
}

int exp_is_div (Exp *this) {
  return this->type == EXP_DIV;
}

Exp *exp_mod (Exp *v1, Exp *v2, char *js) {
  return new(EXP_MOD, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_mod (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_mod, "mod", this);
  return this->value;
}

int exp_is_mod (Exp *this) {
  return this->type == EXP_MOD;
}

Exp *exp_and (Exp *v1, Exp *v2, char *js) {
  return new(EXP_AND, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_and (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_and, "and", this);
  return this->value;
}

int exp_is_and (Exp *this) {
  return this->type == EXP_AND;
}

Exp *exp_or (Exp *v1, Exp *v2, char *js) {
  return new(EXP_OR, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_or (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_or, "or", this);
  return this->value;
}

int exp_is_or (Exp *this) {
  return this->type == EXP_OR;
}

Exp *exp_greater (Exp *v1, Exp *v2, char *js) {
  return new(EXP_GREATER, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_greater (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_greater, "greater", this);
  return this->value;
}

int exp_is_greater (Exp *this) {
  return this->type == EXP_GREATER;
}

Exp *exp_greater_eq (Exp *v1, Exp *v2, char *js) {
  return new(EXP_GREATER_EQ, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_greater_eq (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_greater_eq, "greater_eq", this);
  return this->value;
}

int exp_is_greater_eq (Exp *this) {
  return this->type == EXP_GREATER_EQ;
}

Exp *exp_less (Exp *v1, Exp *v2, char *js) {
  return new(EXP_LESS, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_less (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_less, "less", this);
  return this->value;
}

int exp_is_less (Exp *this) {
  return this->type == EXP_LESS;
}

Exp *exp_less_eq (Exp *v1, Exp *v2, char *js) {
  return new(EXP_LESS_EQ, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_less_eq (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_less_eq, "less_eq", this);
  return this->value;
}

int exp_is_less_eq (Exp *this) {
  return this->type == EXP_LESS_EQ;
}

Exp *exp_eq (Exp *v1, Exp *v2, char *js) {
  return new(EXP_EQ, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_eq (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_eq, "eq", this);
  return this->value;
}

int exp_is_eq (Exp *this) {
  return this->type == EXP_EQ;
}

Exp *exp_neq (Exp *v1, Exp *v2, char *js) {
  return new(EXP_NEQ, tp_new(v1, v2), js);
}

// <Exp, Exp>
Tp *exp_get_neq (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_neq, "neq", this);
  return this->value;
}

int exp_is_neq (Exp *this) {
  return this->type == EXP_NEQ;
}

Exp *exp_ternary (Exp *v1, Exp *v2, Exp *v3, char *js) {
  return new(EXP_TERNARY, tp3_new(v1, v2, v3), js);
}

// <Exp, Exp, Exp>
Tp3 *exp_get_ternary (Exp *this) {
  TEST_EXP_TYPE_ERROR(exp_is_ternary, "ternary", this);
  return this->value;
}

int exp_is_ternary (Exp *this) {
  return this->type == EXP_TERNARY;
}



// UTILITIES


int exp_is_binary (Exp *this) {
  switch (this->type) {
    case EXP_ADD:
    case EXP_SUB:
    case EXP_MUL:
    case EXP_DIV:
    case EXP_MOD:
    case EXP_AND:
    case EXP_OR:
    case EXP_GREATER:
    case EXP_GREATER_EQ:
    case EXP_LESS:
    case EXP_LESS_EQ:
    case EXP_EQ:
    case EXP_NEQ:
      return TRUE;
    default:
      return FALSE;
  }
}

char *exp_type_to_str (Exp *this) {
  return type_to_str(this->type);
}

char *exp_to_str (Exp *this) {
    //--
    // kv is Kv<Exp>
    char *fn_map(Kv *kv) {
      return str_f("\"%s\": %s", kv_key(kv), exp_to_str(kv_value(kv)));
    }
    // tp is Tp<Arr<Exp>, Exp>
    char *fn_switch(Tp *tp) {
      // Exp
      Arr *conds = tp_e1(tp);
        //--
        char *fn_map(Exp *exp) { return exp_to_str(exp); }
      char *sconds = arr_join(arr_map(conds, (FMAP)fn_map), ", ");
      return str_f("%s: %s;", sconds, exp_to_str(tp_e2(tp)));
    }

  switch (this->type) {
    case EXP_NULL:
      return str_f("null(%s)", exp_to_str(this->value));
    case EXP_CLASS:
      return str_f("class(%s)", this->value);
    case EXP_NEW: {
      // <char, Arr<Exp>>
      Tp *tp = this->value;
        //--
        char *fn (Exp *exp) { return exp_to_str(exp); }
      char *params = arr_join(arr_map(tp_e2(tp), (FMAP)fn), ",");
      return str_f("new %s(%s)", tp_e1(tp), params);
    }
    case EXP_EVAL:
      return str_f("eval(%s)", this->value);
    case EXP_STRING:
      return js_ws(this->value);
    case EXP_INT:
      return dec_itos(this->i);
    case EXP_BOOL:
      return this->b ? "true" : "false";
    case EXP_FLOAT:
      return dec_ftos(this->d, 9);
    case EXP_ARR:
      return str_f("[%s]", arr_join(arr_map(this->value, (FMAP)exp_to_str), ", "));
    case EXP_MAP:
      return str_f(
        "{%s}",
        arr_join(arr_map(map_to_array(this->value), (FMAP)fn_map), ", ")
      );
    case EXP_FUNC:
      return function_to_str(this->value);
    case EXP_SYM :
      return this->value;
    case EXP_RANGE: {
      // <Exp, Exp, Opt<Exp>>
      Tp3 *tp = exp_get_range(this);
      Exp *e3 = opt_get(tp3_e3(tp));
      return e3
        ? str_f("[%s:%s:%s]",
            exp_to_str(tp3_e1(tp)), exp_to_str(tp3_e2(tp)), exp_to_str(e3)
          )
        : str_f("[%s:%s]", exp_to_str(tp3_e1(tp)), exp_to_str(tp3_e2(tp)))
      ;
    }
    case EXP_PT: {
      // <Exp, Exp>
      Tp *tp = exp_get_pt(this);
      return str_f("%s.%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_SQ: {
      // <Exp, Exp>
      Tp *tp = exp_get_sq(this);
      return str_f("%s[%s]", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_SLICE: {
      // <Exp, Opt<Exp>, Opt<Exp>>
      Tp3 *tp =this->value;
      Exp *e2 = opt_get(tp3_e2(tp));
      Exp *e3 = opt_get(tp3_e3(tp));
      return str_f(
        "%s[%s:%s]",
        exp_to_str(tp3_e1(tp)),
        e2 ? exp_to_str(tp3_e2(tp)): "",
        e3 ? exp_to_str(tp3_e3(tp)): ""
      );
    }
    case EXP_PR: {
      // <Exp, Arr<Exp>>
      Tp *tp = exp_get_pr(this);
      return str_f(
        "%s(%s)",
        exp_to_str(tp_e1(tp)),
        arr_join(arr_map(tp_e2(tp), (FMAP)exp_to_str), ",")
      );
    }
    case EXP_SWITCH: {
      // <Exp, Arr<Tp<Arr<Exp>, Exp>>>
      Tp *tp = exp_get_switch(this);
      return str_f(
        "switch(%s){%s}",
        exp_to_str(tp_e1(tp)),
        arr_join(arr_map(tp_e2(tp), (FMAP)fn_switch), "")
      );
    }
    case EXP_NOT:
      return str_f("!%s", exp_to_str(exp_get_not(this)));
    case EXP_MINUS:
      return str_f("-%s", exp_to_str(exp_get_minus(this)));
    case EXP_ADD: {
      // <Exp, Exp>
      Tp *tp = exp_get_add(this);
      return str_f("%s+%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_SUB: {
      // <Exp, Exp>
      Tp *tp = exp_get_sub(this);
      return str_f("%s-%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_MUL: {
      // <Exp, Exp>
      Tp *tp = exp_get_mul(this);
      return str_f("%s*%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_DIV: {
      // <Exp, Exp>
      Tp *tp = exp_get_div(this);
      return str_f("%s/%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_MOD: {
      // <Exp, Exp>
      Tp *tp = exp_get_mod(this);
      return str_f("%s%%%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_AND: {
      // <Exp, Exp>
      Tp *tp = exp_get_and(this);
      return str_f("%s&%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_OR: {
      // <Exp, Exp>
      Tp *tp = exp_get_or(this);
      return str_f("%s|%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_GREATER: {
      // <Exp, Exp>
      Tp *tp = exp_get_greater(this);
      return str_f("%s>%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_GREATER_EQ: {
      // <Exp, Exp>
      Tp *tp = exp_get_greater_eq(this);
      return str_f("%s>=%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_LESS: {
      // <Exp, Exp>
      Tp *tp = exp_get_less(this);
      return str_f("%s<%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_LESS_EQ: {
      // <Exp, Exp>
      Tp *tp = exp_get_less_eq(this);
      return str_f("%s<=%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_EQ: {
      // <Exp, Exp>
      Tp *tp = exp_get_eq(this);
      return str_f("%s==%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_NEQ: {
      // <Exp, Exp>
      Tp *tp = exp_get_neq(this);
      return str_f("%s!=%s", exp_to_str(tp_e1(tp)), exp_to_str(tp_e2(tp)));
    }
    case EXP_TERNARY: {
      // <Exp, Exp, Exp>
      Tp3 *tp = exp_get_ternary(this);
      return str_f("%s?%s:%s",
        exp_to_str(tp3_e1(tp)), exp_to_str(tp3_e2(tp)), exp_to_str(tp3_e3(tp))
      );
    }
  }
  EXC_ILLEGAL_ARGUMENT("Bad expression type identifier",
    str_f("(0 to %d)", EXP_TERNARY), str_f("%d", this->type)
  );
  return NULL;
}
