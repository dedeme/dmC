// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Class.h"
#include "DEFS.h"
#include "lexer/Tx.h"
#include "ast/Program.h"
#include "lexer/rclass.h"
#include "parser/pclass.h"
#include "ast/Mtype.h"

/*.
struct: @Class
  id: char *: _string
  +local: bool: _bool
  generics: Achar *: achar
  imports: Mchar *: mchar
  +super: Type *: type
  implements: Atype *: atype
  statics: Aatt *: aatt
  instance: Aatt *: aatt
  -
  used : Achar * = achar_new()
*/

/*.-.*/
struct class_Class {
  char *id;
  bool local;
  Achar *generics;
  Mchar *imports;
  Type *super;
  Atype *implements;
  Aatt *statics;
  Aatt *instance;

  Achar *used;
};

Class *_class_new(
  char *id,
  bool local,
  Achar *generics,
  Mchar *imports,
  Type *super,
  Atype *implements,
  Aatt *statics,
  Aatt *instance
) {
  Class *this = MALLOC(Class);
  this->id = id;
  this->local = local;
  this->generics = generics;
  this->imports = imports;
  this->super = super;
  this->implements = implements;
  this->statics = statics;
  this->instance = instance;
  this->used = achar_new();
  return this;
}

inline
char *class_id(Class *this) {
  return this->id;
}

inline
bool class_local(Class *this) {
  return this->local;
}

inline
void class_set_local(Class *this, bool value) {
  this->local = value;
}

inline
Achar *class_generics(Class *this) {
  return this->generics;
}

inline
Mchar *class_imports(Class *this) {
  return this->imports;
}

inline
Type *class_super(Class *this) {
  return this->super;
}

inline
void class_set_super(Class *this, Type *value) {
  this->super = value;
}

inline
Atype *class_implements(Class *this) {
  return this->implements;
}

inline
Aatt *class_statics(Class *this) {
  return this->statics;
}

inline
Aatt *class_instance(Class *this) {
  return this->instance;
}

inline
Achar *class_used(Class *this) {
  return this->used;
}

Arr/*Json*/ *class_serialize(Class *this) {
  Arr/*Json*/ *serial = arr_new();
  if (!this) return serial;
  jarr_astring(serial, this->id);
  jarr_abool(serial, this->local);
  arr_add(serial, json_warray(achar_serialize(this->generics)));
  arr_add(serial, json_warray(mchar_serialize(this->imports)));
  arr_add(serial, json_warray(type_serialize(this->super)));
  arr_add(serial, json_warray(atype_serialize(this->implements)));
  arr_add(serial, json_warray(aatt_serialize(this->statics)));
  arr_add(serial, json_warray(aatt_serialize(this->instance)));
  return serial;
}

Class *class_restore(Arr/*Json*/ *serial) {
  if (!arr_size(serial)) return NULL;
  Class *this = MALLOC(Class);
  size_t i = 0;
  this->id = jarr_gstring(serial, i++);
  this->local = jarr_gbool(serial, i++);
  this->generics = achar_restore(json_rarray(arr_get(serial, i++)));
  this->imports = mchar_restore(json_rarray(arr_get(serial, i++)));
  this->super = type_restore(json_rarray(arr_get(serial, i++)));
  this->implements = atype_restore(json_rarray(arr_get(serial, i++)));
  this->statics = aatt_restore(json_rarray(arr_get(serial, i++)));
  this->instance = aatt_restore(json_rarray(arr_get(serial, i++)));
  return this;
}
/*.-.*/

inline
Class *class_new(char *id) {
  return _class_new(
    id, false, achar_new(), mchar_new(), type_new_void(),
    atype_new(), aatt_new(), aatt_new()
  );
}

char *class_short_id(Class *this) {
  char *id = this->id;
  int ix = str_last_cindex(id, '_');
  return ix == -1 ? id : str_sub_end(id, ix + 1);
}

Type *class__type(Class *this, Atype *generics) {
  if (arr_size(generics) != achar_size(this->generics)) {
    return NULL;
  }
  return type_new_data(this->id, generics);
}

bool class_contains_type(Class *this, char *id) {
  if (!strcmp(id, class_short_id(this))) {
    return true;
  }

  EACH((Arr *)this->generics, char, g) {
    if (!strcmp(id, g)) {
      return true;
    }
  }_EACH

  if (map_has_key((Map *)this->imports, id)) {
    return true;
  }

  return false;
}

bool class_contains_att(Class *this, char *id) {
  EACH((Arr *)this->statics, Att, a) {
    if (!strcmp(id, att_id(a))) {
      return true;
    }
  }_EACH

  EACH((Arr *)this->instance, Att, a) {
    if (!strcmp(id, att_id(a))) {
      return true;
    }
  }_EACH

  return false;
}

inline
bool class_contains_id(Class *this, char *id) {
  return class_contains_type(this, id) || class_contains_att(this, id);
}

Type *class_actual_type(Class *this, Type *tp, Pos *pos) {
  void check_generics(char *type, int expected, int actual) {
    if (expected != actual)
      TH2(this, pos)
        "'%s' must have %d generics but has %d",
        type, expected, actual
      _TH
  }
  // -----------------------------------

  enum Type_t ttype = type_type(tp);
  if (ttype == UNKNOWN)
    THROW "ttype is unknown" _THROW

  if (ttype == VOID || ttype == ANY) {
    return tp;
  }

  Atype *ps = type_params(tp);
  EACH(ps, Type, t) {
    Type *nt = class_actual_type(this, t, pos);
    arr_set(ps, _i, nt);
  }_EACH

  if (ttype == FN) {
    return tp;
  }

  char *id = type_id(tp);
  if (type_reserved_id(id)) {
    int gen = (*id == 'A' || *id == 'M') ? 1 : 0;
    check_generics(id, gen, arr_size(ps));
    return tp;
  }

  if (!strcmp(id, class_short_id(this))) {
    check_generics(id, achar_size(this->generics), arr_size(ps));
    return type_new_data(class_id(this), ps);
  }

  bool isgen = false;
  EACH((Arr *)this->generics, char, g) {
    if (!strcmp(id, g)) {
      isgen = true;
    }
  }_EACH
  if (isgen) {
    check_generics(id, 0, arr_size(ps));
    return tp;
  }

  char *actual_id = map_oget((Map *)this->imports, id, "");
  if (!*actual_id)
    TH2(this, pos) " type '%s' is unknown", id _TH

  Class *c = rclass(cpath_from_id(actual_id));
  check_generics(id, achar_size(c->generics), arr_size(ps));
  Type *ntp = class__type(c, ps);
  if (!ntp)
    THROW exc_null_pointer("ntp") _THROW

  return ntp;
}

Aatt *class_instance_all(Class *this, Atype *generics) {
  if (arr_size(generics) != achar_size(this->generics))
    THROW
      "generics has %d elements but ought to have %d",
      arr_size(generics), achar_size(this->generics)
    _THROW

  Mtype *gen = mtype_new();
  EACH((Arr *)this->generics, char, g) {
    mtype_put(gen, g, arr_get(generics, _i));
  }_REPEAT
  Aatt *r = aatt_new();
  EACH((Arr *)this->instance, Att, a) {
    aatt_add(r, att_new(
      att_is_public(a), att_id(a),
      type_replace(att_type(a), gen),
      att_is_val(a), att_value(a)
    ));
  }_EACH

  if (type_type(this->super) == VOID) {
    return r;
  }

  Atype *ps = type_params(this->super);
  Class *s = program__class(program_get(), type_id(this->super));
  if (!s)
    THROW exc_null_pointer("s") _THROW
  if (arr_size(ps) != achar_size(s->generics))
    THROW
      "generics has %d elements but ought to have %d",
      arr_size(ps), achar_size(s->generics)
    _THROW


  Mtype *sgen = atype_new();
  EACH((Arr *)s->generics, char, g) {
    mtype_put(sgen, g, arr_get(ps, _i));
  }_EACH

  EACH((Arr *)s->instance, Att, a) {
    aatt_add(r, att_new(
      att_is_public(a), att_id(a),
      type_replace(att_type(a), sgen),
      att_is_val(a), att_value(a)
    ));
  }_EACH

  return r;
}
