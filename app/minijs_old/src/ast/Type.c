// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include <string.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Ichar.h"
#include "ast/Type.h"
#include "ct/Ltype.h"
#include "ct/Itype.h"
#include "ct/Asym.h"

/*.
struct: @Type
  t: enum Type_t: _uint
  id: char *: _string
  params: Atype *: __ltype - type
*/

/*.-.*/
struct type_Type {
  enum Type_t t;
  char *id;
  Ltype *params;
};

Type *_type_new(enum Type_t t, char *id, Ltype *params) {
  Type *this = MALLOC(Type);
  this->t = t;
  XNULL(id)
  this->id = id;
  XNULL(params)
  this->params = params;
  return this;
}

enum Type_t type_t(Type *this) {
  XNULL(this)
  return this->t;
}

char *type_id(Type *this) {
  XNULL(this)
  return this->id;
}

Ltype *type_params(Type *this) {
  XNULL(this)
  return this->params;
}

Ajson *type_to_json(Type *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_auint(serial, this->t);
  jarr_astring(serial, this->id);
  ajson_add(serial, json_warray(ltype_to_json(this->params, type_to_json)));
  return serial;
}

Type *type_from_json(Ajson *serial) {
  XNULL(serial)
  Type *this = MALLOC(Type);
  size_t i = 0;
  this->t = jarr_guint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->params = ltype_from_json(json_rarray(ajson_get(serial, i++)), type_from_json);
  return this;
}
/*.-.*/

inline
Type *type_new_any(void) {
  return _type_new(type_ANY, "", ltype_new());
}

inline
Type *type_new_void(void) {
  return _type_new(type_VOID, "", ltype_new());
}

inline
Type *type_new_unknown(void) {
 return _type_new(type_UNKNOWN, "", ltype_new());
}

Type *type_new_fn(Ltype *params) {
  XNULL(params)
  if (ltype_empty(params))
    THROW(exc_illegal_state_t) exc_illegal_state("'params' is empty") _THROW
  return _type_new(type_FN, "", params);
}

Type *type_new_data(char *id, Ltype *generics) {
  XNULL(id)
  XNULL(generics)
  return _type_new(type_DATA, id, generics);
}

bool type_eq(Type *t1, Type *t2) {
  XNULL(t1)
  XNULL(t2)
  if (t1->t != t2->t) return false;
  if (strcmp(t1->id, t2->id)) return false;
  /**/bool eq(Type *e1, Type *e2) { return type_eq(e1, e2); }
  return itype_eqf(ltype_to_it(t1->params), ltype_to_it(t2->params), eq);
}

static char *data_to_str(char *r, Ltype *gs) {
  if (ltype_empty(gs)) return r;
  char *head = type_to_str(ltype_head(gs));
  Ltype *tail = ltype_tail(gs);
  if (*r) return data_to_str(str_printf("%s,%s", r, head), tail);
  return data_to_str(head, tail);
}

static char *fn_to_str(char *r, char *last, Ltype *ps) {
  if (ltype_empty(ps)) {
    if (*last) {
      if (*r) return str_printf("%s:%s", r, last);
      return last;
    }
    THROW(exc_illegal_state_t) exc_illegal_state("'ps' is empty") _THROW
    return NULL;
  }
  char *head = type_to_str(ltype_head(ps));
  Ltype *tail = ltype_tail(ps);
  if (*last) {
    if (*r) return fn_to_str(str_printf("%s,%s", r, last), head, tail);
    return fn_to_str(last, head, tail);
  }
  return fn_to_str("", head, tail);
}

char *type_test_symbols(Type *this, Asym *symbols) {
  /**/char *test_list(Ltype *ps) {
  /**/  char *r;
  /**/  EACHL(ps, Type, t) {
  /**/    r = type_test_symbols(t, symbols);
  /**/    if (*r) {
  /**/      return r;
  /**/    }
  /**/  }_EACH
  /**/  return "";
  /**/}
  switch (type_t(this)) {
  case type_DATA:
    return ichar_contains((Ichar *)asym_to_it(symbols), type_id(this))
      ? test_list(type_params(this))
      : type_id(this);
  case type_FN:
    return test_list(type_params(this));
  default:
    return "";
  }
}

char *type_to_str(Type *this) {
  switch (this->t) {
  case type_DATA:
    return
      str_eq(this->id, "Arr")
      ? str_printf("[%s]", data_to_str("", this->params))
      : str_eq(this->id, "Map")
        ? str_printf("{%s}", data_to_str("", this->params))
        : str_printf("%s<%s>", this->id, data_to_str("", this->params));
  case type_FN:
    return str_printf("(%s)", fn_to_str("", "", this->params));
  case type_ANY:
    return "*";
  case type_VOID:
    return "";
  case type_UNKNOWN:
    return "?";
  }

  THROW(exc_illegal_state_t) exc_illegal_state("Exhausted switch") _THROW
  return NULL;
}
