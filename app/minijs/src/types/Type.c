// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/Type.h"
#include "lexer/token.h"
#include "ast/Class.h"
#include "Cpath.h"
#include "global.h"

/*.
struct: @Type
  type: enum Type_t: _uint
  id:char *: _string
  params: Arr *: _array type
*/

static Type *t_unknown = NULL;
static Type *t_bool = NULL;
static Type *t_byte = NULL;
static Type *t_int = NULL;
static Type *t_float = NULL;
static Type *t_char = NULL;
static Type *t_str = NULL;
static Type *t_any = NULL;

/*.-.*/
struct type_Type {
  enum Type_t type;
  char *id;
  Arr *params;
};

Type *_type_new(enum Type_t type, char *id, Arr *params) {
  Type *this = MALLOC(Type);
  this->type = type;
  this->id = id;
  this->params = params;
  return this;
}

inline
enum Type_t type_type(Type *this) {
  return this->type;
}

inline
char *type_id(Type *this) {
  return this->id;
}

inline
Arr *type_params(Type *this) {
  return this->params;
}

Json *type_serialize(Type *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->type);
  jarr_astring(serial, this->id);
  jarr_aarray(serial, this->params, (Json*(*)(void*)) type_serialize);
  return json_warray(serial);
}

Type *type_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Type *this = MALLOC(Type);
  size_t i = 0;
  this->type = jarr_guint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->params = jarr_garray(serial, i++, (void*(*)(Json*)) type_restore);
  return this;
}
/*.-.*/

inline
Type *type_new_unknown() {
  return t_unknown
    ? t_unknown : (t_unknown = _type_new(UNKNOWN, "", arr_new()));
}

inline
Type *type_new_bool() {
  return t_bool ? t_bool : (t_bool = _type_new(DATA, "Bool", arr_new()));
}

inline
Type *type_new_byte() {
  return t_byte ? t_byte : (t_bool = _type_new(DATA, "Byte", arr_new()));
}

inline
Type *type_new_int() {
  return t_int ? t_int : (t_int = _type_new(DATA, "Int", arr_new()));
}

inline
Type *type_new_float() {
  return t_float ? t_float : (t_float = _type_new(DATA, "Float", arr_new()));
}

inline
Type *type_new_char() {
  return t_char ? t_char : (t_char = _type_new(DATA, "Char", arr_new()));
}

inline
Type *type_new_str() {
  return t_str ? t_str : (t_str = _type_new(DATA, "Str", arr_new()));
}

inline
Type *type_new_any() {
  return t_any ? t_any : (t_any = _type_new(ANY, "", arr_new()));
}

Type *type_new_arr(Type *t) {
  Arr/*Type*/ *a = arr_new();
  arr_add(a, t);
  return _type_new(ARR, "", a);
}

Type *type_new_map(Type *t) {
  Arr/*Type*/ *a = arr_new();
  arr_add(a, t);
  return _type_new(MAP, "", a);
}

inline
Type *type_new_fn(Arr/*Type*/ *ts) {
  return _type_new(FN, "", ts);
}

Type *type_new_data(char *id, Arr/*Type*/ *generics) {
  return _type_new(DATA, id, generics);
}

bool type_eq(Type *this, Type *other) {
  if (!this) {
    return !other;
  }
  if (!other) {
    return false;
  }
  if (this->type != other->type) {
    return false;
  }
  if (strcmp(this->id, other->id)) {
    return false;
  }

  if (arr_size(this->params) != arr_size(other->params)) {
    return false;
  }

  RANGE0(i, arr_size(this->params)) {
    if (!type_eq(arr_nget(this->params, i), arr_nget(other->params, i))) {
      return false;
    }
  }_RANGE

  return true;
}

bool type_child(Type *this, Type *child) {
  if (!this) {
    return !child;
  }

  if (!child) {
    return false;
  }

  if (this->type == ANY) {
    return true;
  }

  if (type_eq(this, child)) {
    return true;
  }

  if (
    this->type == FN &&
    this->type == FN &&
    arr_size(this->params) == arr_size(child->params)
  ) {
    RANGE0(i, arr_size(this->params)) {
      void *pthis = arr_nget(this->params, i);
      void *pchild = arr_nget(child->params, i);
      if (!type_child(pthis, pchild)) {
        return false;
      }
    }_RANGE
    return true;
  }

  if (
    this->type == DATA &&
    this->type == DATA
  ) {
    if (str_index(
      " Bool Byte Int Float Char Str ", str_printf(" %s ", this->id)
      ) == -1
    ) {
      Cpath *tpath = cpath_new(this->id);
      Cpath *cpath = cpath_new(child->id);

      Class *tclass = NULL;
      EACH(global_classes(), Class, cl) {
        if (cpath_eq(class_cpath(cl), tpath)) {
          tclass = cl;
          break;
        }
      }_EACH
      if (!tclass) {
        return false;
      }

      Class *cclass = NULL;
      EACH(global_classes(), Class, cl) {
        if (cpath_eq(class_cpath(cl), cpath)) {
          cclass = cl;
          break;
        }
      }_EACH
      if (!cclass) {
        return false;
      }

      Arr/*Cvalue*/ *cvalues = class_cvalues(cclass);
      EACH(class_cvalues(tclass), Cvalue, tv) {
        bool ok = false;
        EACH(cvalues, Cvalue, cv) {
          if (!strcmp(cvalue_id(tv), cvalue_id(cv))) {
            ok = type_eq(
              dvalue_type(cvalue_dvalue(tv)),
              dvalue_type(cvalue_dvalue(cv))
            );
            break;
          }
        }_EACH
        if (!ok) {
          return false;
        }
      }_EACH
      return true;
    }
  }

  return false;
}

bool type_is_unknown(Type *this) {
  if (!this) {
    return false;
  }
  if (this->type == UNKNOWN) {
    return true;
  }

  EACH(this->params, Type, t) {
    if (t && t->type == UNKNOWN) {
      return true;
    }
  }_EACH

  return false;
}

char *type_to_str(Type *this) {
  if (!this) {
    return "NULL";
  }
  switch (this->type) {
  case DATA:
    return str_printf("%s%s",
      this->id,
      arr_size(this->params) > 0
        ? str_printf("<%s>", str_cjoin(
            it_map(it_from(this->params), (void*(*)(void *))type_to_str),
            ','
          ))
        : ""
    );
  case ARR:
    return str_printf("[%s]", type_to_str(arr_get(this->params, 0)));
  case MAP:
    return str_printf("{%s}", type_to_str(arr_get(this->params, 0)));
  case FN:
    return str_printf("%s%s)",
      str_printf("(%s:",
        str_cjoin(
          it_map(
            it_take(it_from(this->params), arr_size(this->params) - 1),
            (void*(*)(void *)) type_to_str
          ),
        ',')
      ),
      arr_nget(this->params, arr_size(this->params) - 1)
        ? type_to_str(arr_get(this->params, arr_size(this->params) - 1))
        : ""
    );
  case ANY:
    return "*";
  case UNKNOWN:
    return "?";
  default:
    THROW "Type '%d' is unknown", this->type _THROW
  }
  return NULL;
}
