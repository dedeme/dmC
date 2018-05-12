// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Class.h"

/*.
struct: @Class
  id: char *: _string
  +local: bool: _bool
  super: Achar *: achar
  generics: Achar *: achar
  imports: Mchar *: mchar
  statics: Aatt *: aatt
  instance: Aatt *: aatt
*/

/*.-.*/
struct class_Class {
  char *id;
  bool local;
  Achar *super;
  Achar *generics;
  Mchar *imports;
  Aatt *statics;
  Aatt *instance;
};

Class *_class_new(
  char *id,
  bool local,
  Achar *super,
  Achar *generics,
  Mchar *imports,
  Aatt *statics,
  Aatt *instance
) {
  Class *this = MALLOC(Class);
  this->id = id;
  this->local = local;
  this->super = super;
  this->generics = generics;
  this->imports = imports;
  this->statics = statics;
  this->instance = instance;
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
Achar *class_super(Class *this) {
  return this->super;
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
Aatt *class_statics(Class *this) {
  return this->statics;
}

inline
Aatt *class_instance(Class *this) {
  return this->instance;
}

Arr/*Json*/ *class_serialize(Class *this) {
  Arr/*Json*/ *serial = arr_new();
  if (!this) return serial;
  jarr_astring(serial, this->id);
  jarr_abool(serial, this->local);
  arr_add(serial, json_warray(achar_serialize(this->super)));
  arr_add(serial, json_warray(achar_serialize(this->generics)));
  arr_add(serial, json_warray(mchar_serialize(this->imports)));
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
  this->super = achar_restore(json_rarray(arr_get(serial, i++)));
  this->generics = achar_restore(json_rarray(arr_get(serial, i++)));
  this->imports = mchar_restore(json_rarray(arr_get(serial, i++)));
  this->statics = aatt_restore(json_rarray(arr_get(serial, i++)));
  this->instance = aatt_restore(json_rarray(arr_get(serial, i++)));
  return this;
}
/*.-.*/

inline
Class *class_new(char *id) {
  return _class_new(
    id, false, achar_new(), achar_new(), mchar_new(), aatt_new(), aatt_new()
  );
}

Type *class__type(Class *this, Atype *generics) {
  if (arr_size(generics) != arr_size(this->generics)) {
    return NULL;
  }
  return type_new_data(this->id, generics);
}

bool class_contains_id(Class *this, char *id) {
  EACH(this->generics, char, g) {
    if (!strcmp(id, g)) {
      return true;
    }
  }_EACH

  if (map_has_key(this->imports, id)) {
    return true;
  }

  EACH(this->statics, Att, a) {
    if (!strcmp(id, att_id(a))) {
      return true;
    }
  }_EACH

  EACH(this->instance, Att, a) {
    if (!strcmp(id, att_id(a))) {
      return true;
    }
  }_EACH

  return false;
}

bool class_add_import(Class *this, char *id, char *value) {
  if (class_contains_id(this, id)) {
    return false;
  }
  mchar_put(this->imports, id, value);
  return true;
}
