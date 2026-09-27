// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/DEFS.h"
#include "kut/math.h"
#include "kut/js.h"
#include "exp.h"

enum exp_Exp_t {
  EXP_BOOL, EXP_INT, EXP_FLOAT, EXP_STRING, EXP_OBJECT,
  EXP_ARR, EXP_DIC
};

typedef enum exp_Exp_t Exp_t;

struct exp_Exp {
  union {
    int b;
    int64_t i;
    double d;
    void *value;
  };
  Exp_t type;
};

static Exp empty_exp = { .type = EXP_OBJECT, .value = "<empty expression>" };

static char *type_to_str (Exp_t type) {
  switch (type) {
    case EXP_BOOL: return "bool";
    case EXP_INT: return "int";
    case EXP_FLOAT: return "float";
    case EXP_STRING: return "str";
    case EXP_OBJECT: return "object";
    case EXP_ARR: return "arr";
    case EXP_DIC: return "dic";
  }
  EXC_ILLEGAL_ARGUMENT("Bad expression type identifier",
    str_f("(0 to %d)", EXP_DIC), str_f("%d", type)
  );
  return NULL;
}
static Exp *newb(Exp_t type, int value) {
  Exp *this = MALLOC(Exp);
  this->b = value;
  this->type = type;
  return this;
}

static Exp *newi(Exp_t type, int64_t value) {
  Exp *this = MALLOC(Exp);
  this->i = value;
  this->type = type;
  return this;
}

static Exp *newd(Exp_t type, double value) {
  Exp *this = MALLOC(Exp);
  this->d = value;
  this->type = type;
  return this;
}

static Exp *new(Exp_t type, void *value) {
  Exp *this = MALLOC(Exp);
  this->value = value;
  this->type = type;
  return this;
}

static char *fail_type (char *expected, Exp *exp) {
  char *sexp = exp_to_js(exp);
  // char
  Arr *runes = str_runes(sexp);
  int size = arr_size(runes);
  if (size > 160)
    sexp = str_f("...%s", arr_join(arr_drop(runes, size - 157), ""));

  return str_f(
    "Type error:\n    Expected: %s\n    Found   : %s (%s)",
    expected,
    exp_type_to_str(exp),
    sexp
  );
}

Exp *exp_empty (void) {
  return &empty_exp;
}

int exp_is_empty (Exp *this) {
  return this == &empty_exp;
}

Exp *exp_bool (int value) {
  return newb(EXP_BOOL, value);
}

int exp_get_bool (Exp *this) {
  if (this->type == EXP_BOOL) return this->b;
  EXC_KUT(fail_type("bool", this));
  return 0; // Unreachable.
}

int exp_is_bool(Exp *this) {
  return this->type == EXP_BOOL;
}

int exp_get_as_bool (Exp *this) {
  if (this->type == EXP_BOOL) return this->b ? TRUE : FALSE;
  if (this->type == EXP_ARR) return arr_size(this->value) ? TRUE : FALSE;
  EXC_KUT(fail_type("bool or array", this));
  return 0; // Unreachable
}

Exp *exp_int (int64_t value) {
  return newi(EXP_INT, value);
}

int64_t exp_get_int (Exp *this) {
  if (this->type == EXP_INT) return this->i;
  EXC_KUT(fail_type("int", this));
  return 0; // Unreachable.
}

int exp_is_int(Exp *this) {
  return this->type == EXP_INT;
}

Exp *exp_float (double value) {
  return newd(EXP_FLOAT, value);
}

double exp_get_float (Exp *this) {
  if (this->type == EXP_FLOAT) return this->d;
  EXC_KUT(fail_type("float", this));
  return 0.0; // Unreachable.
}

int exp_is_float (Exp *this) {
  return this->type == EXP_FLOAT;
}

Exp *exp_string (char *value) {
  return new(EXP_STRING, value);
}

char *exp_get_string (Exp *this) {
  if (this->type == EXP_STRING) return this->value;
  EXC_KUT(fail_type("string", this));
  return NULL; // Unreachable.
}

int exp_is_string (Exp *this) {
  return this->type == EXP_STRING;
}

Exp *exp_object (char *type, void *value) {
  return new(EXP_OBJECT, tp_new(type, value));
}

void *exp_get_object (char *type, Exp *this) {
  if (!exp_is_object(type, this))
    EXC_ILLEGAL_ARGUMENT(
      "Bad expression type",
      str_f("%s<%s>", "object", type),
      exp_type_to_str(this)
    );
  return tp_e2(this->value);
}

int exp_is_object (char *type, Exp *this) {
  return !exp_is_empty(this) &&
    this->type == EXP_OBJECT && str_eq((char *)tp_e1(this->value), type)
  ;
}

int exp_is_some_object (Exp *this) {
  return this->type == EXP_OBJECT;
}

Tp *exp_get_object_tuple (Exp *this) {
  if (this->type == EXP_OBJECT) return this->value;
  EXC_KUT(fail_type("object", this));
  return NULL; // Unreachable.
}

Exp *exp_array (Arr *value) {
  return new(EXP_ARR, value);
}

// <Exp>
Arr *exp_get_array (Exp *this) {
  if (this->type == EXP_ARR) return this->value;
  EXC_KUT(fail_type("array", this));
  return NULL; // Unreachable.
}

int exp_is_array (Exp *this) {
  return this->type == EXP_ARR;
}

Exp *exp_dic (Map *value) {
  return new(EXP_DIC, value);
}

// <Exp>
Map *exp_get_dic (Exp *this) {
  if (this->type == EXP_DIC) return this->value;
  EXC_KUT(fail_type("dictionary", this));
  return NULL; // Unreachable.
}

int exp_is_dic (Exp *this) {
  return this->type == EXP_DIC;
}

char *exp_type_to_str (Exp *this) {
  if (exp_is_empty(this))
    return this->value;

  if (this->type == EXP_OBJECT)
    return str_f("%s<%s>", type_to_str(this->type), (char *)tp_e1(this->value));

  return type_to_str(this->type);
}

char *exp_to_str (Exp *this) {
    //--
    // kv is Kv<Exp>
    char *fn_map(Kv *kv) {
      return str_f("\"%s\": %s", kv_key(kv), exp_to_js(kv_value(kv)));
    }

  if (exp_is_empty(this))
    return this->value;

  switch (this->type) {
    case EXP_STRING:
      return this->value;
    case EXP_INT:
      return math_itos(this->i);
    case EXP_BOOL:
      return this->b ? "true" : "false";
    case EXP_FLOAT:
      return math_ftos(this->d, 9);
    case EXP_OBJECT:
      return str_f("%s:%ld", exp_type_to_str(this), (long)tp_e2(this->value));
    case EXP_ARR:
      return str_f("[%s]", arr_join(arr_map(this->value, (FMAP)exp_to_js), ", "));
    case EXP_DIC:
      return str_f(
        "{%s}",
        arr_join(arr_map(map_to_array(this->value), (FMAP)fn_map), ", ")
      );
  }
  EXC_ILLEGAL_ARGUMENT("Bad expression type identifier",
    str_f("(0 to %d)", EXP_DIC), str_f("%d", this->type)
  );
  return NULL; // Unreachable
}

char *exp_to_js (Exp *this) {
    //--
    char *fmtf(char *n) { return math_digits(n) ? str_f("%s.0", n) : n; }
  return exp_is_string(this)
    ? js_ws(this->value)
    : exp_is_float(this)
      ? fmtf(math_ftos(this->d, 9))
      : exp_to_str(this);
}

