// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Type.h"
#include "ast/Atype.h"
#include "ast/Imported.h"

/*.
struct: @Type
  type: enum Type_t: _uint
  id:char *: _string
  params: Atype *: atype
*/

static Type *t_unknown = NULL;
static Type *t_bool = NULL;
static Type *t_byte = NULL;
static Type *t_int = NULL;
static Type *t_float = NULL;
static Type *t_char = NULL;
static Type *t_str = NULL;
static Type *t_any = NULL;
static Type *t_void = NULL;

/*.-.*/
struct type_Type {
  enum Type_t type;
  char *id;
  Atype *params;
};

Type *_type_new(enum Type_t type, char *id, Atype *params) {
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
Atype *type_params(Type *this) {
  return this->params;
}

Json *type_serialize(Type *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->type);
  jarr_astring(serial, this->id);
  arr_add(serial, atype_serialize(this->params));
  return json_warray(serial);
}

Type *type_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Type *this = MALLOC(Type);
  size_t i = 0;
  this->type = jarr_guint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->params = atype_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Type *type_new_unknown() {
  return t_unknown
    ? t_unknown : (t_unknown = _type_new(UNKNOWN, "", atype_new()));
}

inline
Type *type_new_bool() {
  return t_bool ? t_bool : (t_bool = _type_new(DATA, "Bool", atype_new()));
}

inline
Type *type_new_byte() {
  return t_byte ? t_byte : (t_byte = _type_new(DATA, "Byte", atype_new()));
}

inline
Type *type_new_int() {
  return t_int ? t_int : (t_int = _type_new(DATA, "Int", atype_new()));
}

inline
Type *type_new_float() {
  return t_float ? t_float : (t_float = _type_new(DATA, "Float", atype_new()));
}

inline
Type *type_new_char() {
  return t_char ? t_char : (t_char = _type_new(DATA, "Char", atype_new()));
}

inline
Type *type_new_str() {
  return t_str ? t_str : (t_str = _type_new(DATA, "Str", atype_new()));
}

inline
Type *type_new_any() {
  return t_any ? t_any : (t_any = _type_new(ANY, "", atype_new()));
}

inline
Type *type_new_void() {
  return t_void ? t_void : (t_void = _type_new(VOID, "", atype_new()));
}

Type *type_new_arr(Type *t) {
  Atype *a = atype_new();
  atype_add(a, t);
  return _type_new(DATA, "Arr", a);
}

Type *type_new_map(Type *t) {
  Atype *a = atype_new();
  atype_add(a, t);
  return _type_new(DATA, "Map", a);
}

Type *type_new_fn(Atype *ts) {
  if (!arr_size(ts))
    THROW exc_illegal_argument(
      "ts", "To have al least one element", "Empty array"
    ) _THROW
  return _type_new(FN, "", ts);
}

Type *type_new_data(char *id, Atype *generics) {
  return _type_new(DATA, id, generics);
}

bool type_eq(Type *this, Type *other) {
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
    if (!type_eq(atype_get(this->params, i), atype_get(other->params, i))) {
      return false;
    }
  }_RANGE

  return true;
}

/// If type is a not standard DATA must use its complete path_id
bool type_child(Type *this, Type *child) {
  if (type_eq(this, child)) {
    return true;
  }
  enum Type_t tthis = this->type;
  enum Type_t tchild = child->type;

  if (
    tthis == UNKNOWN || tchild == UNKNOWN || tthis == VOID || tchild == VOID ||
    tchild == ANY
  ) {
    return false;
  }

  if (tthis == ANY) {
    return true;
  }

  if (arr_size(this->params) != arr_size(child->params)) {
    return false;
  }

  if (tthis == FN) {
    RANGE0(i, arr_size(this->params)) {
      if (
        !type_child(atype_get(this->params, i), atype_get(child->params, i))
      ) {
        return false;
      }
    }_RANGE
    return true;
  }

  RANGE0(i, arr_size(this->params)) {
    if (
      !type_eq(atype_get(this->params, i), atype_get(child->params, i))
    ) {
      return false;
    }
  }_RANGE

  Imported *im = imported_get();
  Class *cchild = imported__class(im, child->id);
  if (!cchild)
    THROW "Class '%s' does not exist", child->id _THROW

  Achar *super = class_super(cchild);
  if (!arr_size(super)) {
    return false;
  }
  Class *csuper = imported__class(im, achar_get(super, 0));
  if (!csuper)
    THROW "Class '%s' does not exist", achar_get(super, 0) _THROW
  Type *tsuper = class__type(csuper, this->params);
  if (!tsuper) {
    return false;
  }

  return type_child(this, tsuper);
}

///
bool type_is_unknown(Type *this) {
  if (this->type == UNKNOWN) {
    return true;
  }

  EACH(this->params, Type, t) {
    if (t->type == UNKNOWN) {
      return true;
    }
  }_EACH

  return false;
}

///
char *type_to_str(Type *this) {
  switch (this->type) {
  case DATA:
    return
      !strcmp(this->id, "Arr")
      ? str_printf("[%s]", type_to_str(atype_get(this->params, 0)))
      : !strcmp(this->id, "Map")
        ? str_printf("{%s}", type_to_str(atype_get(this->params, 0)))
        : str_printf("%s%s",
          this->id,
          arr_size(this->params) > 0
            ? str_printf("<%s>", str_cjoin(
                it_map(it_from(this->params), (void*(*)(void *))type_to_str),
                ','
              ))
            : ""
        );
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
      type_to_str(arr_get(this->params, arr_size(this->params) - 1))
    );
  case ANY:
    return "*";
  case VOID:
    return "";
  case UNKNOWN:
    return "?";
  }

  THROW exc_illegal_state("Exhausted switch") _THROW
  return NULL;
}
